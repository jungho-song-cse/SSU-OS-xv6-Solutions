5450 //
5451 // File-system system calls.
5452 // Mostly argument checking, since we don't trust
5453 // user code, and calls into file.c and fs.c.
5454 //
5455 
5456 #include "types.h"
5457 #include "defs.h"
5458 #include "param.h"
5459 #include "stat.h"
5460 #include "mmu.h"
5461 #include "proc.h"
5462 #include "fs.h"
5463 #include "spinlock.h"
5464 #include "sleeplock.h"
5465 #include "file.h"
5466 #include "fcntl.h"
5467 
5468 // Fetch the nth word-sized system call argument as a file descriptor
5469 // and return both the descriptor and the corresponding struct file.
5470 static int
5471 argfd(int n, int *pfd, struct file **pf)
5472 {
5473   int fd;
5474   struct file *f;
5475 
5476   if(argint(n, &fd) < 0)
5477     return -1;
5478   if(fd < 0 || fd >= NOFILE || (f=myproc()->ofile[fd]) == 0)
5479     return -1;
5480   if(pfd)
5481     *pfd = fd;
5482   if(pf)
5483     *pf = f;
5484   return 0;
5485 }
5486 
5487 
5488 
5489 
5490 
5491 
5492 
5493 
5494 
5495 
5496 
5497 
5498 
5499 
5500 // Allocate a file descriptor for the given file.
5501 // Takes over file reference from caller on success.
5502 static int
5503 fdalloc(struct file *f)
5504 {
5505   int fd;
5506   struct proc *curproc = myproc();
5507 
5508   for(fd = 0; fd < NOFILE; fd++){
5509     if(curproc->ofile[fd] == 0){
5510       curproc->ofile[fd] = f;
5511       return fd;
5512     }
5513   }
5514   return -1;
5515 }
5516 
5517 int
5518 sys_dup(void)
5519 {
5520   struct file *f;
5521   int fd;
5522 
5523   if(argfd(0, 0, &f) < 0)
5524     return -1;
5525   if((fd=fdalloc(f)) < 0)
5526     return -1;
5527   filedup(f);
5528   return fd;
5529 }
5530 
5531 int
5532 sys_read(void)
5533 {
5534   struct file *f;
5535   int n;
5536   char *p;
5537 
5538   if(argfd(0, 0, &f) < 0 || argint(2, &n) < 0 || argptr(1, &p, n) < 0)
5539     return -1;
5540   return fileread(f, p, n);
5541 }
5542 
5543 
5544 
5545 
5546 
5547 
5548 
5549 
5550 int
5551 sys_write(void)
5552 {
5553   struct file *f;
5554   int n;
5555   char *p;
5556 
5557   if(argfd(0, 0, &f) < 0 || argint(2, &n) < 0 || argptr(1, &p, n) < 0)
5558     return -1;
5559   return filewrite(f, p, n);
5560 }
5561 
5562 int
5563 sys_close(void)
5564 {
5565   int fd;
5566   struct file *f;
5567 
5568   if(argfd(0, &fd, &f) < 0)
5569     return -1;
5570   myproc()->ofile[fd] = 0;
5571   fileclose(f);
5572   return 0;
5573 }
5574 
5575 int
5576 sys_fstat(void)
5577 {
5578   struct file *f;
5579   struct stat *st;
5580 
5581   if(argfd(0, 0, &f) < 0 || argptr(1, (void*)&st, sizeof(*st)) < 0)
5582     return -1;
5583   return filestat(f, st);
5584 }
5585 
5586 
5587 
5588 
5589 
5590 
5591 
5592 
5593 
5594 
5595 
5596 
5597 
5598 
5599 
5600 // Create the path new as a link to the same inode as old.
5601 int
5602 sys_link(void)
5603 {
5604   char name[DIRSIZ], *new, *old;
5605   struct inode *dp, *ip;
5606 
5607   if(argstr(0, &old) < 0 || argstr(1, &new) < 0)
5608     return -1;
5609 
5610   begin_op();
5611   if((ip = namei(old)) == 0){
5612     end_op();
5613     return -1;
5614   }
5615 
5616   ilock(ip);
5617   if(ip->type == T_DIR){
5618     iunlockput(ip);
5619     end_op();
5620     return -1;
5621   }
5622 
5623   ip->nlink++;
5624   iupdate(ip);
5625   iunlock(ip);
5626 
5627   if((dp = nameiparent(new, name)) == 0)
5628     goto bad;
5629   ilock(dp);
5630   if(dp->dev != ip->dev || dirlink(dp, name, ip->inum) < 0){
5631     iunlockput(dp);
5632     goto bad;
5633   }
5634   iunlockput(dp);
5635   iput(ip);
5636 
5637   end_op();
5638 
5639   return 0;
5640 
5641 bad:
5642   ilock(ip);
5643   ip->nlink--;
5644   iupdate(ip);
5645   iunlockput(ip);
5646   end_op();
5647   return -1;
5648 }
5649 
5650 // Is the directory dp empty except for "." and ".." ?
5651 static int
5652 isdirempty(struct inode *dp)
5653 {
5654   int off;
5655   struct dirent de;
5656 
5657   for(off=2*sizeof(de); off<dp->size; off+=sizeof(de)){
5658     if(readi(dp, (char*)&de, off, sizeof(de)) != sizeof(de))
5659       panic("isdirempty: readi");
5660     if(de.inum != 0)
5661       return 0;
5662   }
5663   return 1;
5664 }
5665 
5666 int
5667 sys_unlink(void)
5668 {
5669   struct inode *ip, *dp;
5670   struct dirent de;
5671   char name[DIRSIZ], *path;
5672   uint off;
5673 
5674   if(argstr(0, &path) < 0)
5675     return -1;
5676 
5677   begin_op();
5678   if((dp = nameiparent(path, name)) == 0){
5679     end_op();
5680     return -1;
5681   }
5682 
5683   ilock(dp);
5684 
5685   // Cannot unlink "." or "..".
5686   if(namecmp(name, ".") == 0 || namecmp(name, "..") == 0)
5687     goto bad;
5688 
5689   if((ip = dirlookup(dp, name, &off)) == 0)
5690     goto bad;
5691   ilock(ip);
5692 
5693   if(ip->nlink < 1)
5694     panic("unlink: nlink < 1");
5695   if(ip->type == T_DIR && !isdirempty(ip)){
5696     iunlockput(ip);
5697     goto bad;
5698   }
5699 
5700   memset(&de, 0, sizeof(de));
5701   if(writei(dp, (char*)&de, off, sizeof(de)) != sizeof(de))
5702     panic("unlink: writei");
5703   if(ip->type == T_DIR){
5704     dp->nlink--;
5705     iupdate(dp);
5706   }
5707   iunlockput(dp);
5708 
5709   ip->nlink--;
5710   iupdate(ip);
5711   iunlockput(ip);
5712 
5713   end_op();
5714 
5715   return 0;
5716 
5717 bad:
5718   iunlockput(dp);
5719   end_op();
5720   return -1;
5721 }
5722 
5723 static struct inode*
5724 create(char *path, short type, short major, short minor)
5725 {
5726   struct inode *ip, *dp;
5727   char name[DIRSIZ];
5728 
5729   if((dp = nameiparent(path, name)) == 0)
5730     return 0;
5731   ilock(dp);
5732 
5733   if((ip = dirlookup(dp, name, 0)) != 0){
5734     iunlockput(dp);
5735     ilock(ip);
5736     if(type == T_FILE && ip->type == T_FILE)
5737       return ip;
5738     iunlockput(ip);
5739     return 0;
5740   }
5741 
5742   if((ip = ialloc(dp->dev, type)) == 0)
5743     panic("create: ialloc");
5744 
5745   ilock(ip);
5746   ip->major = major;
5747   ip->minor = minor;
5748   ip->nlink = 1;
5749   iupdate(ip);
5750   if(type == T_DIR){  // Create . and .. entries.
5751     dp->nlink++;  // for ".."
5752     iupdate(dp);
5753     // No ip->nlink++ for ".": avoid cyclic ref count.
5754     if(dirlink(ip, ".", ip->inum) < 0 || dirlink(ip, "..", dp->inum) < 0)
5755       panic("create dots");
5756   }
5757 
5758   if(dirlink(dp, name, ip->inum) < 0)
5759     panic("create: dirlink");
5760 
5761   iunlockput(dp);
5762 
5763   return ip;
5764 }
5765 
5766 int
5767 sys_open(void)
5768 {
5769   char *path;
5770   int fd, omode;
5771   struct file *f;
5772   struct inode *ip;
5773 
5774   if(argstr(0, &path) < 0 || argint(1, &omode) < 0)
5775     return -1;
5776 
5777   begin_op();
5778 
5779   if(omode & O_CREATE){
5780     ip = create(path, T_FILE, 0, 0);
5781     if(ip == 0){
5782       end_op();
5783       return -1;
5784     }
5785   } else {
5786     if((ip = namei(path)) == 0){
5787       end_op();
5788       return -1;
5789     }
5790     ilock(ip);
5791     if(ip->type == T_DIR && omode != O_RDONLY){
5792       iunlockput(ip);
5793       end_op();
5794       return -1;
5795     }
5796   }
5797 
5798 
5799 
5800   if((f = filealloc()) == 0 || (fd = fdalloc(f)) < 0){
5801     if(f)
5802       fileclose(f);
5803     iunlockput(ip);
5804     end_op();
5805     return -1;
5806   }
5807   iunlock(ip);
5808   end_op();
5809 
5810   f->type = FD_INODE;
5811   f->ip = ip;
5812   f->off = 0;
5813   f->readable = !(omode & O_WRONLY);
5814   f->writable = (omode & O_WRONLY) || (omode & O_RDWR);
5815   return fd;
5816 }
5817 
5818 int
5819 sys_mkdir(void)
5820 {
5821   char *path;
5822   struct inode *ip;
5823 
5824   begin_op();
5825   if(argstr(0, &path) < 0 || (ip = create(path, T_DIR, 0, 0)) == 0){
5826     end_op();
5827     return -1;
5828   }
5829   iunlockput(ip);
5830   end_op();
5831   return 0;
5832 }
5833 
5834 int
5835 sys_mknod(void)
5836 {
5837   struct inode *ip;
5838   char *path;
5839   int major, minor;
5840 
5841   begin_op();
5842   if((argstr(0, &path)) < 0 ||
5843      argint(1, &major) < 0 ||
5844      argint(2, &minor) < 0 ||
5845      (ip = create(path, T_DEV, major, minor)) == 0){
5846     end_op();
5847     return -1;
5848   }
5849   iunlockput(ip);
5850   end_op();
5851   return 0;
5852 }
5853 
5854 int
5855 sys_chdir(void)
5856 {
5857   char *path;
5858   struct inode *ip;
5859   struct proc *curproc = myproc();
5860 
5861   begin_op();
5862   if(argstr(0, &path) < 0 || (ip = namei(path)) == 0){
5863     end_op();
5864     return -1;
5865   }
5866   ilock(ip);
5867   if(ip->type != T_DIR){
5868     iunlockput(ip);
5869     end_op();
5870     return -1;
5871   }
5872   iunlock(ip);
5873   iput(curproc->cwd);
5874   end_op();
5875   curproc->cwd = ip;
5876   return 0;
5877 }
5878 
5879 int
5880 sys_exec(void)
5881 {
5882   char *path, *argv[MAXARG];
5883   int i;
5884   uint uargv, uarg;
5885 
5886   if(argstr(0, &path) < 0 || argint(1, (int*)&uargv) < 0){
5887     return -1;
5888   }
5889   memset(argv, 0, sizeof(argv));
5890   for(i=0;; i++){
5891     if(i >= NELEM(argv))
5892       return -1;
5893     if(fetchint(uargv+4*i, (int*)&uarg) < 0)
5894       return -1;
5895     if(uarg == 0){
5896       argv[i] = 0;
5897       break;
5898     }
5899     if(fetchstr(uarg, &argv[i]) < 0)
5900       return -1;
5901   }
5902   return exec(path, argv);
5903 }
5904 
5905 int
5906 sys_pipe(void)
5907 {
5908   int *fd;
5909   struct file *rf, *wf;
5910   int fd0, fd1;
5911 
5912   if(argptr(0, (void*)&fd, 2*sizeof(fd[0])) < 0)
5913     return -1;
5914   if(pipealloc(&rf, &wf) < 0)
5915     return -1;
5916   fd0 = -1;
5917   if((fd0 = fdalloc(rf)) < 0 || (fd1 = fdalloc(wf)) < 0){
5918     if(fd0 >= 0)
5919       myproc()->ofile[fd0] = 0;
5920     fileclose(rf);
5921     fileclose(wf);
5922     return -1;
5923   }
5924   fd[0] = fd0;
5925   fd[1] = fd1;
5926   return 0;
5927 }
5928 
5929 
5930 
5931 
5932 
5933 
5934 
5935 
5936 
5937 
5938 
5939 
5940 
5941 
5942 
5943 
5944 
5945 
5946 
5947 
5948 
5949 
