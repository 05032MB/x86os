#ifndef _SCHED_HPP
#define _SCHED_HPP

class task2;

class scheduler
{
public:
	scheduler() = default;
	~scheduler() = default;

	virtual void schedule() = 0; //schedule tasks with an algorithm
	virtual void dispatch() = 0; //launch new task, previously scheduled. Should be O(1)

	virtual void accept_task(task2 *) = 0; //add a new task
	virtual task2* accept_create_task() = 0; //creates new task on heap

	virtual task2* front_task() = 0; //task which is currently being served
	
};

class non_preempt_fifo : public scheduler
{
private:
	class fq{
	struct fq_node{
		task2* me;
		fq_node* next;
		bool destroy_on_exit;
	} *first{nullptr};
	public:
		void push(task2 *, bool dt = false);
		task2 * pop();
		task2 * top()const;

		fq();
		~fq();
	}q;

	task2* current;

public:
	non_preempt_fifo() = default;
	~non_preempt_fifo() = default;

	void schedule(); 
	void dispatch();

	void accept_task(task2 *);
	task2* accept_create_task();

	task2* front_task();
};

#endif