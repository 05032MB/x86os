#ifndef _CRITICAL_H
#define _CRITICAL_H

#include <types.hpp>

//panic
__nooptimize __noreturn
void kpanic(const char *);

#endif