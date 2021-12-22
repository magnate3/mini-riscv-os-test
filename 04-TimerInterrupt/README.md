# 02-ContextSwitch

## Build & Run

```sh
make run
rm -f build/*_asm.o build/*.o kernel.elf kernel.img
riscv64-linux-gnu-gcc -g -I inc -c src/sys.s -o build/sys_asm.o
riscv64-linux-gnu-gcc -g -I inc -c src/start.s -o build/start_asm.o
riscv64-linux-gnu-gcc -Wall -mcmodel=medany -g -I inc -O0 -c src/uart.c -o build/uart.o
riscv64-linux-gnu-gcc -Wall -mcmodel=medany -g -I inc -O0 -c src/os.c -o build/os.o
riscv64-linux-gnu-ld -T link.ld -o kernel.elf build/sys_asm.o build/start_asm.o build/uart.o build/os.o
objcopy kernel.elf -I binary kernel.img
qemu-system-riscv64 -M virt -kernel kernel.img -bios none -serial stdio -display none
OS start
Task0: Context Switch Success !
qemu-system-riscv64: terminating on signal 2
```
