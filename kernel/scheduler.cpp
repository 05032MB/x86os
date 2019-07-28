#include <scheduler.hpp>
#include <kheap.hpp>
#include <types.hpp>
#include <task.hpp>
#include <critical.hpp>

//FIFO Q

non_preempt_fifo::fq::fq()
{}
non_preempt_fifo::fq::~fq()
{
	for(auto *e = first; e != nullptr; )
	{
		auto *next = e->next;
		__FREE(voidcast(e));
		e = next;
	}
}

void non_preempt_fifo::fq::push(task2 * t2, bool dt /* = true */)
{
	if(first == nullptr){
		first = _SCALLOC(fq_node);
		first->me = t2, first->next = nullptr;
		if(dt)first->destroy_on_exit = true;
		else first->destroy_on_exit = false;

		return;
	}
	{
		auto *last = first;
		while(last->next != nullptr)
		{
			last = last->next;
		}
		last->next = _SCALLOC(fq_node);
		last = last->next;

		last->me = t2;
		last->next = nullptr;
		if(dt)last->destroy_on_exit = true;
		else last->destroy_on_exit = false;
	}
}
task2* non_preempt_fifo::fq::pop()
{
	if(first == nullptr)return nullptr;

	auto * ret = first->me;
	if(first->destroy_on_exit == true){
		__FREE(ret);
		ret = nullptr;
	}
	first = first->next;

	return ret;
}
task2* non_preempt_fifo::fq::top()const
{
	if(first == nullptr)return nullptr;
	return first->me;
}

//FIFO

void non_preempt_fifo::accept_task(task2 * t)
{
	q.push(t);
}
task2* non_preempt_fifo::accept_create_task()
{
	auto *t = _SCALLOC(task2);
	q.push(t, true);
	return t;
}
void non_preempt_fifo::schedule()
{
	while(q.top() != nullptr && q.top()->get_state() == task2::state::CALLQUITS)q.pop();
}

__nooptimize
void non_preempt_fifo::dispatch() 
{
	auto t = q.top();
	if(t == nullptr)
	{
		term_log("[FIFO DP]No tasks for this scheduler.",LOG_WARNING);
		return;
	}
	t->set_state(task2::state::RUNNING);
	current = t;
	term_log("[FIFO DP] dispatching proc with id=",t->get_pid() ,LOG_MINOR);
	t->launch();
}
task2* non_preempt_fifo::front_task()
{
	return current;
}