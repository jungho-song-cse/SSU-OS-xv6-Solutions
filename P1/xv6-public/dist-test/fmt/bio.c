4000 // Buffer cache.
4001 //
4002 // The buffer cache is a linked list of buf structures holding
4003 // cached copies of disk block contents.  Caching disk blocks
4004 // in memory reduces the number of disk reads and also provides
4005 // a synchronization point for disk blocks used by multiple processes.
4006 //
4007 // Interface:
4008 // * To get a buffer for a particular disk block, call bread.
4009 // * After changing buffer data, call bwrite to write it to disk.
4010 // * When done with the buffer, call brelse.
4011 // * Do not use the buffer after calling brelse.
4012 // * Only one process at a time can use a buffer,
4013 //     so do not keep them longer than necessary.
4014 //
4015 // The implementation uses two state flags internally:
4016 // * B_VALID: the buffer data has been read from the disk.
4017 // * B_DIRTY: the buffer data has been modified
4018 //     and needs to be written to disk.
4019 
4020 #include "types.h"
4021 #include "defs.h"
4022 #include "param.h"
4023 #include "spinlock.h"
4024 #include "sleeplock.h"
4025 #include "fs.h"
4026 #include "buf.h"
4027 
4028 struct {
4029   struct spinlock lock;
4030   struct buf buf[NBUF];
4031 
4032   // Linked list of all buffers, through prev/next.
4033   // head.next is most recently used.
4034   struct buf head;
4035 } bcache;
4036 
4037 void
4038 binit(void)
4039 {
4040   struct buf *b;
4041 
4042   initlock(&bcache.lock, "bcache");
4043 
4044   // Create linked list of buffers
4045   bcache.head.prev = &bcache.head;
4046   bcache.head.next = &bcache.head;
4047   for(b = bcache.buf; b < bcache.buf+NBUF; b++){
4048     b->next = bcache.head.next;
4049     b->prev = &bcache.head;
4050     initsleeplock(&b->lock, "buffer");
4051     bcache.head.next->prev = b;
4052     bcache.head.next = b;
4053   }
4054 }
4055 
4056 // Look through buffer cache for block on device dev.
4057 // If not found, allocate a buffer.
4058 // In either case, return locked buffer.
4059 static struct buf*
4060 bget(uint dev, uint blockno)
4061 {
4062   struct buf *b;
4063 
4064   acquire(&bcache.lock);
4065 
4066   // Is the block already cached?
4067   for(b = bcache.head.next; b != &bcache.head; b = b->next){
4068     if(b->dev == dev && b->blockno == blockno){
4069       b->refcnt++;
4070       release(&bcache.lock);
4071       acquiresleep(&b->lock);
4072       return b;
4073     }
4074   }
4075 
4076   // Not cached; recycle an unused buffer.
4077   // Even if refcnt==0, B_DIRTY indicates a buffer is in use
4078   // because log.c has modified it but not yet committed it.
4079   for(b = bcache.head.prev; b != &bcache.head; b = b->prev){
4080     if(b->refcnt == 0 && (b->flags & B_DIRTY) == 0) {
4081       b->dev = dev;
4082       b->blockno = blockno;
4083       b->flags = 0;
4084       b->refcnt = 1;
4085       release(&bcache.lock);
4086       acquiresleep(&b->lock);
4087       return b;
4088     }
4089   }
4090   panic("bget: no buffers");
4091 }
4092 
4093 
4094 
4095 
4096 
4097 
4098 
4099 
4100 // Return a locked buf with the contents of the indicated block.
4101 struct buf*
4102 bread(uint dev, uint blockno)
4103 {
4104   struct buf *b;
4105 
4106   b = bget(dev, blockno);
4107   if((b->flags & B_VALID) == 0) {
4108     iderw(b);
4109   }
4110   return b;
4111 }
4112 
4113 // Write b's contents to disk.  Must be locked.
4114 void
4115 bwrite(struct buf *b)
4116 {
4117   if(!holdingsleep(&b->lock))
4118     panic("bwrite");
4119   b->flags |= B_DIRTY;
4120   iderw(b);
4121 }
4122 
4123 // Release a locked buffer.
4124 // Move to the head of the MRU list.
4125 void
4126 brelse(struct buf *b)
4127 {
4128   if(!holdingsleep(&b->lock))
4129     panic("brelse");
4130 
4131   releasesleep(&b->lock);
4132 
4133   acquire(&bcache.lock);
4134   b->refcnt--;
4135   if (b->refcnt == 0) {
4136     // no one is waiting for it.
4137     b->next->prev = b->prev;
4138     b->prev->next = b->next;
4139     b->next = bcache.head.next;
4140     b->prev = &bcache.head;
4141     bcache.head.next->prev = b;
4142     bcache.head.next = b;
4143   }
4144 
4145   release(&bcache.lock);
4146 }
4147 // Blank page.
4148 
4149 
