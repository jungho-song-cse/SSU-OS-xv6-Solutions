2800 // x86 trap and interrupt constants.
2801 
2802 // Processor-defined:
2803 #define T_DIVIDE         0      // divide error
2804 #define T_DEBUG          1      // debug exception
2805 #define T_NMI            2      // non-maskable interrupt
2806 #define T_BRKPT          3      // breakpoint
2807 #define T_OFLOW          4      // overflow
2808 #define T_BOUND          5      // bounds check
2809 #define T_ILLOP          6      // illegal opcode
2810 #define T_DEVICE         7      // device not available
2811 #define T_DBLFLT         8      // double fault
2812 // #define T_COPROC      9      // reserved (not used since 486)
2813 #define T_TSS           10      // invalid task switch segment
2814 #define T_SEGNP         11      // segment not present
2815 #define T_STACK         12      // stack exception
2816 #define T_GPFLT         13      // general protection fault
2817 #define T_PGFLT         14      // page fault
2818 // #define T_RES        15      // reserved
2819 #define T_FPERR         16      // floating point error
2820 #define T_ALIGN         17      // aligment check
2821 #define T_MCHK          18      // machine check
2822 #define T_SIMDERR       19      // SIMD floating point error
2823 
2824 // These are arbitrarily chosen, but with care not to overlap
2825 // processor defined exceptions or interrupt vectors.
2826 #define T_SYSCALL       64      // system call
2827 #define T_DEFAULT      500      // catchall
2828 
2829 #define T_IRQ0          32      // IRQ 0 corresponds to int T_IRQ
2830 
2831 #define IRQ_TIMER        0
2832 #define IRQ_KBD          1
2833 #define IRQ_COM1         4
2834 #define IRQ_IDE         14
2835 #define IRQ_ERROR       19
2836 #define IRQ_SPURIOUS    31
2837 
2838 
2839 
2840 
2841 
2842 
2843 
2844 
2845 
2846 
2847 
2848 
2849 
