7650 #include "syscall.h"
7651 #include "traps.h"
7652 
7653 #define SYSCALL(name) \
7654   .globl name; \
7655   name: \
7656     movl $SYS_ ## name, %eax; \
7657     int $T_SYSCALL; \
7658     ret
7659 
7660 SYSCALL(fork)
7661 SYSCALL(exit)
7662 SYSCALL(wait)
7663 SYSCALL(pipe)
7664 SYSCALL(read)
7665 SYSCALL(write)
7666 SYSCALL(close)
7667 SYSCALL(kill)
7668 SYSCALL(exec)
7669 SYSCALL(open)
7670 SYSCALL(mknod)
7671 SYSCALL(unlink)
7672 SYSCALL(fstat)
7673 SYSCALL(link)
7674 SYSCALL(mkdir)
7675 SYSCALL(chdir)
7676 SYSCALL(dup)
7677 SYSCALL(getpid)
7678 SYSCALL(sbrk)
7679 SYSCALL(sleep)
7680 SYSCALL(uptime)
7681 
7682 
7683 
7684 
7685 
7686 
7687 
7688 
7689 
7690 
7691 
7692 
7693 
7694 
7695 
7696 
7697 
7698 
7699 
