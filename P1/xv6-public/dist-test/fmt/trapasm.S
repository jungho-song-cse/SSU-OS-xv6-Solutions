2900 #include "mmu.h"
2901 
2902   # vectors.S sends all traps here.
2903 .globl alltraps
2904 alltraps:
2905   # Build trap frame.
2906   pushl %ds
2907   pushl %es
2908   pushl %fs
2909   pushl %gs
2910   pushal
2911 
2912   # Set up data segments.
2913   movw $(SEG_KDATA<<3), %ax
2914   movw %ax, %ds
2915   movw %ax, %es
2916 
2917   # Call trap(tf), where tf=%esp
2918   pushl %esp
2919   call trap
2920   addl $4, %esp
2921 
2922   # Return falls through to trapret...
2923 .globl trapret
2924 trapret:
2925   popal
2926   popl %gs
2927   popl %fs
2928   popl %es
2929   popl %ds
2930   addl $0x8, %esp  # trapno and errcode
2931   iret
2932 
2933 
2934 
2935 
2936 
2937 
2938 
2939 
2940 
2941 
2942 
2943 
2944 
2945 
2946 
2947 
2948 
2949 
