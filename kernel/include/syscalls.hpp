#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <types.hpp>
#include <interrupts.hpp>

//interrupt gate 48 is reserved for system calls
extern void register_interrupt_handler(word interrupt, isr_func_t fun); //func to register int handler from interrupts.hpp

/*
convention for raising syscalls:
eax - syscall number
ebx, ecx, edx, edi, esi - parameters

syscalls are not to utilize stack to pass parameters
*/

#define SYSCALL_MAX 11

//extern void* syscalls[SYSCALL_MAX];

void init_syscalls();

bool register_syscall(int, void*);
void syscall_handler(const int_iden);


#endif