6500 // Multiprocessor support
6501 // Search memory for MP description structures.
6502 // http://developer.intel.com/design/pentium/datashts/24201606.pdf
6503 
6504 #include "types.h"
6505 #include "defs.h"
6506 #include "param.h"
6507 #include "memlayout.h"
6508 #include "mp.h"
6509 #include "x86.h"
6510 #include "mmu.h"
6511 #include "proc.h"
6512 
6513 struct cpu cpus[NCPU];
6514 int ncpu;
6515 uchar ioapicid;
6516 
6517 static uchar
6518 sum(uchar *addr, int len)
6519 {
6520   int i, sum;
6521 
6522   sum = 0;
6523   for(i=0; i<len; i++)
6524     sum += addr[i];
6525   return sum;
6526 }
6527 
6528 // Look for an MP structure in the len bytes at addr.
6529 static struct mp*
6530 mpsearch1(uint a, int len)
6531 {
6532   uchar *e, *p, *addr;
6533 
6534   addr = P2V(a);
6535   e = addr+len;
6536   for(p = addr; p < e; p += sizeof(struct mp))
6537     if(memcmp(p, "_MP_", 4) == 0 && sum(p, sizeof(struct mp)) == 0)
6538       return (struct mp*)p;
6539   return 0;
6540 }
6541 
6542 
6543 
6544 
6545 
6546 
6547 
6548 
6549 
6550 // Search for the MP Floating Pointer Structure, which according to the
6551 // spec is in one of the following three locations:
6552 // 1) in the first KB of the EBDA;
6553 // 2) in the last KB of system base memory;
6554 // 3) in the BIOS ROM between 0xE0000 and 0xFFFFF.
6555 static struct mp*
6556 mpsearch(void)
6557 {
6558   uchar *bda;
6559   uint p;
6560   struct mp *mp;
6561 
6562   bda = (uchar *) P2V(0x400);
6563   if((p = ((bda[0x0F]<<8)| bda[0x0E]) << 4)){
6564     if((mp = mpsearch1(p, 1024)))
6565       return mp;
6566   } else {
6567     p = ((bda[0x14]<<8)|bda[0x13])*1024;
6568     if((mp = mpsearch1(p-1024, 1024)))
6569       return mp;
6570   }
6571   return mpsearch1(0xF0000, 0x10000);
6572 }
6573 
6574 // Search for an MP configuration table.  For now,
6575 // don't accept the default configurations (physaddr == 0).
6576 // Check for correct signature, calculate the checksum and,
6577 // if correct, check the version.
6578 // To do: check extended table checksum.
6579 static struct mpconf*
6580 mpconfig(struct mp **pmp)
6581 {
6582   struct mpconf *conf;
6583   struct mp *mp;
6584 
6585   if((mp = mpsearch()) == 0 || mp->physaddr == 0)
6586     return 0;
6587   conf = (struct mpconf*) P2V((uint) mp->physaddr);
6588   if(memcmp(conf, "PCMP", 4) != 0)
6589     return 0;
6590   if(conf->version != 1 && conf->version != 4)
6591     return 0;
6592   if(sum((uchar*)conf, conf->length) != 0)
6593     return 0;
6594   *pmp = mp;
6595   return conf;
6596 }
6597 
6598 
6599 
6600 void
6601 mpinit(void)
6602 {
6603   uchar *p, *e;
6604   int ismp;
6605   struct mp *mp;
6606   struct mpconf *conf;
6607   struct mpproc *proc;
6608   struct mpioapic *ioapic;
6609 
6610   if((conf = mpconfig(&mp)) == 0)
6611     panic("Expect to run on an SMP");
6612   ismp = 1;
6613   lapic = (uint*)conf->lapicaddr;
6614   for(p=(uchar*)(conf+1), e=(uchar*)conf+conf->length; p<e; ){
6615     switch(*p){
6616     case MPPROC:
6617       proc = (struct mpproc*)p;
6618       if(ncpu < NCPU) {
6619         cpus[ncpu].apicid = proc->apicid;  // apicid may differ from ncpu
6620         ncpu++;
6621       }
6622       p += sizeof(struct mpproc);
6623       continue;
6624     case MPIOAPIC:
6625       ioapic = (struct mpioapic*)p;
6626       ioapicid = ioapic->apicno;
6627       p += sizeof(struct mpioapic);
6628       continue;
6629     case MPBUS:
6630     case MPIOINTR:
6631     case MPLINTR:
6632       p += 8;
6633       continue;
6634     default:
6635       ismp = 0;
6636       break;
6637     }
6638   }
6639   if(!ismp)
6640     panic("Didn't find a suitable machine");
6641 
6642   if(mp->imcrp){
6643     // Bochs doesn't support IMCR, so this doesn't run on Bochs.
6644     // But it would on real hardware.
6645     outb(0x22, 0x70);   // Select IMCR
6646     outb(0x23, inb(0x23) | 1);  // Mask external interrupts.
6647   }
6648 }
6649 
