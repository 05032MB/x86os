#include <overseer.hpp>

#include <types.hpp>
#include <scheduler.hpp>
#include <logger.hpp>
#include <syscalls.hpp>
#include <critical.hpp>

#include <kheap.hpp>
#include <task.hpp>

#include <string.h>

template <typename T>
static constexpr T min(T a, T b)
{
	return (a > b ? b : a);
}

template <typename T>
struct rem_ref {using type = T;};
template <typename T>
struct rem_ref<T&> {using type = T;};

template <typename T>
struct rem_const {using type = T;};
template <typename T>
struct rem_const<const T> {using type = T;};

sched_manager::sched_manager()
{

}

void sched_manager::add_scheduler(scheduler * sh)
{
	if(last_sched_in_list < num_of_instances)
	{
		schedulers_list[last_sched_in_list++] = sh;
	}
}
void sched_manager::schedule()
{
	const auto &end = last_sched_in_list;
	for(rem_const<rem_ref<decltype(end)>::type>::type i = 0; i < end; i++)
	{
		schedulers_list[i]->schedule();
	}
}
void sched_manager::schedule(unsigned n)
{
	if(n >= last_sched_in_list)return;
	schedulers_list[n]->schedule();
}
void sched_manager::seq_dispatch()
{
	const auto &end = last_sched_in_list;
	for(rem_const<rem_ref<decltype(end)>::type>::type i = 0; i < end; i++)
	{
		active_sched = schedulers_list[i];
		active_sched->dispatch();
	}
	active_sched = nullptr;
	kpanic("[Overseer] All schedulers refusing dispatch.");
}

void sched_manager::dispatch(unsigned n)
{
	if(n >= last_sched_in_list)return;
	active_sched = schedulers_list[n];
	active_sched->dispatch();
}

void sched_manager::exit(byte b)
{
	if(active_sched == nullptr){
		term_log("Can't exit, no process to stop.", LOG_CRITICAL);
		return;
	}
	active_sched->front_task()->exit(b);
	active_sched->schedule();
	seq_dispatch();
	
}

void sched_manager::exit_call(byte b)
{
	overseer().exit(b);
}
void sched_manager::exec_call()
{
	task2* t = _SCALLOC(task2);
}

void init_overseer()
{
	return;
}