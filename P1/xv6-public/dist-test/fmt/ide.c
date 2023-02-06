3800 // Simple PIO-based (non-DMA) IDE driver code.
3801 
3802 #include "types.h"
3803 #include "defs.h"
3804 #include "param.h"
3805 #include "memlayout.h"
3806 #include "mmu.h"
3807 #include "proc.h"
3808 #include "x86.h"
3809 #include "traps.h"
3810 #include "spinlock.h"
3811 #include "sleeplock.h"
3812 #include "fs.h"
3813 #include "buf.h"
3814 
3815 #define SECTOR_SIZE   512
3816 #define IDE_BSY       0x80
3817 #define IDE_DRDY      0x40
3818 #define IDE_DF        0x20
3819 #define IDE_ERR       0x01
3820 
3821 #define IDE_CMD_READ  0x20
3822 #define IDE_CMD_WRITE 0x30
3823 #define IDE_CMD_RDMUL 0xc4
3824 #define IDE_CMD_WRMUL 0xc5
3825 
3826 // idequeue points to the buf now being read/written to the disk.
3827 // idequeue->qnext points to the next buf to be processed.
3828 // You must hold idelock while manipulating queue.
3829 
3830 static struct spinlock idelock;
3831 static struct buf *idequeue;
3832 
3833 static int havedisk1;
3834 static void idestart(struct buf*);
3835 
3836 // Wait for IDE disk to become ready.
3837 static int
3838 idewait(int checkerr)
3839 {
3840   int r;
3841 
3842   while(((r = inb(0x1f7)) & (IDE_BSY|IDE_DRDY)) != IDE_DRDY)
3843     ;
3844   if(checkerr && (r & (IDE_DF|IDE_ERR)) != 0)
3845     return -1;
3846   return 0;
3847 }
3848 
3849 
3850 void
3851 ideinit(void)
3852 {
3853   int i;
3854 
3855   initlock(&idelock, "ide");
3856   ioapicenable(IRQ_IDE, ncpu - 1);
3857   idewait(0);
3858 
3859   // Check if disk 1 is present
3860   outb(0x1f6, 0xe0 | (1<<4));
3861   for(i=0; i<1000; i++){
3862     if(inb(0x1f7) != 0){
3863       havedisk1 = 1;
3864       break;
3865     }
3866   }
3867 
3868   // Switch back to disk 0.
3869   outb(0x1f6, 0xe0 | (0<<4));
3870 }
3871 
3872 // Start the request for b.  Caller must hold idelock.
3873 static void
3874 idestart(struct buf *b)
3875 {
3876   if(b == 0)
3877     panic("idestart");
3878   if(b->blockno >= FSSIZE)
3879     panic("incorrect blockno");
3880   int sector_per_block =  BSIZE/SECTOR_SIZE;
3881   int sector = b->blockno * sector_per_block;
3882   int read_cmd = (sector_per_block == 1) ? IDE_CMD_READ :  IDE_CMD_RDMUL;
3883   int write_cmd = (sector_per_block == 1) ? IDE_CMD_WRITE : IDE_CMD_WRMUL;
3884 
3885   if (sector_per_block > 7) panic("idestart");
3886 
3887   idewait(0);
3888   outb(0x3f6, 0);  // generate interrupt
3889   outb(0x1f2, sector_per_block);  // number of sectors
3890   outb(0x1f3, sector & 0xff);
3891   outb(0x1f4, (sector >> 8) & 0xff);
3892   outb(0x1f5, (sector >> 16) & 0xff);
3893   outb(0x1f6, 0xe0 | ((b->dev&1)<<4) | ((sector>>24)&0x0f));
3894   if(b->flags & B_DIRTY){
3895     outb(0x1f7, write_cmd);
3896     outsl(0x1f0, b->data, BSIZE/4);
3897   } else {
3898     outb(0x1f7, read_cmd);
3899   }
3900 }
3901 
3902 // Interrupt handler.
3903 void
3904 ideintr(void)
3905 {
3906   struct buf *b;
3907 
3908   // First queued buffer is the active request.
3909   acquire(&idelock);
3910 
3911   if((b = idequeue) == 0){
3912     release(&idelock);
3913     return;
3914   }
3915   idequeue = b->qnext;
3916 
3917   // Read data if needed.
3918   if(!(b->flags & B_DIRTY) && idewait(1) >= 0)
3919     insl(0x1f0, b->data, BSIZE/4);
3920 
3921   // Wake process waiting for this buf.
3922   b->flags |= B_VALID;
3923   b->flags &= ~B_DIRTY;
3924   wakeup(b);
3925 
3926   // Start disk on next buf in queue.
3927   if(idequeue != 0)
3928     idestart(idequeue);
3929 
3930   release(&idelock);
3931 }
3932 
3933 // Sync buf with disk.
3934 // If B_DIRTY is set, write buf to disk, clear B_DIRTY, set B_VALID.
3935 // Else if B_VALID is not set, read buf from disk, set B_VALID.
3936 void
3937 iderw(struct buf *b)
3938 {
3939   struct buf **pp;
3940 
3941   if(!holdingsleep(&b->lock))
3942     panic("iderw: buf not locked");
3943   if((b->flags & (B_VALID|B_DIRTY)) == B_VALID)
3944     panic("iderw: nothing to do");
3945   if(b->dev != 0 && !havedisk1)
3946     panic("iderw: ide disk 1 not present");
3947 
3948   acquire(&idelock);  //DOC:acquire-lock
3949 
3950   // Append b to idequeue.
3951   b->qnext = 0;
3952   for(pp=&idequeue; *pp; pp=&(*pp)->qnext)  //DOC:insert-queue
3953     ;
3954   *pp = b;
3955 
3956   // Start disk if necessary.
3957   if(idequeue == b)
3958     idestart(b);
3959 
3960   // Wait for request to finish.
3961   while((b->flags & (B_VALID|B_DIRTY)) != B_VALID){
3962     sleep(b, &idelock);
3963   }
3964 
3965 
3966   release(&idelock);
3967 }
3968 
3969 
3970 
3971 
3972 
3973 
3974 
3975 
3976 
3977 
3978 
3979 
3980 
3981 
3982 
3983 
3984 
3985 
3986 
3987 
3988 
3989 
3990 
3991 
3992 
3993 
3994 
3995 
3996 
3997 
3998 
3999 
