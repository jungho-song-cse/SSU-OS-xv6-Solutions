3750 struct file {
3751   enum { FD_NONE, FD_PIPE, FD_INODE } type;
3752   int ref; // reference count
3753   char readable;
3754   char writable;
3755   struct pipe *pipe;
3756   struct inode *ip;
3757   uint off;
3758 };
3759 
3760 
3761 // in-memory copy of an inode
3762 struct inode {
3763   uint dev;           // Device number
3764   uint inum;          // Inode number
3765   int ref;            // Reference count
3766   struct sleeplock lock; // protects everything below here
3767   int valid;          // inode has been read from disk?
3768 
3769   short type;         // copy of disk inode
3770   short major;
3771   short minor;
3772   short nlink;
3773   uint size;
3774   uint addrs[NDIRECT+1];
3775 };
3776 
3777 // table mapping major device number to
3778 // device functions
3779 struct devsw {
3780   int (*read)(struct inode*, char*, int);
3781   int (*write)(struct inode*, char*, int);
3782 };
3783 
3784 extern struct devsw devsw[];
3785 
3786 #define CONSOLE 1
3787 
3788 
3789 
3790 
3791 
3792 
3793 
3794 
3795 
3796 
3797 
3798 
3799 
