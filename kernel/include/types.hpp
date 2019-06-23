#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <stdint.h>

typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t dword;
typedef uint64_t qword;

typedef int8_t sbyte;
typedef int16_t sword;
typedef int32_t sdword;
//typedef int64_t sqword;

typedef word paddr;
typedef dword addr_t;
typedef dword offset_t;

typedef long unsigned int size_t;

#undef to_addr_t
#define to_addr_t(x) (addr_t)x

#undef voidcast
#define voidcast(x) (reinterpret_cast<void *>(x))

#define __packed __attribute__((packed)) 
#define __nooptimize __attribute__((optimize("-O0")))
#define __maxoptimize __attribute__((optimize("-O3")))
#define __nonreturn __attribute__((nonreturn))
#define __fastcall __attribute__((fastcall))
#define __used __attribute__((used))
#define __ASM_IMPORT extern "C"


#endif //TYPES_H