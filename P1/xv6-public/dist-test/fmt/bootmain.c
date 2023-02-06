8400 // Boot loader.
8401 //
8402 // Part of the boot block, along with bootasm.S, which calls bootmain().
8403 // bootasm.S has put the processor into protected 32-bit mode.
8404 // bootmain() loads an ELF kernel image from the disk starting at
8405 // sector 1 and then jumps to the kernel entry routine.
8406 
8407 #include "types.h"
8408 #include "elf.h"
8409 #include "x86.h"
8410 #include "memlayout.h"
8411 
8412 #define SECTSIZE  512
8413 
8414 void readseg(uchar*, uint, uint);
8415 
8416 void
8417 bootmain(void)
8418 {
8419   struct elfhdr *elf;
8420   struct proghdr *ph, *eph;
8421   void (*entry)(void);
8422   uchar* pa;
8423 
8424   elf = (struct elfhdr*)0x10000;  // scratch space
8425 
8426   // Read 1st page off disk
8427   readseg((uchar*)elf, 4096, 0);
8428 
8429   // Is this an ELF executable?
8430   if(elf->magic != ELF_MAGIC)
8431     return;  // let bootasm.S handle error
8432 
8433   // Load each program segment (ignores ph flags).
8434   ph = (struct proghdr*)((uchar*)elf + elf->phoff);
8435   eph = ph + elf->phnum;
8436   for(; ph < eph; ph++){
8437     pa = (uchar*)ph->paddr;
8438     readseg(pa, ph->filesz, ph->off);
8439     if(ph->memsz > ph->filesz)
8440       stosb(pa + ph->filesz, 0, ph->memsz - ph->filesz);
8441   }
8442 
8443   // Call the entry point from the ELF header.
8444   // Does not return!
8445   entry = (void(*)(void))(elf->entry);
8446   entry();
8447 }
8448 
8449 
8450 void
8451 waitdisk(void)
8452 {
8453   // Wait for disk ready.
8454   while((inb(0x1F7) & 0xC0) != 0x40)
8455     ;
8456 }
8457 
8458 // Read a single sector at offset into dst.
8459 void
8460 readsect(void *dst, uint offset)
8461 {
8462   // Issue command.
8463   waitdisk();
8464   outb(0x1F2, 1);   // count = 1
8465   outb(0x1F3, offset);
8466   outb(0x1F4, offset >> 8);
8467   outb(0x1F5, offset >> 16);
8468   outb(0x1F6, (offset >> 24) | 0xE0);
8469   outb(0x1F7, 0x20);  // cmd 0x20 - read sectors
8470 
8471   // Read data.
8472   waitdisk();
8473   insl(0x1F0, dst, SECTSIZE/4);
8474 }
8475 
8476 // Read 'count' bytes at 'offset' from kernel into physical address 'pa'.
8477 // Might copy more than asked.
8478 void
8479 readseg(uchar* pa, uint count, uint offset)
8480 {
8481   uchar* epa;
8482 
8483   epa = pa + count;
8484 
8485   // Round down to sector boundary.
8486   pa -= offset % SECTSIZE;
8487 
8488   // Translate from bytes to sectors; kernel starts at sector 1.
8489   offset = (offset / SECTSIZE) + 1;
8490 
8491   // If this is too slow, we could read lots of sectors at a time.
8492   // We'd write more to memory than asked, but it doesn't matter --
8493   // we load in increasing order.
8494   for(; pa < epa; pa += SECTSIZE, offset++)
8495     readsect(pa, offset);
8496 }
8497 
8498 
8499 
