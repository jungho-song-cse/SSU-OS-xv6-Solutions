2700 // Physical memory allocator, intended to allocate
2701 // memory for user processes, kernel stacks, page table pages,
2702 // and pipe buffers. Allocates 4096-byte pages.
2703 
2704 #include "types.h"
2705 #include "defs.h"
2706 #include "param.h"
2707 #include "memlayout.h"
2708 #include "mmu.h"
2709 #include "spinlock.h"
2710 
2711 void freerange(void *vstart, void *vend);
2712 extern char end[]; // first address after kernel loaded from ELF file
2713                    // defined by the kernel linker script in kernel.ld
2714 
2715 struct run {
2716   struct run *next;
2717 };
2718 
2719 struct {
2720   struct spinlock lock;
2721   int use_lock;
2722   struct run *freelist;
2723 } kmem;
2724 
2725 // Initialization happens in two phases.
2726 // 1. main() calls kinit1() while still using entrypgdir to place just
2727 // the pages mapped by entrypgdir on free list.
2728 // 2. main() calls kinit2() with the rest of the physical pages
2729 // after installing a full page table that maps them on all cores.
2730 void
2731 kinit1(void *vstart, void *vend)
2732 {
2733   initlock(&kmem.lock, "kmem");
2734   kmem.use_lock = 0;
2735   freerange(vstart, vend);
2736 }
2737 
2738 void
2739 kinit2(void *vstart, void *vend)
2740 {
2741   freerange(vstart, vend);
2742   kmem.use_lock = 1;
2743 }
2744 
2745 
2746 
2747 
2748 
2749 
2750 void
2751 freerange(void *vstart, void *vend)
2752 {
2753   char *p;
2754   p = (char*)PGROUNDUP((uint)vstart);
2755   for(; p + PGSIZE <= (char*)vend; p += PGSIZE)
2756     kfree(p);
2757 }
2758 // Free the page of physical memory pointed at by v,
2759 // which normally should have been returned by a
2760 // call to kalloc().  (The exception is when
2761 // initializing the allocator; see kinit above.)
2762 void
2763 kfree(char *v)
2764 {
2765   struct run *r;
2766 
2767   if((uint)v % PGSIZE || v < end || V2P(v) >= PHYSTOP)
2768     panic("kfree");
2769 
2770   // Fill with junk to catch dangling refs.
2771   memset(v, 1, PGSIZE);
2772 
2773   if(kmem.use_lock)
2774     acquire(&kmem.lock);
2775   r = (struct run*)v;
2776   r->next = kmem.freelist;
2777   kmem.freelist = r;
2778   if(kmem.use_lock)
2779     release(&kmem.lock);
2780 }
2781 
2782 // Allocate one 4096-byte page of physical memory.
2783 // Returns a pointer that the kernel can use.
2784 // Returns 0 if the memory cannot be allocated.
2785 char*
2786 kalloc(void)
2787 {
2788   struct run *r;
2789 
2790   if(kmem.use_lock)
2791     acquire(&kmem.lock);
2792   r = kmem.freelist;
2793   if(r)
2794     kmem.freelist = r->next;
2795   if(kmem.use_lock)
2796     release(&kmem.lock);
2797   return (char*)r;
2798 }
2799 
