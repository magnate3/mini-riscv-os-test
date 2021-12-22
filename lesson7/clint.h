#ifndef QEMU_RISCV64_CLINT_CLINT_H_
#define QEMU_RISCV64_CLINT_CLINT_H_

#define TIMER_CLK_RATE  (10 * 1000 * 1000) //10MHz

void msoftint_make(void);
void msoftint_clear(void);
unsigned long timer_get(void);
void timer_set(unsigned long interval);

#endif /* QEMU_RISCV64_CLINT_CLINT_H_ */