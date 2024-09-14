#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
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

uint64
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


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  uint64 va;
  int page_num;
  uint64 useraddress;

  if(argaddr(0, &va) < 0 || argint(1, &page_num) < 0 || argaddr(2, &useraddress) < 0)
    return -1;
  // pagetables里有page_num个页表，对每个页表，检查里面所有的pte，如果有发现有PTE_A，就说明有访问过?

  struct proc* p = myproc();
  uint64 tmp_va = va;
  uint64 result = 0;
  for(int i = 0; i < page_num; i++)
  {
    tmp_va = va + i * PGSIZE;
    pte_t *pte = walk(p->pagetable, tmp_va, 0);
    if(pte && (*pte & PTE_A))
    {
      result |= (1 << i);
      *pte &= (~PTE_A);
    }
  }
  copyout(p->pagetable, useraddress, (char*)&result, sizeof(uint64));
  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
