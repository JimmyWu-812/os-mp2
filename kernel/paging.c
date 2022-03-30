#include "param.h"
#include "types.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "defs.h"
#include "proc.h"

/* NTU OS 2022 */
/* Page fault handler */
int handle_pgfault() {
  /* Find the address that caused the fault */
  uint64 va = r_stval();

  /* TODO */
  struct proc *p = myproc();
  char *pa = kalloc();

  va = PGROUNDDOWN(va);
  if(pa == 0){
    return -1;
  }
  memset(pa, 0, PGSIZE);
  if(mappages(p->pagetable, va, PGSIZE, (uint64)pa, PTE_W|PTE_X|PTE_R|PTE_U) != 0){
    kfree(pa);
    return -1;
  }
  return 0;

  // panic("not implemented yet\n");
}
