3500 // Long-term locks for processes
3501 struct sleeplock {
3502   uint locked;       // Is the lock held?
3503   struct spinlock lk; // spinlock protecting this sleep lock
3504 
3505   // For debugging:
3506   char *name;        // Name of lock.
3507   int pid;           // Process holding lock
3508 };
3509 
3510 
3511 
3512 
3513 
3514 
3515 
3516 
3517 
3518 
3519 
3520 
3521 
3522 
3523 
3524 
3525 
3526 
3527 
3528 
3529 
3530 
3531 
3532 
3533 
3534 
3535 
3536 
3537 
3538 
3539 
3540 
3541 
3542 
3543 
3544 
3545 
3546 
3547 
3548 
3549 
