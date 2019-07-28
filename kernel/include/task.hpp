#ifndef _TASK_H
#define _TASK_H

#include <types.hpp>

class task_basic_interface{
public:
	virtual void launch(void) = 0;
	virtual void exit(byte) = 0;
};

class task2 /*: public task_basic_interface*/{

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
	}regs;
} *tinfo{nullptr};

public:

	enum class state{
		INCOMING,
		RUNNING,
		READY,
		ENDING,
		CALLQUITS
	};

	byte prepare_task_from_elf(void * elf, void* sh, void* hl, dword *pd);
	void launch(void);
	void suspend(const task_info::reg_t &rgs);
	void exit(byte);

	void set_state(task2::state);
	task2::state get_state()const;

	dword get_pid()const{return tinfo->pid;}

	task2();
	task2(task_info *);
	~task2();

	static void yield();

private:
	state proc_state{task2::state::INCOMING};

};

extern class task2 only_task;

#endif