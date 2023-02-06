2950 #include "types.h"
2951 #include "defs.h"
2952 #include "param.h"
2953 #include "memlayout.h"
2954 #include "mmu.h"
2955 #include "proc.h"
2956 #include "x86.h"
2957 #include "traps.h"
2958 #include "spinlock.h"
2959 
2960 // Interrupt descriptor table (shared by all CPUs).
2961 struct gatedesc idt[256];
2962 extern uint vectors[];  // in vectors.S: array of 256 entry pointers
2963 struct spinlock tickslock;
2964 uint ticks;
2965 
2966 void
2967 tvinit(void)
2968 {
2969   int i;
2970 
2971   for(i = 0; i < 256; i++)
2972     SETGATE(idt[i], 0, SEG_KCODE<<3, vectors[i], 0);
2973   SETGATE(idt[T_SYSCALL], 1, SEG_KCODE<<3, vectors[T_SYSCALL], DPL_USER);
2974 
2975   initlock(&tickslock, "time");
2976 }
2977 
2978 void
2979 idtinit(void)
2980 {
2981   lidt(idt, sizeof(idt));
2982 }
2983 
2984 void
2985 trap(struct trapframe *tf)
2986 {
2987   if(tf->trapno == T_SYSCALL){
2988     if(myproc()->killed)
2989       exit();
2990     myproc()->tf = tf;
2991     syscall();
2992     if(myproc()->killed)
2993       exit();
2994     return;
2995   }
2996 
2997   switch(tf->trapno){
2998   case T_IRQ0 + IRQ_TIMER:
2999     if(cpuid() == 0){
3000       acquire(&tickslock);
3001       ticks++;
3002       wakeup(&ticks);
3003       release(&tickslock);
3004     }
3005     lapiceoi();
3006     break;
3007   case T_IRQ0 + IRQ_IDE:
3008     ideintr();
3009     lapiceoi();
3010     break;
3011   case T_IRQ0 + IRQ_IDE+1:
3012     // Bochs generates spurious IDE1 interrupts.
3013     break;
3014   case T_IRQ0 + IRQ_KBD:
3015     kbdintr();
3016     lapiceoi();
3017     break;
3018   case T_IRQ0 + IRQ_COM1:
3019     uartintr();
3020     lapiceoi();
3021     break;
3022   case T_IRQ0 + 7:
3023   case T_IRQ0 + IRQ_SPURIOUS:
3024     cprintf("cpu%d: spurious interrupt at %x:%x\n",
3025             cpuid(), tf->cs, tf->eip);
3026     lapiceoi();
3027     break;
3028 
3029   default:
3030     if(myproc() == 0 || (tf->cs&3) == 0){
3031       // In kernel, it must be our mistake.
3032       cprintf("unexpected trap %d from cpu %d eip %x (cr2=0x%x)\n",
3033               tf->trapno, cpuid(), tf->eip, rcr2());
3034       panic("trap");
3035     }
3036     // In user space, assume process misbehaved.
3037     cprintf("pid %d %s: trap %d err %d on cpu %d "
3038             "eip 0x%x addr 0x%x--kill proc\n",
3039             myproc()->pid, myproc()->name, tf->trapno,
3040             tf->err, cpuid(), tf->eip, rcr2());
3041     myproc()->killed = 1;
3042   }
3043 
3044   // Force process exit if it has been killed and is in user space.
3045   // (If it is still executing in the kernel, let it keep running
3046   // until it gets to the regular system call return.)
3047   if(myproc() && myproc()->killed && (tf->cs&3) == DPL_USER)
3048     exit();
3049 
3050   // Force process to give up CPU on clock tick.
3051   // If interrupts were on while locks held, would need to check nlock.
3052   if(myproc() && myproc()->state == RUNNING &&
3053      tf->trapno == T_IRQ0+IRQ_TIMER)
3054     yield();
3055 
3056   // Check if the process has been killed since we yielded
3057   if(myproc() && myproc()->killed && (tf->cs&3) == DPL_USER)
3058     exit();
3059 }
3060 
3061 
3062 
3063 
3064 
3065 
3066 
3067 
3068 
3069 
3070 
3071 
3072 
3073 
3074 
3075 
3076 
3077 
3078 
3079 
3080 
3081 
3082 
3083 
3084 
3085 
3086 
3087 
3088 
3089 
3090 
3091 
3092 
3093 
3094 
3095 
3096 
3097 
3098 
3099 
