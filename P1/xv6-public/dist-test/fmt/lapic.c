6650 // The local APIC manages internal (non-I/O) interrupts.
6651 // See Chapter 8 & Appendix C of Intel processor manual volume 3.
6652 
6653 #include "param.h"
6654 #include "types.h"
6655 #include "defs.h"
6656 #include "date.h"
6657 #include "memlayout.h"
6658 #include "traps.h"
6659 #include "mmu.h"
6660 #include "x86.h"
6661 
6662 // Local APIC registers, divided by 4 for use as uint[] indices.
6663 #define ID      (0x0020/4)   // ID
6664 #define VER     (0x0030/4)   // Version
6665 #define TPR     (0x0080/4)   // Task Priority
6666 #define EOI     (0x00B0/4)   // EOI
6667 #define SVR     (0x00F0/4)   // Spurious Interrupt Vector
6668   #define ENABLE     0x00000100   // Unit Enable
6669 #define ESR     (0x0280/4)   // Error Status
6670 #define ICRLO   (0x0300/4)   // Interrupt Command
6671   #define INIT       0x00000500   // INIT/RESET
6672   #define STARTUP    0x00000600   // Startup IPI
6673   #define DELIVS     0x00001000   // Delivery status
6674   #define ASSERT     0x00004000   // Assert interrupt (vs deassert)
6675   #define DEASSERT   0x00000000
6676   #define LEVEL      0x00008000   // Level triggered
6677   #define BCAST      0x00080000   // Send to all APICs, including self.
6678   #define BUSY       0x00001000
6679   #define FIXED      0x00000000
6680 #define ICRHI   (0x0310/4)   // Interrupt Command [63:32]
6681 #define TIMER   (0x0320/4)   // Local Vector Table 0 (TIMER)
6682   #define X1         0x0000000B   // divide counts by 1
6683   #define PERIODIC   0x00020000   // Periodic
6684 #define PCINT   (0x0340/4)   // Performance Counter LVT
6685 #define LINT0   (0x0350/4)   // Local Vector Table 1 (LINT0)
6686 #define LINT1   (0x0360/4)   // Local Vector Table 2 (LINT1)
6687 #define ERROR   (0x0370/4)   // Local Vector Table 3 (ERROR)
6688   #define MASKED     0x00010000   // Interrupt masked
6689 #define TICR    (0x0380/4)   // Timer Initial Count
6690 #define TCCR    (0x0390/4)   // Timer Current Count
6691 #define TDCR    (0x03E0/4)   // Timer Divide Configuration
6692 
6693 volatile uint *lapic;  // Initialized in mp.c
6694 
6695 static void
6696 lapicw(int index, int value)
6697 {
6698   lapic[index] = value;
6699   lapic[ID];  // wait for write to finish, by reading
6700 }
6701 
6702 void
6703 lapicinit(void)
6704 {
6705   if(!lapic)
6706     return;
6707 
6708   // Enable local APIC; set spurious interrupt vector.
6709   lapicw(SVR, ENABLE | (T_IRQ0 + IRQ_SPURIOUS));
6710 
6711   // The timer repeatedly counts down at bus frequency
6712   // from lapic[TICR] and then issues an interrupt.
6713   // If xv6 cared more about precise timekeeping,
6714   // TICR would be calibrated using an external time source.
6715   lapicw(TDCR, X1);
6716   lapicw(TIMER, PERIODIC | (T_IRQ0 + IRQ_TIMER));
6717   lapicw(TICR, 10000000);
6718 
6719   // Disable logical interrupt lines.
6720   lapicw(LINT0, MASKED);
6721   lapicw(LINT1, MASKED);
6722 
6723   // Disable performance counter overflow interrupts
6724   // on machines that provide that interrupt entry.
6725   if(((lapic[VER]>>16) & 0xFF) >= 4)
6726     lapicw(PCINT, MASKED);
6727 
6728   // Map error interrupt to IRQ_ERROR.
6729   lapicw(ERROR, T_IRQ0 + IRQ_ERROR);
6730 
6731   // Clear error status register (requires back-to-back writes).
6732   lapicw(ESR, 0);
6733   lapicw(ESR, 0);
6734 
6735   // Ack any outstanding interrupts.
6736   lapicw(EOI, 0);
6737 
6738   // Send an Init Level De-Assert to synchronise arbitration ID's.
6739   lapicw(ICRHI, 0);
6740   lapicw(ICRLO, BCAST | INIT | LEVEL);
6741   while(lapic[ICRLO] & DELIVS)
6742     ;
6743 
6744   // Enable interrupts on the APIC (but not on the processor).
6745   lapicw(TPR, 0);
6746 }
6747 
6748 
6749 
6750 int
6751 lapicid(void)
6752 {
6753   if (!lapic)
6754     return 0;
6755   return lapic[ID] >> 24;
6756 }
6757 
6758 // Acknowledge interrupt.
6759 void
6760 lapiceoi(void)
6761 {
6762   if(lapic)
6763     lapicw(EOI, 0);
6764 }
6765 
6766 // Spin for a given number of microseconds.
6767 // On real hardware would want to tune this dynamically.
6768 void
6769 microdelay(int us)
6770 {
6771 }
6772 
6773 #define CMOS_PORT    0x70
6774 #define CMOS_RETURN  0x71
6775 
6776 // Start additional processor running entry code at addr.
6777 // See Appendix B of MultiProcessor Specification.
6778 void
6779 lapicstartap(uchar apicid, uint addr)
6780 {
6781   int i;
6782   ushort *wrv;
6783 
6784   // "The BSP must initialize CMOS shutdown code to 0AH
6785   // and the warm reset vector (DWORD based at 40:67) to point at
6786   // the AP startup code prior to the [universal startup algorithm]."
6787   outb(CMOS_PORT, 0xF);  // offset 0xF is shutdown code
6788   outb(CMOS_PORT+1, 0x0A);
6789   wrv = (ushort*)P2V((0x40<<4 | 0x67));  // Warm reset vector
6790   wrv[0] = 0;
6791   wrv[1] = addr >> 4;
6792 
6793   // "Universal startup algorithm."
6794   // Send INIT (level-triggered) interrupt to reset other CPU.
6795   lapicw(ICRHI, apicid<<24);
6796   lapicw(ICRLO, INIT | LEVEL | ASSERT);
6797   microdelay(200);
6798   lapicw(ICRLO, INIT | LEVEL);
6799   microdelay(100);    // should be 10ms, but too slow in Bochs!
6800   // Send startup IPI (twice!) to enter code.
6801   // Regular hardware is supposed to only accept a STARTUP
6802   // when it is in the halted state due to an INIT.  So the second
6803   // should be ignored, but it is part of the official Intel algorithm.
6804   // Bochs complains about the second one.  Too bad for Bochs.
6805   for(i = 0; i < 2; i++){
6806     lapicw(ICRHI, apicid<<24);
6807     lapicw(ICRLO, STARTUP | (addr>>12));
6808     microdelay(200);
6809   }
6810 }
6811 
6812 #define CMOS_STATA   0x0a
6813 #define CMOS_STATB   0x0b
6814 #define CMOS_UIP    (1 << 7)        // RTC update in progress
6815 
6816 #define SECS    0x00
6817 #define MINS    0x02
6818 #define HOURS   0x04
6819 #define DAY     0x07
6820 #define MONTH   0x08
6821 #define YEAR    0x09
6822 
6823 static uint
6824 cmos_read(uint reg)
6825 {
6826   outb(CMOS_PORT,  reg);
6827   microdelay(200);
6828 
6829   return inb(CMOS_RETURN);
6830 }
6831 
6832 static void
6833 fill_rtcdate(struct rtcdate *r)
6834 {
6835   r->second = cmos_read(SECS);
6836   r->minute = cmos_read(MINS);
6837   r->hour   = cmos_read(HOURS);
6838   r->day    = cmos_read(DAY);
6839   r->month  = cmos_read(MONTH);
6840   r->year   = cmos_read(YEAR);
6841 }
6842 
6843 
6844 
6845 
6846 
6847 
6848 
6849 
6850 // qemu seems to use 24-hour GWT and the values are BCD encoded
6851 void
6852 cmostime(struct rtcdate *r)
6853 {
6854   struct rtcdate t1, t2;
6855   int sb, bcd;
6856 
6857   sb = cmos_read(CMOS_STATB);
6858 
6859   bcd = (sb & (1 << 2)) == 0;
6860 
6861   // make sure CMOS doesn't modify time while we read it
6862   for(;;) {
6863     fill_rtcdate(&t1);
6864     if(cmos_read(CMOS_STATA) & CMOS_UIP)
6865         continue;
6866     fill_rtcdate(&t2);
6867     if(memcmp(&t1, &t2, sizeof(t1)) == 0)
6868       break;
6869   }
6870 
6871   // convert
6872   if(bcd) {
6873 #define    CONV(x)     (t1.x = ((t1.x >> 4) * 10) + (t1.x & 0xf))
6874     CONV(second);
6875     CONV(minute);
6876     CONV(hour  );
6877     CONV(day   );
6878     CONV(month );
6879     CONV(year  );
6880 #undef     CONV
6881   }
6882 
6883   *r = t1;
6884   r->year += 2000;
6885 }
6886 
6887 
6888 
6889 
6890 
6891 
6892 
6893 
6894 
6895 
6896 
6897 
6898 
6899 
