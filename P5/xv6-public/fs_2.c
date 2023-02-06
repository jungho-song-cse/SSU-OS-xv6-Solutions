// File system implementation.  Five layers:
//   + Blocks: allocator for raw disk blocks.
//   + Log: crash recovery for multi-step updates.
//   + Files: inode allocator, reading, writing, metadata.
//   + Directories: inode with special contents (list of other inodes!)
//   + Names: paths like /usr/rtm/xv6/fs.c for convenient naming.
//
// This file contains the low-level file system manipulation
// routines.  The (higher-level) system call implementations
// are in sysfile.c.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "stat.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "buf.h"
#include "file.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
static void itrunc(struct inode*);
// there should be one superblock per disk device, but we run with
// only one device
struct superblock sb; 

// Zero a block.
static void
bzero(int dev, int bno)
{
  struct buf *bp;

  bp = bread(dev, bno);
  memset(bp->data, 0, BSIZE);
  log_write(bp);
  brelse(bp);
}

// Blocks.

// Allocate a zeroed disk block.
static uint
balloc(uint dev)
{
  int b, bi, m;
  struct buf *bp;

  bp = 0;
  for(b = 0; b < sb.size; b += BPB){
    bp = bread(dev, BBLOCK(b, sb));
    for(bi = 0; bi < BPB && b + bi < sb.size; bi++){
      m = 1 << (bi % 8);
      if((bp->data[bi/8] & m) == 0){  // Is block free?
        bp->data[bi/8] |= m;  // Mark block in use.
        log_write(bp);
        brelse(bp);
        bzero(dev, b + bi);
        return b + bi;
      }
    }
    brelse(bp);
    if(b+bi>=FSSIZE){
      cprintf("Error : Data block(1000) exceeded!\n");
      return -1;
    }
  }
  panic("balloc: out of blocks");
}

// Free a disk block.
static void
bfree(int dev, uint b)
{
  struct buf *bp;
  int bi, m, i, max;

  max = (b&255);
  bp = bread(dev, BBLOCK((b>>8), sb));
  for(i=0; i<max; i++){
    bi = ((b>>8)+i) % BPB;
    m = 1 << (bi % 8);
    if((bp->data[bi/8] & m) == 0)
      panic("freeing free block");
    bp->data[bi/8] &= ~m;
    log_write(bp);
  }
  brelse(bp);
}

// Inodes.
struct {
  struct spinlock lock;
  struct inode inode[NINODE];
} icache;

// Drop a reference to an in-memory inode.
// If that was the last reference, the inode cache entry can
// be recycled.
// If that was the last reference and the inode has no links
// to it, free the inode (and its content) on disk.
// All calls to iput_CS() must be inside a transaction in
// case it has to free the inode.
void
iput_CS(struct inode *ip)
{
  acquiresleep(&ip->lock);
  if(ip->valid && ip->nlink == 0){
    acquire(&icache.lock);
    int r = ip->ref;
    release(&icache.lock);
    if(r == 1){
      // inode has no links and no other references: truncate and free.
      itrunc(ip);
      ip->type = 0;
      iupdate(ip);
      ip->valid = 0;
    }
  }
  releasesleep(&ip->lock);

  acquire(&icache.lock);
  ip->ref--;
  release(&icache.lock);
}

// Common idiom: unlock, then put.
void
iunlockput_CS(struct inode *ip)
{
  iunlock(ip);
  iput_CS(ip);
}

//PAGEBREAK!
// Inode content
//
// The content (data) associated with each inode is stored
// in blocks on the disk. The first NDIRECT block numbers
// are listed in ip->addrs[].  The next NINDIRECT blocks are
// listed in block ip->addrs[NDIRECT].

// Return the disk block address of the nth block in inode ip.
// If there is no such block, bmap allocates one.
static uint
bmap_CS(struct inode *ip, uint bn, uint off)
{
  uint addr, *a, tempAddr, bnTemp, count, currP;
  struct buf *bp;

  count = 0;
  currP=0;
  
  if(ip->type==T_CS){ //added for T_CS(p5)
    if(bn < NDIRECT){
      if((addr = ip->addrs[bn]) == 0){
        addr = balloc(ip->dev);
        ip->addrs[bn] = (addr<<8)+1; //first alloced block *256(p5)
        return addr;
      }
      else{
        bnTemp = 0;
        while(ip->addrs[bnTemp]!=0){  //check if block is already allocated in CS inode(p5)
          count = (ip->addrs[bnTemp]&255);
          if(off/BSIZE-currP<count)
            return (ip->addrs[bnTemp]>>8)+off/BSIZE-currP;
          bnTemp++;
          currP+=count;
          if(bnTemp>=NDIRECT)
                  break;
        }
        tempAddr = balloc(ip->dev);
        if(tempAddr==-1)
          return -1;
        //if file had stopped written continuously, save new balloced block in bn+1;
        if(tempAddr != ((ip->addrs[bn]>>8)+(ip->addrs[bn]&255))){
          bnTemp = bn;
          if(ip->addrs[bnTemp]==0){
            ip->addrs[bnTemp] = (tempAddr<<8)+1;
            return tempAddr;
          }
          else{
            while(ip->addrs[bnTemp]!=0){
              if(tempAddr==((ip->addrs[bnTemp]>>8)+(ip->addrs[bnTemp]&255))){
                ip->addrs[bnTemp]++;
                return tempAddr;
              }
              bnTemp++;
              if(bnTemp>=NDIRECT){
                bfree(ip->dev, (tempAddr<<8)+1);  //free allocated block(p5)
                cprintf("Error : Direct block num(12) exceeded!\n");
                return -1;
              }
            }
            ip->addrs[bnTemp] = (tempAddr<<8)+1;
            return tempAddr;
          }
        }
        else{
          if((ip->addrs[bn]&255)==255){
            ip->addrs[bn+1]=(tempAddr<<8)+1;
            return tempAddr;
          }
          ip->addrs[bn]++;
          return tempAddr;
        }
      }
    }
  }
  else{  //if ip type is not T_CS -> used when ilock
    if(bn < NDIRECT){
      if((addr = ip->addrs[bn]) == 0)
        ip->addrs[bn] = addr = balloc(ip->dev);
      return addr;
    }
    bn -= NDIRECT;

    if(bn < NINDIRECT){
      // Load indirect block, allocating if necessary.
      if((addr = ip->addrs[NDIRECT]) == 0)
        ip->addrs[NDIRECT] = addr = balloc(ip->dev);
      bp = bread(ip->dev, addr);
      a = (uint*)bp->data;
      if((addr = a[bn]) == 0){
        a[bn] = addr = balloc(ip->dev);
        log_write(bp);
      }
      brelse(bp);
      return addr;
    }
  }
  

  panic("bmap: out of range");
}

// Truncate inode (discard contents).
// Only called when the inode has no links
// to it (no directory entries referring to it)
// and has no in-memory reference to it (is
// not an open file or current directory).
static void
itrunc(struct inode *ip)
{
  int i;

  for(i = 0; i < NDIRECT; i++){
    if(ip->addrs[i]){
      bfree(ip->dev, ip->addrs[i]);
      ip->addrs[i] = 0;
    }
  }

  ip->size = 0;
  iupdate(ip);
}

//PAGEBREAK!
// Read data from inode.
// Caller must hold ip->lock.
int
readi_CS(struct inode *ip, char *dst, uint off, uint n)
{
  uint tot, m;
  struct buf *bp;

  if(off > ip->size || off + n < off)
    return -1;
  if(off + n > ip->size)
    n = ip->size - off;

  for(tot=0; tot<n; tot+=m, off+=m, dst+=m){
    bp = bread(ip->dev, bmap_CS(ip, off/(BSIZE*255), off));  //modified(p5)
    m = min(n - tot, BSIZE - off%BSIZE);
    memmove(dst, bp->data + off%BSIZE, m);
    brelse(bp);
  }
  return n;
}

// PAGEBREAK!
// Write data to inode.
// Caller must hold ip->lock.
int
writei_CS(struct inode *ip, char *src, uint off, uint n)
{
  uint tot, m, block;
  struct buf *bp;

  if(ip->type == T_DEV){
    if(ip->major < 0 || ip->major >= NDEV || !devsw[ip->major].write)
      return -1;
    return devsw[ip->major].write(ip, src, n);
  }

  //modified for distinguish ip->type (p5)
  if(off > ip->size || off + n < off)
    return -1;
  if(off + n > NDIRECT*255*BSIZE) //not using NINDIRECT! make error when try to use NINDIRECT BLOCK (p5)
    return -1;

  for(tot=0; tot<n; tot+=m, off+=m, src+=m){
    block = bmap_CS(ip, off/(BSIZE*255),off);
    if(block == -1)
      return -1;
    bp = bread(ip->dev, block);
    m = min(n - tot, BSIZE - off%BSIZE);
    memmove(bp->data + off%BSIZE, src, m);
    iupdate(ip);
    log_write(bp);
    brelse(bp);
  }

  if(n > 0 && off > ip->size){
    ip->size = off;
    iupdate(ip);
  }
  
  return n;
}
