%macro exp 1
global %1
%1
%endmacro

%macro calle 1 ;call extern
extern %1
call %1
%endmacro

section .bss

global KERNEL_CS, KERNEL_DS, USER_CS, USER_DS

KERNEL_CS resw 1;kernel code segment selector
KERNEL_DS resw 1;kernel data segment selector
USER_CS resw 1
USER_DS resw 1

;vl resw 256*1024*1024; 1GB

section .text


exp _term_override:
	mov ax, 0x8 ; data selector
	mov gs, ax
	mov dword [gs:0xB8000], 'OV'
	;mov dword [gs:0xB8000], esp
	hlt

;name: _lidt
;action: executes lidt command (sets up IDT)
;in: pointer to idt structure
;out: nope
;C prototype: __attribute__((fastcall)) void _lidt(idtptr* i); [fastcall so param is passed directly to ecx/edx]
exp _lidt:
	lidt [ecx]
	ret
;;

;name: _push
;action: pushes element on stack
;in:  element(32bit)
;out: nope
;C prototype: __attribute__((fastcall, optnone)) void _push(dword i);
exp _push:
	pop eax
	push ecx
	push eax
	ret
;name: _pop
;action: pops element from stack
;in:  no
;out: yes
;C prototype: __attribute__((optnone)) dword _pop(void);
exp _pop:
	pop eax
	add esp, 4
	push eax
	ret
	
;;;;;;;;;;;;;;;;;;;;
exp get_int_handler:
	mov eax, int_handler
	ret

;assembly 32 bit int handling func
exp int_handler:
	pushad
	
	push ds
	push es
	push fs
	push gs
	
	push ax
	mov ax, [KERNEL_DS] ; kernel data selector
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	pop ax
	;add esp, 4*4
	
	calle _c_int_handler
	
	;sub esp, 4*4
	pop gs
	pop fs
	pop es
	pop ds
	
	popad	
	
	add esp, 4*2 ;rem fake error code & iden. (2 * 32-bit regs (32/8=4bytes))
	sti
	iret


;name:		halt
; action:	halts processor until next interrupt
; in:		nothing
; out:		nothing
; C prototype: void halt(void);
exp halt:
	hlt
	ret
	
exp _sti:
	sti
	ret
	
exp _cli:
	cli
	ret
	
exp _nop:
	nop
	ret;
	
exp div0:
	mov     eax, 15
	mov     ecx, 0
	;div     ecx
	ret

;;;;;;;;;;;Segmentation;;;;;;;;;;;;;;;;;;;;;;;
	
exp _lgdt:
	lgdt [ecx]
	ret

exp _set_userspace_selectors:
	;edx - code selector
	;ecx - data selector
	
	mov [USER_CS], dx
	mov [USER_DS], cx
	
	ret
	
exp _on_gdt_change:
	;edx - code selector
	;ecx - data selector
	
	mov ax, cx ;cx->data selector
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	
	mov [KERNEL_CS], dx
	mov [KERNEL_DS], cx
	

	push dword [KERNEL_CS];
	push dword _returner;
	retf ;far return
	
	hlt ;somethings wrong
	
	;jmp 0x8:_returner ;far jump to update cs. 
	
_returner:
	ret
;;;;;;;;;;;;;;;;;;segEnd;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;userland&multitasking;;;;;;;;;;;;;;
exp _get_esp:
	mov eax, esp
	ret
exp _tss_flush:
	mov ax, cx
	ltr ax
	ret
	
exp switch_to_ring_3: ;fastcall, cx - function address
	cli
	mov ax, [USER_DS]
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	;hlt
	mov eax, esp
	push dword [USER_DS]
	push dword eax
	pushf
	push dword [USER_CS]
	push dword ecx
	;hlt
	iret

exp _jump_as_if_userspace_task: ;fastcall, ecx - function address, edx - new stack
	cli
	mov esp, edx ; jest ok
	mov ax, [USER_DS]
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov eax, esp
	push dword [USER_DS]
	push dword eax
	pushf
	push dword [USER_CS]
	push dword ecx

	iret

exp _lets_err:
	;cli
	pushad
	mov eax, 1
	mov ebx, 'c'
	mov ecx, 0xF
	
	int 80
	
	mov eax, 2
	mov ebx, tsts
	mov ecx, 0xF
	
	int 80
	
	mov eax, 2
	mov ebx, string111
	mov ecx, 0xF
	
	int 80
	
	mov eax, 3
	mov ebx, 0x11
	
	int 80
	
	popad
	ret
;;;;;;;;;;;;;;;;;end;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;useful in mem managment
;;;write and read control registers cr0 , cr3
;;;setters must use __fastcall calling convention
	
exp _get_cr0:
	mov eax, cr0;
	ret

exp _get_cr2:
	mov eax, cr2;
	ret

exp _get_cr3:
	mov eax, cr3;
	ret

exp _write_cr0:
	mov cr0, ecx
	ret

exp _write_cr3:
	mov cr3, ecx
	ret
	
exp _reload_tlbs:
	mov eax, cr3
	mov cr3, eax
	ret

exp _invlpg:
	invlpg [ecx]
	ret
;;;;;;;INTERRUPT SERVICE ROUTINES!!;;;;;;;;
;Format-------
;isr###:
;push (fake) error code
;push identifier (###)
;jmp int_handler
; 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;macros;;;;;;;;;;;;;;;;;;;;;;;
%macro ISR_NOERROR 1
isr%1:
	cli
	push dword 0
	push %1
	jmp long int_handler ;force long jump
%endmacro

%macro ISR_ERROR 1
isr%1:
	cli
	nop
	nop ; push 0
	push %1
	jmp long int_handler
%endmacro
;;;;;;;;;;;endmacros;;;;;;;;;;;;;

%assign i 0
%rep 8
	ISR_NOERROR i
	%assign i i+1
%endrep
ISR_ERROR 8;Double Fault
ISR_NOERROR 9;

%assign i 10
%rep 5 
	ISR_ERROR i
	%assign i i+1
%endrep

ISR_NOERROR 15;isr15
ISR_NOERROR 16;isr16
ISR_ERROR 17;isr17

%assign i 18
%rep 12 ;4+8
	ISR_NOERROR i
	%assign i i+1
%endrep

ISR_ERROR 30
ISR_NOERROR 31

%assign i 32
%rep 200
	ISR_NOERROR i
	%assign i i+1
%endrep

;ISR_NOERROR 33

;;;;;;;;;;;;;;;;;;;;;;;;;;;
exp _get_isr_size:
	mov ebx, isr0
	mov eax, isr1
	sub eax, ebx
	ret;	
	
exp _get_isr0_addr:
	mov eax, isr0;
	ret;
	
;;;;;;;;;;;;;;ISR END;;;;;;;;;;;;;;;

section .data
tsts db "SURTR\0"
string111 db "Printing number in hex\n"

section .text