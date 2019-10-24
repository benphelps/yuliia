#ifndef DEBUG_H

#include <arch/types.h>

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

extern void panic(char *message, char *file, u32int line);
extern void panic_assert(char *file, u32int line, char *desc);

#endif // !DEBUG_H
