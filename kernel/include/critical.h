#ifndef _CRITICAL_H
#define _CRITICAL_H

#include <stdio.h>

//assembly import
extern "C"{
void _cli();
void _sti();
void halt();
}

//panic
extern __attribute__((noinline, noreturn))
void kpanic(const char *);

#endif