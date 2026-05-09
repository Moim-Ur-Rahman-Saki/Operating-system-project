#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
extern struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

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
int sys_setpriority(void) {
  int pid, priority;

  if(argint(0, &pid) < 0 || argint(1, &priority) < 0)
    return -1;

  struct proc *p;
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->pid == pid){
      p->priority = priority;
      return 0;
    }
  }
  return -1;
}
int
sys_getwaitingtime(void)
{
  return myproc()->waiting_time;  
}


int sys_getprocinfo(void) {
    struct pinfo *info;

    if(argptr(0, (void*)&info, sizeof(*info)) < 0)
        return -1;

    struct proc *p = myproc();

    info->pid = p->pid;
    safestrcpy(info->name, p->name, sizeof(p->name));

    info->state = p->state;   // for state

    return 0;
}

char* getStateName(int state) {
    switch(state) {
        case UNUSED: return "UNUSED";
        case EMBRYO: return "EMBRYO";
        case SLEEPING: return "SLEEPING";
        case RUNNABLE: return "RUNNABLE";
        case RUNNING: return "RUNNING";
        case ZOMBIE: return "ZOMBIE";
        default: return "UNKNOWN";
    }
}

int sys_printprocs(void) {
    struct proc *p;

    acquire(&ptable.lock);

 for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->state == UNUSED)
        continue;

    int ppid = 0;
    if(p->parent)
        ppid = p->parent->pid;

    //  child count calculate
    int child_count = 0;
    struct proc *q;

    for(q = ptable.proc; q < &ptable.proc[NPROC]; q++){
        if(q->parent == p)
            child_count++;
    }

    cprintf("PID: %d | Name: %s | State: %s | Mem: %d | PPID: %d | Child: %d\n",
        p->pid, p->name, getStateName(p->state), p->sz, ppid, child_count);
}
    release(&ptable.lock);
    return 0;
}