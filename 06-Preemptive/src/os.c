#include "os.h"

extern void trap_init(void);

void os_kernel()
{
	lib_puts("kernel start\n");
	task_os();
}

void os_start()
{
	lib_puts("OS start\n");
	user_init();
	trap_init();
	//configure Physical Memory Protection to give supervisor mode
	// access to all of physical memory.
	//w_pmpaddr0(0x3fffffffffffffull);
	//w_pmpcfg0(0xf);
	//w_satp(0);
	timer_init(); // start timer interrupt ...
}

int os_main(void)
{
	os_start();
	uart_puts("OS start loop \n");
	int current_task = 0;
	while (1)
	{
		lib_printf("OS: Activate next task %d\n", current_task);
		task_go(current_task);
		lib_puts("OS: Back to OS\n");
		current_task = (current_task + 1) % taskTop; // Round Robin Scheduling
		lib_puts("\n");
	}
	return 0;
}
