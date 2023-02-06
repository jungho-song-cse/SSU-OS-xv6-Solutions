3450 struct buf {
3451   int flags;
3452   uint dev;
3453   uint blockno;
3454   struct sleeplock lock;
3455   uint refcnt;
3456   struct buf *prev; // LRU cache list
3457   struct buf *next;
3458   struct buf *qnext; // disk queue
3459   uchar data[BSIZE];
3460 };
3461 #define B_VALID 0x2  // buffer has been read from disk
3462 #define B_DIRTY 0x4  // buffer needs to be written to disk
3463 
3464 
3465 
3466 
3467 
3468 
3469 
3470 
3471 
3472 
3473 
3474 
3475 
3476 
3477 
3478 
3479 
3480 
3481 
3482 
3483 
3484 
3485 
3486 
3487 
3488 
3489 
3490 
3491 
3492 
3493 
3494 
3495 
3496 
3497 
3498 
3499 
