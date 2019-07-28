#include <task.hpp>
#include <logger.hpp>
#include <paging.hpp>
#include <elf.hpp>
#include <kheap.hpp>
#include <critical.hpp>

/*switches rings and jumps to the pointer*/
__ASM_IMPORT __fastcall void switch_to_ring_3(void(*)());
/*same as above, but the second argument is a new stack location*/
__ASM_IMPORT __fastcall void _jump_as_if_userspace_task(addr_t, addr_t);
__ASM_IMPORT void halt();

static dword top_pid = 1;

task2::task2()
{
	tinfo = _SCALLOC(task_info);
}
task2::task2(task_info * tf)
{
	this->tinfo = tf;
}
task2::~task2()
{
	__FREE(voidcast(tinfo));
}

byte task2::prepare_task_from_elf(void * elf, void* sh, void* hl, dword * pd)
{
	if(tinfo == nullptr){
		tinfo = _SCALLOC(task_info);
	}

	dword * pclone = clone_pdir(pd);

	this->tinfo->pid = top_pid++;
	this->tinfo->page_dir = pclone;

	auto entr = ELF::load_elf(reinterpret_cast<ELF::ELF32_Header *> (to_addr_t(elf)), pclone );
	this->tinfo->entry = to_addr_t(entr);

	this->tinfo->stack_high = to_addr_t(sh);
	
	if(entr == nullptr)return 1;
	return 0;

}
void task2::launch()
{
	set_page_dir(this->tinfo->page_dir);
	_jump_as_if_userspace_task(this->tinfo->entry, this->tinfo->stack_high);
}
void task2::suspend(const task_info::reg_t &rgs)
{
	tinfo->regs = rgs;
}
void task2::set_state(task2::state s)
{
	proc_state = s;
}
task2::state task2::get_state()const
{
	return proc_state;
}

task2 only_task;

///////////////

void task2::exit(byte ecode)
{
	set_state(state::ENDING);
	//destroy_aspace(this->tinfo->page_dir);
	//close files
	//close streams etc.
	set_state(state::CALLQUITS);
}

void task2::yield()
{
	term_log("Yield test call", 1,LOG_MINOR);
	halt();
}