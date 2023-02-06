8300 #include "asm.h"
8301 #include "memlayout.h"
8302 #include "mmu.h"
8303 
8304 # Start the first CPU: switch to 32-bit protected mode, jump into C.
8305 # The BIOS loads this code from the first sector of the hard disk into
8306 # memory at physical address 0x7c00 and starts executing in real mode
8307 # with %cs=0 %ip=7c00.
8308 
8309 .code16                       # Assemble for 16-bit mode
8310 .globl start
8311 start:
8312   cli                         # BIOS enabled interrupts; disable
8313 
8314   # Zero data segment registers DS, ES, and SS.
8315   xorw    %ax,%ax             # Set %ax to zero
8316   movw    %ax,%ds             # -> Data Segment
8317   movw    %ax,%es             # -> Extra Segment
8318   movw    %ax,%ss             # -> Stack Segment
8319 
8320   # Physical address line A20 is tied to zero so that the first PCs
8321   # with 2 MB would run software that assumed 1 MB.  Undo that.
8322 seta20.1:
8323   inb     $0x64,%al               # Wait for not busy
8324   testb   $0x2,%al
8325   jnz     seta20.1
8326 
8327   movb    $0xd1,%al               # 0xd1 -> port 0x64
8328   outb    %al,$0x64
8329 
8330 seta20.2:
8331   inb     $0x64,%al               # Wait for not busy
8332   testb   $0x2,%al
8333   jnz     seta20.2
8334 
8335   movb    $0xdf,%al               # 0xdf -> port 0x60
8336   outb    %al,$0x60
8337 
8338   # Switch from real to protected mode.  Use a bootstrap GDT that makes
8339   # virtual addresses map directly to physical addresses so that the
8340   # effective memory map doesn't change during the transition.
8341   lgdt    gdtdesc
8342   movl    %cr0, %eax
8343   orl     $CR0_PE, %eax
8344   movl    %eax, %cr0
8345 
8346   # Complete the transition to 32-bit protected mode by using a long jmp
8347   # to reload %cs and %eip.  The segment descriptors are set up with no
8348   # translation, so that the mapping is still the identity mapping.
8349   ljmp    $(SEG_KCODE<<3), $start32
8350 .code32  # Tell assembler to generate 32-bit code now.
8351 start32:
8352   # Set up the protected-mode data segment registers
8353   movw    $(SEG_KDATA<<3), %ax    # Our data segment selector
8354   movw    %ax, %ds                # -> DS: Data Segment
8355   movw    %ax, %es                # -> ES: Extra Segment
8356   movw    %ax, %ss                # -> SS: Stack Segment
8357   movw    $0, %ax                 # Zero segments not ready for use
8358   movw    %ax, %fs                # -> FS
8359   movw    %ax, %gs                # -> GS
8360 
8361   # Set up the stack pointer and call into C.
8362   movl    $start, %esp
8363   call    bootmain
8364 
8365   # If bootmain returns (it shouldn't), trigger a Bochs
8366   # breakpoint if running under Bochs, then loop.
8367   movw    $0x8a00, %ax            # 0x8a00 -> port 0x8a00
8368   movw    %ax, %dx
8369   outw    %ax, %dx
8370   movw    $0x8ae0, %ax            # 0x8ae0 -> port 0x8a00
8371   outw    %ax, %dx
8372 spin:
8373   jmp     spin
8374 
8375 # Bootstrap GDT
8376 .p2align 2                                # force 4 byte alignment
8377 gdt:
8378   SEG_NULLASM                             # null seg
8379   SEG_ASM(STA_X|STA_R, 0x0, 0xffffffff)   # code seg
8380   SEG_ASM(STA_W, 0x0, 0xffffffff)         # data seg
8381 
8382 gdtdesc:
8383   .word   (gdtdesc - gdt - 1)             # sizeof(gdt) - 1
8384   .long   gdt                             # address gdt
8385 
8386 
8387 
8388 
8389 
8390 
8391 
8392 
8393 
8394 
8395 
8396 
8397 
8398 
8399 
