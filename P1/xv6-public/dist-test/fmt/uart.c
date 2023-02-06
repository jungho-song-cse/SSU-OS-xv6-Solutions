7500 // Intel 8250 serial port (UART).
7501 
7502 #include "types.h"
7503 #include "defs.h"
7504 #include "param.h"
7505 #include "traps.h"
7506 #include "spinlock.h"
7507 #include "sleeplock.h"
7508 #include "fs.h"
7509 #include "file.h"
7510 #include "mmu.h"
7511 #include "proc.h"
7512 #include "x86.h"
7513 
7514 #define COM1    0x3f8
7515 
7516 static int uart;    // is there a uart?
7517 
7518 void
7519 uartinit(void)
7520 {
7521   char *p;
7522 
7523   // Turn off the FIFO
7524   outb(COM1+2, 0);
7525 
7526   // 9600 baud, 8 data bits, 1 stop bit, parity off.
7527   outb(COM1+3, 0x80);    // Unlock divisor
7528   outb(COM1+0, 115200/9600);
7529   outb(COM1+1, 0);
7530   outb(COM1+3, 0x03);    // Lock divisor, 8 data bits.
7531   outb(COM1+4, 0);
7532   outb(COM1+1, 0x01);    // Enable receive interrupts.
7533 
7534   // If status is 0xFF, no serial port.
7535   if(inb(COM1+5) == 0xFF)
7536     return;
7537   uart = 1;
7538 
7539   // Acknowledge pre-existing interrupt conditions;
7540   // enable interrupts.
7541   inb(COM1+2);
7542   inb(COM1+0);
7543   ioapicenable(IRQ_COM1, 0);
7544 
7545   // Announce that we're here.
7546   for(p="xv6...\n"; *p; p++)
7547     uartputc(*p);
7548 }
7549 
7550 void
7551 uartputc(int c)
7552 {
7553   int i;
7554 
7555   if(!uart)
7556     return;
7557   for(i = 0; i < 128 && !(inb(COM1+5) & 0x20); i++)
7558     microdelay(10);
7559   outb(COM1+0, c);
7560 }
7561 
7562 static int
7563 uartgetc(void)
7564 {
7565   if(!uart)
7566     return -1;
7567   if(!(inb(COM1+5) & 0x01))
7568     return -1;
7569   return inb(COM1+0);
7570 }
7571 
7572 void
7573 uartintr(void)
7574 {
7575   consoleintr(uartgetc);
7576 }
7577 
7578 
7579 
7580 
7581 
7582 
7583 
7584 
7585 
7586 
7587 
7588 
7589 
7590 
7591 
7592 
7593 
7594 
7595 
7596 
7597 
7598 
7599 
