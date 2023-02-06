4500 // File system implementation.  Five layers:
4501 //   + Blocks: allocator for raw disk blocks.
4502 //   + Log: crash recovery for multi-step updates.
4503 //   + Files: inode allocator, reading, writing, metadata.
4504 //   + Directories: inode with special contents (list of other inodes!)
4505 //   + Names: paths like /usr/rtm/xv6/fs.c for convenient naming.
4506 //
4507 // This file contains the low-level file system manipulation
4508 // routines.  The (higher-level) system call implementations
4509 // are in sysfile.c.
4510 
4511 #include "types.h"
4512 #include "defs.h"
4513 #include "param.h"
4514 #include "stat.h"
4515 #include "mmu.h"
4516 #include "proc.h"
4517 #include "spinlock.h"
4518 #include "sleeplock.h"
4519 #include "fs.h"
4520 #include "buf.h"
4521 #include "file.h"
4522 
4523 #define min(a, b) ((a) < (b) ? (a) : (b))
4524 static void itrunc(struct inode*);
4525 // there should be one superblock per disk device, but we run with
4526 // only one device
4527 struct superblock sb;
4528 
4529 // Read the super block.
4530 void
4531 readsb(int dev, struct superblock *sb)
4532 {
4533   struct buf *bp;
4534 
4535   bp = bread(dev, 1);
4536   memmove(sb, bp->data, sizeof(*sb));
4537   brelse(bp);
4538 }
4539 
4540 
4541 
4542 
4543 
4544 
4545 
4546 
4547 
4548 
4549 
4550 // Zero a block.
4551 static void
4552 bzero(int dev, int bno)
4553 {
4554   struct buf *bp;
4555 
4556   bp = bread(dev, bno);
4557   memset(bp->data, 0, BSIZE);
4558   log_write(bp);
4559   brelse(bp);
4560 }
4561 
4562 // Blocks.
4563 
4564 // Allocate a zeroed disk block.
4565 static uint
4566 balloc(uint dev)
4567 {
4568   int b, bi, m;
4569   struct buf *bp;
4570 
4571   bp = 0;
4572   for(b = 0; b < sb.size; b += BPB){
4573     bp = bread(dev, BBLOCK(b, sb));
4574     for(bi = 0; bi < BPB && b + bi < sb.size; bi++){
4575       m = 1 << (bi % 8);
4576       if((bp->data[bi/8] & m) == 0){  // Is block free?
4577         bp->data[bi/8] |= m;  // Mark block in use.
4578         log_write(bp);
4579         brelse(bp);
4580         bzero(dev, b + bi);
4581         return b + bi;
4582       }
4583     }
4584     brelse(bp);
4585   }
4586   panic("balloc: out of blocks");
4587 }
4588 
4589 
4590 
4591 
4592 
4593 
4594 
4595 
4596 
4597 
4598 
4599 
4600 // Free a disk block.
4601 static void
4602 bfree(int dev, uint b)
4603 {
4604   struct buf *bp;
4605   int bi, m;
4606 
4607   bp = bread(dev, BBLOCK(b, sb));
4608   bi = b % BPB;
4609   m = 1 << (bi % 8);
4610   if((bp->data[bi/8] & m) == 0)
4611     panic("freeing free block");
4612   bp->data[bi/8] &= ~m;
4613   log_write(bp);
4614   brelse(bp);
4615 }
4616 
4617 // Inodes.
4618 //
4619 // An inode describes a single unnamed file.
4620 // The inode disk structure holds metadata: the file's type,
4621 // its size, the number of links referring to it, and the
4622 // list of blocks holding the file's content.
4623 //
4624 // The inodes are laid out sequentially on disk at
4625 // sb.startinode. Each inode has a number, indicating its
4626 // position on the disk.
4627 //
4628 // The kernel keeps a cache of in-use inodes in memory
4629 // to provide a place for synchronizing access
4630 // to inodes used by multiple processes. The cached
4631 // inodes include book-keeping information that is
4632 // not stored on disk: ip->ref and ip->valid.
4633 //
4634 // An inode and its in-memory representation go through a
4635 // sequence of states before they can be used by the
4636 // rest of the file system code.
4637 //
4638 // * Allocation: an inode is allocated if its type (on disk)
4639 //   is non-zero. ialloc() allocates, and iput() frees if
4640 //   the reference and link counts have fallen to zero.
4641 //
4642 // * Referencing in cache: an entry in the inode cache
4643 //   is free if ip->ref is zero. Otherwise ip->ref tracks
4644 //   the number of in-memory pointers to the entry (open
4645 //   files and current directories). iget() finds or
4646 //   creates a cache entry and increments its ref; iput()
4647 //   decrements ref.
4648 //
4649 // * Valid: the information (type, size, &c) in an inode
4650 //   cache entry is only correct when ip->valid is 1.
4651 //   ilock() reads the inode from
4652 //   the disk and sets ip->valid, while iput() clears
4653 //   ip->valid if ip->ref has fallen to zero.
4654 //
4655 // * Locked: file system code may only examine and modify
4656 //   the information in an inode and its content if it
4657 //   has first locked the inode.
4658 //
4659 // Thus a typical sequence is:
4660 //   ip = iget(dev, inum)
4661 //   ilock(ip)
4662 //   ... examine and modify ip->xxx ...
4663 //   iunlock(ip)
4664 //   iput(ip)
4665 //
4666 // ilock() is separate from iget() so that system calls can
4667 // get a long-term reference to an inode (as for an open file)
4668 // and only lock it for short periods (e.g., in read()).
4669 // The separation also helps avoid deadlock and races during
4670 // pathname lookup. iget() increments ip->ref so that the inode
4671 // stays cached and pointers to it remain valid.
4672 //
4673 // Many internal file system functions expect the caller to
4674 // have locked the inodes involved; this lets callers create
4675 // multi-step atomic operations.
4676 //
4677 // The icache.lock spin-lock protects the allocation of icache
4678 // entries. Since ip->ref indicates whether an entry is free,
4679 // and ip->dev and ip->inum indicate which i-node an entry
4680 // holds, one must hold icache.lock while using any of those fields.
4681 //
4682 // An ip->lock sleep-lock protects all ip-> fields other than ref,
4683 // dev, and inum.  One must hold ip->lock in order to
4684 // read or write that inode's ip->valid, ip->size, ip->type, &c.
4685 
4686 struct {
4687   struct spinlock lock;
4688   struct inode inode[NINODE];
4689 } icache;
4690 
4691 void
4692 iinit(int dev)
4693 {
4694   int i = 0;
4695 
4696   initlock(&icache.lock, "icache");
4697   for(i = 0; i < NINODE; i++) {
4698     initsleeplock(&icache.inode[i].lock, "inode");
4699   }
4700   readsb(dev, &sb);
4701   cprintf("sb: size %d nblocks %d ninodes %d nlog %d logstart %d\
4702  inodestart %d bmap start %d\n", sb.size, sb.nblocks,
4703           sb.ninodes, sb.nlog, sb.logstart, sb.inodestart,
4704           sb.bmapstart);
4705 }
4706 
4707 static struct inode* iget(uint dev, uint inum);
4708 
4709 // Allocate an inode on device dev.
4710 // Mark it as allocated by  giving it type type.
4711 // Returns an unlocked but allocated and referenced inode.
4712 struct inode*
4713 ialloc(uint dev, short type)
4714 {
4715   int inum;
4716   struct buf *bp;
4717   struct dinode *dip;
4718 
4719   for(inum = 1; inum < sb.ninodes; inum++){
4720     bp = bread(dev, IBLOCK(inum, sb));
4721     dip = (struct dinode*)bp->data + inum%IPB;
4722     if(dip->type == 0){  // a free inode
4723       memset(dip, 0, sizeof(*dip));
4724       dip->type = type;
4725       log_write(bp);   // mark it allocated on the disk
4726       brelse(bp);
4727       return iget(dev, inum);
4728     }
4729     brelse(bp);
4730   }
4731   panic("ialloc: no inodes");
4732 }
4733 
4734 // Copy a modified in-memory inode to disk.
4735 // Must be called after every change to an ip->xxx field
4736 // that lives on disk, since i-node cache is write-through.
4737 // Caller must hold ip->lock.
4738 void
4739 iupdate(struct inode *ip)
4740 {
4741   struct buf *bp;
4742   struct dinode *dip;
4743 
4744   bp = bread(ip->dev, IBLOCK(ip->inum, sb));
4745   dip = (struct dinode*)bp->data + ip->inum%IPB;
4746   dip->type = ip->type;
4747   dip->major = ip->major;
4748   dip->minor = ip->minor;
4749   dip->nlink = ip->nlink;
4750   dip->size = ip->size;
4751   memmove(dip->addrs, ip->addrs, sizeof(ip->addrs));
4752   log_write(bp);
4753   brelse(bp);
4754 }
4755 
4756 // Find the inode with number inum on device dev
4757 // and return the in-memory copy. Does not lock
4758 // the inode and does not read it from disk.
4759 static struct inode*
4760 iget(uint dev, uint inum)
4761 {
4762   struct inode *ip, *empty;
4763 
4764   acquire(&icache.lock);
4765 
4766   // Is the inode already cached?
4767   empty = 0;
4768   for(ip = &icache.inode[0]; ip < &icache.inode[NINODE]; ip++){
4769     if(ip->ref > 0 && ip->dev == dev && ip->inum == inum){
4770       ip->ref++;
4771       release(&icache.lock);
4772       return ip;
4773     }
4774     if(empty == 0 && ip->ref == 0)    // Remember empty slot.
4775       empty = ip;
4776   }
4777 
4778   // Recycle an inode cache entry.
4779   if(empty == 0)
4780     panic("iget: no inodes");
4781 
4782   ip = empty;
4783   ip->dev = dev;
4784   ip->inum = inum;
4785   ip->ref = 1;
4786   ip->valid = 0;
4787   release(&icache.lock);
4788 
4789   return ip;
4790 }
4791 
4792 
4793 
4794 
4795 
4796 
4797 
4798 
4799 
4800 // Increment reference count for ip.
4801 // Returns ip to enable ip = idup(ip1) idiom.
4802 struct inode*
4803 idup(struct inode *ip)
4804 {
4805   acquire(&icache.lock);
4806   ip->ref++;
4807   release(&icache.lock);
4808   return ip;
4809 }
4810 
4811 // Lock the given inode.
4812 // Reads the inode from disk if necessary.
4813 void
4814 ilock(struct inode *ip)
4815 {
4816   struct buf *bp;
4817   struct dinode *dip;
4818 
4819   if(ip == 0 || ip->ref < 1)
4820     panic("ilock");
4821 
4822   acquiresleep(&ip->lock);
4823 
4824   if(ip->valid == 0){
4825     bp = bread(ip->dev, IBLOCK(ip->inum, sb));
4826     dip = (struct dinode*)bp->data + ip->inum%IPB;
4827     ip->type = dip->type;
4828     ip->major = dip->major;
4829     ip->minor = dip->minor;
4830     ip->nlink = dip->nlink;
4831     ip->size = dip->size;
4832     memmove(ip->addrs, dip->addrs, sizeof(ip->addrs));
4833     brelse(bp);
4834     ip->valid = 1;
4835     if(ip->type == 0)
4836       panic("ilock: no type");
4837   }
4838 }
4839 
4840 // Unlock the given inode.
4841 void
4842 iunlock(struct inode *ip)
4843 {
4844   if(ip == 0 || !holdingsleep(&ip->lock) || ip->ref < 1)
4845     panic("iunlock");
4846 
4847   releasesleep(&ip->lock);
4848 }
4849 
4850 // Drop a reference to an in-memory inode.
4851 // If that was the last reference, the inode cache entry can
4852 // be recycled.
4853 // If that was the last reference and the inode has no links
4854 // to it, free the inode (and its content) on disk.
4855 // All calls to iput() must be inside a transaction in
4856 // case it has to free the inode.
4857 void
4858 iput(struct inode *ip)
4859 {
4860   acquiresleep(&ip->lock);
4861   if(ip->valid && ip->nlink == 0){
4862     acquire(&icache.lock);
4863     int r = ip->ref;
4864     release(&icache.lock);
4865     if(r == 1){
4866       // inode has no links and no other references: truncate and free.
4867       itrunc(ip);
4868       ip->type = 0;
4869       iupdate(ip);
4870       ip->valid = 0;
4871     }
4872   }
4873   releasesleep(&ip->lock);
4874 
4875   acquire(&icache.lock);
4876   ip->ref--;
4877   release(&icache.lock);
4878 }
4879 
4880 // Common idiom: unlock, then put.
4881 void
4882 iunlockput(struct inode *ip)
4883 {
4884   iunlock(ip);
4885   iput(ip);
4886 }
4887 
4888 
4889 
4890 
4891 
4892 
4893 
4894 
4895 
4896 
4897 
4898 
4899 
4900 // Inode content
4901 //
4902 // The content (data) associated with each inode is stored
4903 // in blocks on the disk. The first NDIRECT block numbers
4904 // are listed in ip->addrs[].  The next NINDIRECT blocks are
4905 // listed in block ip->addrs[NDIRECT].
4906 
4907 // Return the disk block address of the nth block in inode ip.
4908 // If there is no such block, bmap allocates one.
4909 static uint
4910 bmap(struct inode *ip, uint bn)
4911 {
4912   uint addr, *a;
4913   struct buf *bp;
4914 
4915   if(bn < NDIRECT){
4916     if((addr = ip->addrs[bn]) == 0)
4917       ip->addrs[bn] = addr = balloc(ip->dev);
4918     return addr;
4919   }
4920   bn -= NDIRECT;
4921 
4922   if(bn < NINDIRECT){
4923     // Load indirect block, allocating if necessary.
4924     if((addr = ip->addrs[NDIRECT]) == 0)
4925       ip->addrs[NDIRECT] = addr = balloc(ip->dev);
4926     bp = bread(ip->dev, addr);
4927     a = (uint*)bp->data;
4928     if((addr = a[bn]) == 0){
4929       a[bn] = addr = balloc(ip->dev);
4930       log_write(bp);
4931     }
4932     brelse(bp);
4933     return addr;
4934   }
4935 
4936   panic("bmap: out of range");
4937 }
4938 
4939 
4940 
4941 
4942 
4943 
4944 
4945 
4946 
4947 
4948 
4949 
4950 // Truncate inode (discard contents).
4951 // Only called when the inode has no links
4952 // to it (no directory entries referring to it)
4953 // and has no in-memory reference to it (is
4954 // not an open file or current directory).
4955 static void
4956 itrunc(struct inode *ip)
4957 {
4958   int i, j;
4959   struct buf *bp;
4960   uint *a;
4961 
4962   for(i = 0; i < NDIRECT; i++){
4963     if(ip->addrs[i]){
4964       bfree(ip->dev, ip->addrs[i]);
4965       ip->addrs[i] = 0;
4966     }
4967   }
4968 
4969   if(ip->addrs[NDIRECT]){
4970     bp = bread(ip->dev, ip->addrs[NDIRECT]);
4971     a = (uint*)bp->data;
4972     for(j = 0; j < NINDIRECT; j++){
4973       if(a[j])
4974         bfree(ip->dev, a[j]);
4975     }
4976     brelse(bp);
4977     bfree(ip->dev, ip->addrs[NDIRECT]);
4978     ip->addrs[NDIRECT] = 0;
4979   }
4980 
4981   ip->size = 0;
4982   iupdate(ip);
4983 }
4984 
4985 // Copy stat information from inode.
4986 // Caller must hold ip->lock.
4987 void
4988 stati(struct inode *ip, struct stat *st)
4989 {
4990   st->dev = ip->dev;
4991   st->ino = ip->inum;
4992   st->type = ip->type;
4993   st->nlink = ip->nlink;
4994   st->size = ip->size;
4995 }
4996 
4997 
4998 
4999 
5000 // Read data from inode.
5001 // Caller must hold ip->lock.
5002 int
5003 readi(struct inode *ip, char *dst, uint off, uint n)
5004 {
5005   uint tot, m;
5006   struct buf *bp;
5007 
5008   if(ip->type == T_DEV){
5009     if(ip->major < 0 || ip->major >= NDEV || !devsw[ip->major].read)
5010       return -1;
5011     return devsw[ip->major].read(ip, dst, n);
5012   }
5013 
5014   if(off > ip->size || off + n < off)
5015     return -1;
5016   if(off + n > ip->size)
5017     n = ip->size - off;
5018 
5019   for(tot=0; tot<n; tot+=m, off+=m, dst+=m){
5020     bp = bread(ip->dev, bmap(ip, off/BSIZE));
5021     m = min(n - tot, BSIZE - off%BSIZE);
5022     memmove(dst, bp->data + off%BSIZE, m);
5023     brelse(bp);
5024   }
5025   return n;
5026 }
5027 
5028 // Write data to inode.
5029 // Caller must hold ip->lock.
5030 int
5031 writei(struct inode *ip, char *src, uint off, uint n)
5032 {
5033   uint tot, m;
5034   struct buf *bp;
5035 
5036   if(ip->type == T_DEV){
5037     if(ip->major < 0 || ip->major >= NDEV || !devsw[ip->major].write)
5038       return -1;
5039     return devsw[ip->major].write(ip, src, n);
5040   }
5041 
5042   if(off > ip->size || off + n < off)
5043     return -1;
5044   if(off + n > MAXFILE*BSIZE)
5045     return -1;
5046 
5047 
5048 
5049 
5050   for(tot=0; tot<n; tot+=m, off+=m, src+=m){
5051     bp = bread(ip->dev, bmap(ip, off/BSIZE));
5052     m = min(n - tot, BSIZE - off%BSIZE);
5053     memmove(bp->data + off%BSIZE, src, m);
5054     log_write(bp);
5055     brelse(bp);
5056   }
5057 
5058   if(n > 0 && off > ip->size){
5059     ip->size = off;
5060     iupdate(ip);
5061   }
5062   return n;
5063 }
5064 
5065 // Directories
5066 
5067 int
5068 namecmp(const char *s, const char *t)
5069 {
5070   return strncmp(s, t, DIRSIZ);
5071 }
5072 
5073 // Look for a directory entry in a directory.
5074 // If found, set *poff to byte offset of entry.
5075 struct inode*
5076 dirlookup(struct inode *dp, char *name, uint *poff)
5077 {
5078   uint off, inum;
5079   struct dirent de;
5080 
5081   if(dp->type != T_DIR)
5082     panic("dirlookup not DIR");
5083 
5084   for(off = 0; off < dp->size; off += sizeof(de)){
5085     if(readi(dp, (char*)&de, off, sizeof(de)) != sizeof(de))
5086       panic("dirlookup read");
5087     if(de.inum == 0)
5088       continue;
5089     if(namecmp(name, de.name) == 0){
5090       // entry matches path element
5091       if(poff)
5092         *poff = off;
5093       inum = de.inum;
5094       return iget(dp->dev, inum);
5095     }
5096   }
5097 
5098   return 0;
5099 }
5100 // Write a new directory entry (name, inum) into the directory dp.
5101 int
5102 dirlink(struct inode *dp, char *name, uint inum)
5103 {
5104   int off;
5105   struct dirent de;
5106   struct inode *ip;
5107 
5108   // Check that name is not present.
5109   if((ip = dirlookup(dp, name, 0)) != 0){
5110     iput(ip);
5111     return -1;
5112   }
5113 
5114   // Look for an empty dirent.
5115   for(off = 0; off < dp->size; off += sizeof(de)){
5116     if(readi(dp, (char*)&de, off, sizeof(de)) != sizeof(de))
5117       panic("dirlink read");
5118     if(de.inum == 0)
5119       break;
5120   }
5121 
5122   strncpy(de.name, name, DIRSIZ);
5123   de.inum = inum;
5124   if(writei(dp, (char*)&de, off, sizeof(de)) != sizeof(de))
5125     panic("dirlink");
5126 
5127   return 0;
5128 }
5129 
5130 // Paths
5131 
5132 // Copy the next path element from path into name.
5133 // Return a pointer to the element following the copied one.
5134 // The returned path has no leading slashes,
5135 // so the caller can check *path=='\0' to see if the name is the last one.
5136 // If no name to remove, return 0.
5137 //
5138 // Examples:
5139 //   skipelem("a/bb/c", name) = "bb/c", setting name = "a"
5140 //   skipelem("///a//bb", name) = "bb", setting name = "a"
5141 //   skipelem("a", name) = "", setting name = "a"
5142 //   skipelem("", name) = skipelem("////", name) = 0
5143 //
5144 static char*
5145 skipelem(char *path, char *name)
5146 {
5147   char *s;
5148   int len;
5149 
5150   while(*path == '/')
5151     path++;
5152   if(*path == 0)
5153     return 0;
5154   s = path;
5155   while(*path != '/' && *path != 0)
5156     path++;
5157   len = path - s;
5158   if(len >= DIRSIZ)
5159     memmove(name, s, DIRSIZ);
5160   else {
5161     memmove(name, s, len);
5162     name[len] = 0;
5163   }
5164   while(*path == '/')
5165     path++;
5166   return path;
5167 }
5168 
5169 // Look up and return the inode for a path name.
5170 // If parent != 0, return the inode for the parent and copy the final
5171 // path element into name, which must have room for DIRSIZ bytes.
5172 // Must be called inside a transaction since it calls iput().
5173 static struct inode*
5174 namex(char *path, int nameiparent, char *name)
5175 {
5176   struct inode *ip, *next;
5177 
5178   if(*path == '/')
5179     ip = iget(ROOTDEV, ROOTINO);
5180   else
5181     ip = idup(myproc()->cwd);
5182 
5183   while((path = skipelem(path, name)) != 0){
5184     ilock(ip);
5185     if(ip->type != T_DIR){
5186       iunlockput(ip);
5187       return 0;
5188     }
5189     if(nameiparent && *path == '\0'){
5190       // Stop one level early.
5191       iunlock(ip);
5192       return ip;
5193     }
5194     if((next = dirlookup(ip, name, 0)) == 0){
5195       iunlockput(ip);
5196       return 0;
5197     }
5198     iunlockput(ip);
5199     ip = next;
5200   }
5201   if(nameiparent){
5202     iput(ip);
5203     return 0;
5204   }
5205   return ip;
5206 }
5207 
5208 struct inode*
5209 namei(char *path)
5210 {
5211   char name[DIRSIZ];
5212   return namex(path, 0, name);
5213 }
5214 
5215 struct inode*
5216 nameiparent(char *path, char *name)
5217 {
5218   return namex(path, 1, name);
5219 }
5220 
5221 
5222 
5223 
5224 
5225 
5226 
5227 
5228 
5229 
5230 
5231 
5232 
5233 
5234 
5235 
5236 
5237 
5238 
5239 
5240 
5241 
5242 
5243 
5244 
5245 
5246 
5247 
5248 
5249 
