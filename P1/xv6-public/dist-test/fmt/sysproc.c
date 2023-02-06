3350 #include "types.h"
3351 #include "x86.h"
3352 #include "defs.h"
3353 #include "date.h"
3354 #include "param.h"
3355 #include "memlayout.h"
3356 #include "mmu.h"
3357 #include "proc.h"
3358 
3359 int
3360 sys_fork(void)
3361 {
3362   return fork();
3363 }
3364 
3365 int
3366 sys_exit(void)
3367 {
3368   exit();
3369   return 0;  // not reached
3370 }
3371 
3372 int
3373 sys_wait(void)
3374 {
3375   return wait();
3376 }
3377 
3378 int
3379 sys_kill(void)
3380 {
3381   int pid;
3382 
3383   if(argint(0, &pid) < 0)
3384     return -1;
3385   return kill(pid);
3386 }
3387 
3388 int
3389 sys_getpid(void)
3390 {
3391   return myproc()->pid;
3392 }
3393 
3394 
3395 
3396 
3397 
3398 
3399 
3400 int
3401 sys_sbrk(void)
3402 {
3403   int addr;
3404   int n;
3405 
3406   if(argint(0, &n) < 0)
3407     return -1;
3408   addr = myproc()->sz;
3409   if(growproc(n) < 0)
3410     return -1;
3411   return addr;
3412 }
3413 
3414 int
3415 sys_sleep(void)
3416 {
3417   int n;
3418   uint ticks0;
3419 
3420   if(argint(0, &n) < 0)
3421     return -1;
3422   acquire(&tickslock);
3423   ticks0 = ticks;
3424   while(ticks - ticks0 < n){
3425     if(myproc()->killed){
3426       release(&tickslock);
3427       return -1;
3428     }
3429     sleep(&ticks, &tickslock);
3430   }
3431   release(&tickslock);
3432   return 0;
3433 }
3434 
3435 // return how many clock tick interrupts have occurred
3436 // since start.
3437 int
3438 sys_uptime(void)
3439 {
3440   uint xticks;
3441 
3442   acquire(&tickslock);
3443   xticks = ticks;
3444   release(&tickslock);
3445   return xticks;
3446 }
3447 
3448 
3449 
