```cpp
pte_t *
walk(pagetable_t pagetable, uint64 va, int alloc)
{
  if(va >= MAXVA)
    panic("walk");

  for(int level = 2; level > 0; level--) {
    pte_t *pte = &pagetable[PX(level, va)];
    if(*pte & PTE_V) {
      pagetable = (pagetable_t)PTE2PA(*pte);
    } else {
      if(!alloc || (pagetable = (pde_t*)kalloc()) == 0)
        return 0;
      memset(pagetable, 0, PGSIZE);
      *pte = PA2PTE(pagetable) | PTE_V;
    }
  }
  return &pagetable[PX(0, va)];
}

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
```
got PUBLIC

* 这里如果要调用walk函数，pagetable指的是页表（的物理地址），页表是一个有大量虚拟地址的结构体，提供虚拟地址到物理地址的映射。va指的是虚拟地址，虚拟地址离开了页表不能表示任何东西，需要依靠页表来索引到实际的物理地址。页表中间27个比特，每9比特映射一个物理地址，前两个9比特映射到二、一级页表（L2、L1），第三个映射到零级页表（L0）。二级页表中，存档的是一级页表的地址（指明信息存放在哪一个页表），我们需要跳转到一级页表后，使用第二个9比特在一级页表中找到下一项，即零级页表的地址（不然无法得知零级页表在哪），最后使用va的最后一个九比特，找到这个虚拟地址对应的物理地址。

* sys_pgaccess函数实际要做的是，检测哪一个虚拟**页**被访问过（而不是页表）。页是这里最小的内存单元，我们计算都应该以PGSIZE为单位
