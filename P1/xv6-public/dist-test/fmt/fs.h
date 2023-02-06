3650 // On-disk file system format.
3651 // Both the kernel and user programs use this header file.
3652 
3653 
3654 #define ROOTINO 1  // root i-number
3655 #define BSIZE 512  // block size
3656 
3657 // Disk layout:
3658 // [ boot block | super block | log | inode blocks |
3659 //                                          free bit map | data blocks]
3660 //
3661 // mkfs computes the super block and builds an initial file system. The
3662 // super block describes the disk layout:
3663 struct superblock {
3664   uint size;         // Size of file system image (blocks)
3665   uint nblocks;      // Number of data blocks
3666   uint ninodes;      // Number of inodes.
3667   uint nlog;         // Number of log blocks
3668   uint logstart;     // Block number of first log block
3669   uint inodestart;   // Block number of first inode block
3670   uint bmapstart;    // Block number of first free map block
3671 };
3672 
3673 #define NDIRECT 12
3674 #define NINDIRECT (BSIZE / sizeof(uint))
3675 #define MAXFILE (NDIRECT + NINDIRECT)
3676 
3677 // On-disk inode structure
3678 struct dinode {
3679   short type;           // File type
3680   short major;          // Major device number (T_DEV only)
3681   short minor;          // Minor device number (T_DEV only)
3682   short nlink;          // Number of links to inode in file system
3683   uint size;            // Size of file (bytes)
3684   uint addrs[NDIRECT+1];   // Data block addresses
3685 };
3686 
3687 
3688 
3689 
3690 
3691 
3692 
3693 
3694 
3695 
3696 
3697 
3698 
3699 
3700 // Inodes per block.
3701 #define IPB           (BSIZE / sizeof(struct dinode))
3702 
3703 // Block containing inode i
3704 #define IBLOCK(i, sb)     ((i) / IPB + sb.inodestart)
3705 
3706 // Bitmap bits per block
3707 #define BPB           (BSIZE*8)
3708 
3709 // Block of free map containing bit for block b
3710 #define BBLOCK(b, sb) (b/BPB + sb.bmapstart)
3711 
3712 // Directory is a file containing a sequence of dirent structures.
3713 #define DIRSIZ 14
3714 
3715 struct dirent {
3716   ushort inum;
3717   char name[DIRSIZ];
3718 };
3719 
3720 
3721 
3722 
3723 
3724 
3725 
3726 
3727 
3728 
3729 
3730 
3731 
3732 
3733 
3734 
3735 
3736 
3737 
3738 
3739 
3740 
3741 
3742 
3743 
3744 
3745 
3746 
3747 
3748 
3749 
