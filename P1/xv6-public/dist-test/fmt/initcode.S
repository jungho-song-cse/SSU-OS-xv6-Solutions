7600 # Initial process execs /init.
7601 # This code runs in user space.
7602 
7603 #include "syscall.h"
7604 #include "traps.h"
7605 
7606 
7607 # exec(init, argv)
7608 .globl start
7609 start:
7610   pushl $argv
7611   pushl $init
7612   pushl $0  // where caller pc would be
7613   movl $SYS_exec, %eax
7614   int $T_SYSCALL
7615 
7616 # for(;;) exit();
7617 exit:
7618   movl $SYS_exit, %eax
7619   int $T_SYSCALL
7620   jmp exit
7621 
7622 # char init[] = "/init\0";
7623 init:
7624   .string "/init\0"
7625 
7626 # char *argv[] = { init, 0 };
7627 .p2align 2
7628 argv:
7629   .long init
7630   .long 0
7631 
7632 
7633 
7634 
7635 
7636 
7637 
7638 
7639 
7640 
7641 
7642 
7643 
7644 
7645 
7646 
7647 
7648 
7649 
