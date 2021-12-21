#include "lib.h"

void lib_delay(volatile int count)
{
	count *= 50000;
	while (count--);
}

