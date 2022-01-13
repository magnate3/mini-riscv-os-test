# RISCV Bare Metal
A RISC-V bare-metal example with uart, exception, interrupt and kvmmap.

The tutorial is at [https://mullerlee.cyou/2020/07/09/riscv-exception-interrupt/](https://mullerlee.cyou/2020/07/09/riscv-exception-interrupt/)

## Requirement
- qemu
- riscv64-linux-gnu-*

## Run
```bash
mkdir build
make
make run
```

## Debug
```bash
mkdir build
make
make debug
riscv64-linux-gnu-gdb -x debug.txt
```
## solve an error by this

```
2:
    # initialize global pointer
    la gp, _gp
  
    li      t0, 0xffffffff
    csrw    pmpaddr0, t0
    li      t0, 0xf
    csrw    pmpcfg0, t0
    # initialize stack pointer
    la sp, stack_top
    call   start
```

## mmu

### pagetable_t k_pagetable;

#### (1)init
```
void kvminit() {
    k_pagetable = (pagetable_t)kalloc()
```
#### (2) set satp with  k_pagetable

```
void kvminithart() {
    w_satp(MAKE_SATP(k_pagetable));
    sfence_vma();
}
```

#### (3)  kvmmap use  k_pagetable
kvmmap --> mappages --> walk

```
// uart registers
    kvmmap(UART0, UART0, PGSIZE, PTE_R | PTE_W);
```

```
void kvmmap(uint64_t va, uint64_t pa, uint64_t sz, int perm) {
    mappages(k_pagetable, va, sz, pa, perm);
}

void mappages(pagetable_t pagetable, uint64_t va, uint64_t size, uint64_t pa,
              int perm) {
    uint64_t a, last;
    pte_t *pte;
    a = PGROUNDDOWN(va);
    last = PGROUNDDOWN(va + size - 1);
    for (; a != last + PGSIZE; a += PGSIZE, pa += PGSIZE) {
        pte = walk(pagetable, a, 1);
        *pte = PA2PTE(pa) | perm | PTE_V;
    }
}

pte_t *walk(pagetable_t pagetable, uint64_t va, int alloc) {
    for (int level = 2; level > 0; level--) {
        pte_t *pte = &pagetable[PX(level, va)];
        if (*pte & PTE_V) {
            pagetable = (pagetable_t)PTE2PA(*pte);
        } else {
            if (!alloc || (pagetable = (pde_t *)kalloc()) == 0) return 0;
            c_memset(pagetable, 0, PGSIZE);
            *pte = PA2PTE(pagetable) | PTE_V;
        }
    }
    return &pagetable[PX(0, va)];
}
```

## with no mmu

```
int main() {
    uart_init();
    //kinit();
    //kvminit();
    //kvminithart();
    //paginginit();
    print_s("Hello world!\n");
    print_s("Raise exception to enable timer...\n");
    print_s("Back to user mode\n");
    while (1)
        ;
    return 0;
}
```


```
qemu-system-riscv64 -M virt -kernel kernel.img -bios none -serial stdio -display none
Hello world!
Raise exception to enable timer...
Back to user mode
Timer Interrupt: 1
Timer Interrupt: 2
Timer Interrupt: 3
Timer Interrupt: 4
Timer Interrupt: 5
Timer Interrupt: 6
Timer Interrupt: 7
Timer Interrupt: 8
Timer Interrupt: 9
Timer Interrupt: 10
```

## with mmu

```
int main() {
    uart_init();
    kinit();
    kvminit();
    kvminithart();
    paginginit();
    print_s("Hello world!\n");
    print_s("Raise exception to enable timer...\n");
    print_s("Back to user mode\n");
    while (1)
        ;
    return 0;
}
```

```
qemu-system-riscv64 -M virt -kernel kernel.img -bios none -serial stdio -display none
Timer Interrupt: 1
Timer Interrupt: 2
Timer Interrupt: 3
Hello world!
Raise exception to enable timer...
Back to user mode
Timer Interrupt: 4
Timer Interrupt: 5
Timer Interrupt: 6
Timer Interrupt: 7
Timer Interrupt: 8
Timer Interrupt: 9
Timer Interrupt: 10

```
