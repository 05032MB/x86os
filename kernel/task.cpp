#include <task.hpp>
#include <logger.hpp>

extern "C" __fastcall void switch_to_ring_3(void(*)());
extern "C" __fastcall void _jump_as_if_userspace_task(void(*)(), addr_t);
extern "C" void halt();

task only_task;

void task::prepare_task(task_header_t* th, task_footer_t* tf, addr_t en, size_t s)
{
	entry = en, header = th, footer = tf, memsize_max = s;
	pid = top_pid++;
}
bool task::launch_task()
{
	_jump_as_if_userspace_task((void(*)())entry, 0xA00050);
}
__nooptimize
void task::yield()
{
	term_log("Yield test call", 1,LOG_CRITICAL);
	halt();
}