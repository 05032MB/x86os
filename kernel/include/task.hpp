#ifndef _TASK_H
#define _TASK_H

#include <types.hpp>

static unsigned top_pid = 1;

struct task_header_t{

	const unsigned magic = 0xE451E;
	unsigned pid;

};
struct task_footer_t{
	const unsigned magic = 0xE451F;
};

class task{
	task_header_t* header;
	task_footer_t* footer;

	offset_t entry;
	offset_t stack_top;
	unsigned pid = 1;

	size_t memsize_max;

public:
	void prepare_task(task_header_t*, task_footer_t*, addr_t, size_t);
	bool launch_task();

	auto getPid(){return pid;};

	static void yield();

};
extern class task only_task;

#endif