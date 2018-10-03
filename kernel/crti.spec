section .init
global _init
_init:
	push ebp
	mov long ebp, esp

section .fini
global _fini
_fini:
	push ebp
	mov long ebp, esp