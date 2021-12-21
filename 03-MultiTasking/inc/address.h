#ifndef QEMU_RISCV64_ADDRESS_H_
#define QEMU_RISCV64_ADDRESS_H_

// the kernel expects there to be RAM
// for use by the kernel and user pages
// from physical address 0x80000000 to PHYSTOP.
#define KERNBASE 0x80000000L
#define PHYSTOP (KERNBASE + 128*1024*1024)

#define UART0_REG_BASE      (0x10000000L)
#define CLINT_REG_BASE      (0x02000000L)

#endif /* QEMU_RISCV64_ADDRESS_H_ */
