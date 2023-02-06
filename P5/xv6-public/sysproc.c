#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

//syscall weightset (p3)
int sys_weightset(void)
{
  int newWeight;
  
  if(argint(0, &newWeight) < 0)
    return -1;
  if(newWeight==0){
    cprintf("error, use weight not 0\n");
    return -1;
  }

  weightset(newWeight); 

  return 0;
}

//syscall printProcInfo(p3)
int sys_printProcInfo(void)
{
  int time;
  struct proc* curproc = myproc();

  if(argint(0, &time) < 0)
    return -1;

  cprintf("PID: %d, WEIGHT: %d, TIMES : %d ms\n",curproc->pid, curproc->weight, time);//tip: cprintf can get max three arguments(p3)

  return 0;
}

//syscall printTerminated(p3)
int sys_printTerminated(void)
{
  struct proc* curproc = myproc();

  cprintf("PID: %d is termiated\n",curproc->pid);

  return 0;
}
