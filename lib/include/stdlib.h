#ifndef _STDLIB_H
#define _STDLIB_H 1
 
//#include <sys/cdefs.h>
#include <stdbool.h>
#include <_os.h>
#include <types.hpp>

#ifdef __cplusplus
extern "C" {
#endif

//typedef int site_t;

__attribute__((__noreturn__))
void abort(void);

int atoi(char *str);

char *itoa (int value, char * str, int base);

//#if defined(__is_libk)
//	extern void halt(void);
//#endif

#ifdef __cplusplus
}
#endif
 
#endif