# 02-ContextSwitch

## Build & Run

```sh
make run
rm -f build/*_asm.o build/*.o kernel.elf kernel.img
riscv64-linux-gnu-gcc -g -I inc -c src/sys.s -o build/sys_asm.o
riscv64-linux-gnu-gcc -g -I inc -c src/start.s -o build/start_asm.o
riscv64-linux-gnu-gcc -Wall -mcmodel=medany -g -I inc -O0 -c src/lib.c -o build/lib.o
riscv64-linux-gnu-gcc -Wall -mcmodel=medany -g -I inc -O0 -c src/user.c -o build/user.o
src/user.c: In function ‘user_task0’:
src/user.c:10:3: warning: implicit declaration of function ‘lib_delay’ [-Wimplicit-function-declaration]
   lib_delay(1000);
   ^~~~~~~~~
riscv64-linux-gnu-gcc -Wall -mcmodel=medany -g -I inc -O0 -c src/uart.c -o build/uart.o
riscv64-linux-gnu-gcc -Wall -mcmodel=medany -g -I inc -O0 -c src/os.c -o build/os.o
riscv64-linux-gnu-gcc -Wall -mcmodel=medany -g -I inc -O0 -c src/task.c -o build/task.o
riscv64-linux-gnu-ld -T link.ld -o kernel.elf build/sys_asm.o build/start_asm.o build/lib.o build/user.o build/uart.o build/os.o build/task.o
objcopy kernel.elf -I binary kernel.img
qemu-system-riscv64 -M virt -kernel kernel.img -bios none -serial stdio -display none
OS start
OS: Activate next task
Task0: Created!
Task0: Now, return to kernel mode
OS: Back to OS

OS: Activate next task
Task1: Created!
Task1: Now, return to kernel mode
OS: Back to OS

OS: Activate next task
Task0: Running...
OS: Back to OS

OS: Activate next task
Task1: Running...
OS: Back to OS

OS: Activate next task
Task0: Running...
OS: Back to OS

OS: Activate next task
Task1: Running...
OS: Back to OS

OS: Activate next task
Task0: Running...
OS: Back to OS

OS: Activate next task
Task1: Running...
OS: Back to OS

OS: Activate next task
Task0: Running...
OS: Back to OS

OS: Activate next task
Task1: Running...
OS: Back to OS

OS: Activate next task
Task0: Running...
OS: Back to OS

OS: Activate next task
Task1: Running...
OS: Back to OS

OS: Activate next task
Task0: Running...
qemu-system-riscv64: terminating on signal 2
```
