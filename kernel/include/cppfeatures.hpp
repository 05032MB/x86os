#ifndef _CPP_INC
#define _CPP_INC

#include <types.hpp>

__ASM_IMPORT {

 int __cxa_atexit(void (*func) (void *), void * arg, void * dso_handle);
 void __cxa_pure_virtual();
 bool __cxa_guard_acquire(addr_t& guardian);
 void __cxa_guard_release(addr_t& guardian);

}

#endif