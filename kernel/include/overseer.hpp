#ifndef _OVERSEER_H_
#define _OVERSEER_H_

#include <types.hpp>

class scheduler;
//proc overseer

class sched_manager{
	static constexpr unsigned num_of_instances = 1;

	scheduler * schedulers_list[num_of_instances];
	unsigned last_sched_in_list{0};

	scheduler * active_sched{nullptr};

	sched_manager();

	void exit(byte b);

public:

	static sched_manager& overseer()
	{
		static sched_manager overseer;
		return overseer;
	}

	void add_scheduler(scheduler *);
	void schedule();
	void schedule(unsigned n);
	void seq_dispatch();
	void dispatch(unsigned );

	static void exit_call(byte b);
	static void exec_call();

	//
	sched_manager(sched_manager const &) = delete;
	void operator=(sched_manager const &) = delete;

};

constexpr auto overseer = &sched_manager::overseer;

void init_overseer();

#endif