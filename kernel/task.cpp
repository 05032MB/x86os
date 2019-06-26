#include <task.hpp>
#include <logger.hpp>
#include <paging.hpp>
#include <elf.hpp>

/*switches rings and jumps to the pointer*/
__ASM_IMPORT __fastcall void switch_to_ring_3(void(*)());
/*same as above, but the second argument is a new stack location*/
__ASM_IMPORT __fastcall void _jump_as_if_userspace_task(addr_t, addr_t);
__ASM_IMPORT void halt();

byte task2::prepare_task_from_elf(void * elf, void* sh, void* hl, dword * pd)
{
	dword * pclone = clone_pdir(pd);

	this->tinfo->pid = top_pid++;
	this->tinfo->page_dir = pclone;

	auto entr = ELF::load_elf(reinterpret_cast<ELF::ELF32_Header *> (to_addr_t(elf)), pclone );
	this->tinfo->entry = to_addr_t(entr);

	this->tinfo->stack_high = to_addr_t(sh);

	return 0;

}
void task2::launch()
{
	set_page_dir(this->tinfo->page_dir);
	_jump_as_if_userspace_task(this->tinfo->entry, this->tinfo->stack_high);
}
void task2::suspend()
{
	
}

task2 only_task;

///////////////

__nooptimize
void task2::yield()
{
	term_log("Yield test call", 1,LOG_CRITICAL);
	halt();
}