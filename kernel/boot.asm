%macro exp 1
global %1
%1
%endmacro

%macro calle 1 ;call extern
extern %1
call %1
%endmacro


MBALIGN equ 1<<0           
MEMINFO equ 1 << 1            
FLAGS equ MBALIGN | MEMINFO 
MAGIC equ 0x1BADB002        
CHECKSUM equ - (MAGIC + FLAGS)   
 
section .multiboot;need to rebuild
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM
 
section .bss
align 32
exp auxillary_stack_bottom:
resb 16384 ; 16
exp auxillary_stack_top:

stack_bottom:
resb 16384*2 ; 32 KiB
stack_top:
 
section .text
;global _entry:function (_entry.end - _entry)
exp _entry:

	mov esp, stack_top
 	
	push ebx ;get multiboot info struct

	calle kernel_main
 
	cli
.hang:	hlt
	jmp .hang
.end: