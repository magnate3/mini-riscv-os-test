#include "os.h"

void os_kernel() {
	task_os();
}

void os_start() {
	uart_puts("OS start\n");
	user_init();
}

int os_main(void)
{
	os_start();
	
	int current_task = 0;
	while (1) {
		uart_puts("OS: Activate next task\n");
		task_go(current_task);
		uart_puts("OS: Back to OS\n");
		current_task = (current_task + 1) % taskTop; // Round Robin Scheduling
		uart_puts("\n");
	}
	return 0;
}

