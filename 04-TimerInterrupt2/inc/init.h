#ifndef __INIT_H__
#define __INIT_H__

#include "bits.h"
#include "encoding.h"
#include "mtrap.h"
//#include <stddef.h>
//#include <stdarg.h>
typedef unsigned long int uintptr_t;
extern void machine_mode_fn(uintptr_t arg0, uintptr_t arg1);
extern void supervisor_mode_fn(uintptr_t arg0);

#endif
