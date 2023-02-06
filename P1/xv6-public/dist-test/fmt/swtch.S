2650 # Context switch
2651 #
2652 #   void swtch(struct context **old, struct context *new);
2653 #
2654 # Save the current registers on the stack, creating
2655 # a struct context, and save its address in *old.
2656 # Switch stacks to new and pop previously-saved registers.
2657 
2658 .globl swtch
2659 swtch:
2660   movl 4(%esp), %eax
2661   movl 8(%esp), %edx
2662 
2663   # Save old callee-saved registers
2664   pushl %ebp
2665   pushl %ebx
2666   pushl %esi
2667   pushl %edi
2668 
2669   # Switch stacks
2670   movl %esp, (%eax)
2671   movl %edx, %esp
2672 
2673   # Load new callee-saved registers
2674   popl %edi
2675   popl %esi
2676   popl %ebx
2677   popl %ebp
2678   ret
2679 
2680 
2681 
2682 
2683 
2684 
2685 
2686 
2687 
2688 
2689 
2690 
2691 
2692 
2693 
2694 
2695 
2696 
2697 
2698 
2699 
