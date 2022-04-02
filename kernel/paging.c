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

  // if(va >= p->sz || va < PGROUNDUP(p->trapframe->sp)){
  struct proc *p = myproc();
  if(va >= p->sz){
    return -1;
  }
  va = PGROUNDDOWN(va);
  pte_t *pte = walk(p->pagetable, va, 0);
  if(*pte & PTE_S){
    uint64 blockno = PTE2BLOCKNO(*pte);
    uint64 *pa1 = kalloc();
    if(pa1 == 0){
      return -1;
    }
    memset(pa1, 0, PGSIZE);
    begin_op();
    read_page_from_disk(ROOTDEV, (char *)pa1, blockno);
    bfree_page(ROOTDEV, blockno);
    end_op();
    *pte = PA2PTE(pa1) | PTE_FLAGS(*pte);
    *pte |= PTE_V;
    *pte &= ~PTE_S;;
  }
  else if(!(*pte & PTE_V)){
    char *pa2 = kalloc();
    if(pa2 == 0){
      return -1;
    }
    memset(pa2, 0, PGSIZE);
    if(mappages(p->pagetable, va, PGSIZE, (uint64)pa2, PTE_W|PTE_X|PTE_R|PTE_U) != 0){
      kfree(pa2);
      return -1;
    }
  }
  return 0;
  // panic("not implemented yet\n");
}
