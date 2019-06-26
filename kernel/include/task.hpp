#ifndef _TASK_H
#define _TASK_H

#include <types.hpp>

static dword top_pid = 1;

struct task_info{
	dword pid;
	addr_t entry;
	addr_t mem_low;
	addr_t mem_high;
	addr_t stack_high;
	addr_t heap_low;

	dword * page_dir;

	//for context switching
	struct reg_t{
		dword eax,ebx,ecx,edx,edi,esi;
		dword ebp, esp;
		dword eip;
		dword gs, fs, es, ds;
		dword eflags;
	};
}__packed;

class task2{

	task_info *tinfo;

public:
	byte prepare_task_from_elf(void * elf, void* sh, void* hl, dword *pd);
	void launch(void);
	void suspend();

	static void yield();

};

extern class task2 only_task;

#endif