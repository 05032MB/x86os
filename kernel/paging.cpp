#include <paging.hpp>
#include <logger.hpp>
#include <kmemory.hpp>
#include <mtracker.hpp>

ALIGNED_4kB//4KiB in bytes; 4KiB aligned
dword page_directory[PTSIZE]; //4KiB /32 = 1024

ALIGNED_4kB
dword page_table_primary[PTSIZE]; //maps first 4MB, for kernel, stack

ALIGNED_4kB
dword * current_directory;

//assembly functions
__ASM_IMPORT{
 dword _get_cr0(void);
 dword _get_cr3(void);
 dword _get_cr2(void);
 
 //Flushes tlb buffers, needs to be called after page changes
 void _reload_tlbs(void);

 void __fastcall _write_cr0(dword);
 void __fastcall _write_cr3(dword);
 
}


/*
Purpose: Create page-aligned entry in page directory (new page table), or assigns existing one, with specified flags
Params:
	->dir_entry - the page directory entry, where to put a new page table
	->flags - flags for this entry
	->pointer - pointer to existing page table, if set to nullptr, it will allocate a new one on the system heap
Return: pointer to the new structure
*/
void* init_pagedir_entry(dword* dir_entry, word flags, void * pointer = nullptr) //creates aligned ENTRY in page directory (new page table) with specified FLAGS
{
	if(pointer == nullptr)pointer = voidcast(__AALLOC(sizeof(dword)*PTSIZE,0x1000));
	
	*dir_entry = (dword)((to_addr_t(pointer)  & ~(0xFFF)) | flags);
	
	return pointer;
}
/*
Purpose: destroy (remove from heap), a page dir entry (page table)
Params: pointer to the entry (not table itself)
Return: pointer to the destroyed table
*/
void * destroy_pagedir_entry(dword * dir_entry)
{
	__FREE(voidcast(ONLY_ADDR(*dir_entry)));
	auto ret = voidcast(ONLY_ADDR(*dir_entry));
	*dir_entry = 0;
	return ret;
}

void init_frame(dword* tab_entry, addr_t phys_addr, word flags)
{
	phys_addr = phys_addr & ~(0xFFF); //assure it's 4KiB aligned
	
	*tab_entry = (dword)((to_addr_t(phys_addr)  & ~(0xFFF)) | flags);
}
void init_primary_page_table()
{
	for(int i = 0; i < PTSIZE; i++)init_frame(&page_table_primary[i], 0 + i * 0x1000, ENTR_PRESENT(1) | ENTR_RW(1) | ENTR_USER(0) | ENTR_WRITETHROUGH(1) );
	init_pagedir_entry(&page_directory[0], ENTR_PRESENT(1) | ENTR_RW(1) | ENTR_USER(0) | ENTR_WRITETHROUGH(1), page_table_primary);
}
void * translate_virtual_to_physical(dword virtualaddr)
{
	dword iab = (virtualaddr >> 12); //dzielę na bloki po 4kB

	dword tabl = iab >> 10;// /1024;

	dword exar = ONLY_ADDR(page_directory[tabl]); //address of the table 2
/*	term_print_hex(iab);
	term_print_hex(iab / 1024);
	term_print_hex(iab % 1024);*/
	if(exar == 0)
	{
		return nullptr;
	}
	dword sptable = (reinterpret_cast<dword *>(exar))[iab & 0x3FF]; //(mod 1024) searched page table
	return ONLY_ADDR(sptable) ? reinterpret_cast<void *>(ONLY_ADDR(sptable) + (virtualaddr & 0xFFF)) : nullptr;
}

dword * clone_ptable(dword * pt)
{
	size_t size = PTSIZE;

	term_log("[PG]Cloning table of size=",size,LOG_MINOR);

	auto npt = reinterpret_cast<dword *>(__AALLOC(sizeof(dword) * size, 0x1000));

	term_log_hex("[PG]Allocated clone at=", to_addr_t(npt), LOG_MINOR);

	for(size_t i = 0; i < size; i++)
	{
			npt[i] = pt[i];
	}
	return npt;
}

dword * clone_pdir(dword * pd)
{
	size_t size = PTSIZE;

	term_log("[PG]Cloning dir of size=",size,LOG_MINOR);

	auto npd = reinterpret_cast<dword*>(__AALLOC(sizeof(dword)*size, 0x1000));

	term_log_hex("[PG]Allocated clone at=", to_addr_t(npd), LOG_MINOR);

	for(size_t i = 0; i < size; i++)
	{
		if(pd[i] != 0){
			npd[i] = ONLY_DATA(pd[i]);
			npd[i] |= to_addr_t(clone_ptable(reinterpret_cast<dword*>(ONLY_ADDR(pd[i])))), LOG_WARNING;
		}else{
			npd[i] = 0;
		}
	}
	return npd;
}

void reset_global_pdir()
{
	set_page_dir(page_directory);
}

void page_fault_handler(const int_iden ii)
{
	addr_t fault_loc = _get_cr2();
	bool p0 = ii.e_code & 1<<0; //present flag is 0
	bool w0 = ii.e_code & 1<<1; //written to non-writable page. if 0 read is a problem
	bool u0 = ii.e_code & 1<<2; //was in user mode. if 0 was in kernel mode
	bool r0 = ii.e_code & 1<<3; //malformed entry in paging structure - manipulation of reserved bits
	bool f0 = ii.e_code & 1<<4; //fault caused by instruction fetch.
	
	

	term_log("---Page Fault debug information.---", LOG_CRITICAL);
	term_log("Location: ", fault_loc, LOG_CRITICAL);

	if(p0)term_log("Page was present (but still failed).",LOG_CRITICAL);
	else term_log("Page not was present.",LOG_CRITICAL);

	if(w0)term_log("Write operation not permitted.",LOG_CRITICAL);
	else term_log("Read operation not permitted", LOG_CRITICAL);

	if(u0)term_log("Fault occurred in user mode.", LOG_CRITICAL);
	else term_log("Fault occurred in kernel mode.", LOG_CRITICAL);

	if(r0)term_log("Malformed page description.",LOG_CRITICAL);

	if(f0)term_log("Fault during instruction fetch.",LOG_CRITICAL);

	#ifdef __k_debug
	kpanic("Page Fault");
	#endif //i assume that there would be no need to panic after more coding
}

void set_page_dir(void *pg)
{
	_write_cr3(to_addr_t(pg) );

	current_directory = reinterpret_cast<dword*>(pg);
}

void init_paging()
{
	init_primary_page_table();
	
	set_page_dir(page_directory);
	_write_cr0(0x80000000 | _get_cr0());

}

__nooptimize __used
static void poke_addr(addr_t addr)
{
	int* pointer = (int*)addr;
	//dputs((unsigned int)pointer);
	int bckp = *pointer;
	*pointer = 5;
	*pointer = bckp;

}
/*
Maps virtual address to physical address with flags. 
Can specify page directory, if not will chose kernel directory.
Can specify memory tracker, or nullptr for no tracker
Can override by setting override.

Behaviour:
With override not set, it will not remap virtual address if it's already mapped somewhere.
If page dir entry doesn't exist it will create one on the stack.
Returns true on success, else false.
Note that the function WILL NOT flush TLB buffers.
*/
bool map_page(addr_t virt, addr_t phys, word flags, dword * page_directory = current_directory, mtracker *mt = nullptr, bool override = false)
{
	if(translate_virtual_to_physical(virt) != nullptr && override == false) return false; //na pewno nie?

	dword tabe = (virt >> 12) >> 10;
	dword tabx = (virt >> 12) & 0x3FF;

	if(ONLY_ADDR(page_directory[tabe]) == 0)
	{
		auto ntable = reinterpret_cast<dword *>( init_pagedir_entry(&page_directory[tabe], flags) );
		init_frame(&ntable[tabx], phys , flags);
	}else{
		init_frame( &( reinterpret_cast<dword*>(ONLY_ADDR(page_directory[tabe])) )[tabx], phys, flags);
	}

	if(mt != nullptr)mt->resetFrame(phys);

	term_log("[PG] We mapped virtual=", virt, LOG_MINOR);
	term_log("[PG] to=", phys, LOG_MINOR);

	return true;

}
/*
Allocates frames for memory region starting at virtual address of specified size and flags
This one requires mem tracker in parameters
Note that if function fails it will not roll back already made changes, you will have to do it yourself
Note that on success the function WILL flush TLB buffers
Return: number of mapped bytes
*/
size_t memmap(addr_t virt, size_t size, word flags, mtracker *mt, dword * page_directory = current_directory, bool override = false)
{
	if(mt == nullptr)return 0;

	for(dword i = 0; i < size; i += 0x1000){
		auto addr = mt->findFreeFrame();
		
		if(addr == nullptr)
		{
			term_log("[MEMMAP] All frames are full", LOG_WARNING);
			return i;
		}

		mt->resetFrame(to_addr_t(addr));
		if(!map_page(virt, to_addr_t(addr), flags, page_directory, mt, override))return false;
	}

	_reload_tlbs();

	return size;
}

size_t fmemmap(addr_t virt, size_t size, word flags, mtracker *mt, dword * page_directory)
{
	if(page_directory == nullptr)return memmap(virt, size, flags, mt, current_directory, true);
	else return memmap(virt, size, flags, mt, page_directory, true);
}
void init_paging_phase_2()
{	

	/*dword* page_table_2 = (dword*)init_pagedir_entry(&page_directory[1], 0xB);
	init_frame(&page_table_2[0], 0x400000 , 0xB); */

	pTracker.init(PTSIZE*PTSIZE);
	term_log("MM tracker initialized.",LOG_OK);
	for(unsigned i = 0; i < 0x400000; i++)pTracker.resetFrame(i);

	dword* page_table_2 = (dword*)init_pagedir_entry(&page_directory[1], 0xF);//krnl
	for(int i =0; i<1024; i++){
		init_frame(&page_table_2[i], 0x400000 + i*4*1024 , 0xB); //[4MB;8MB), system memory
		pTracker.resetFrame(0x400000 + i*4*1024);
	}
	
	dword* page_table_3 = (dword*)init_pagedir_entry(&page_directory[2], 0xF);//user
	for(int i =0; i<1024; i++)init_frame(&page_table_3[i], 0xA00000 + i*4*1024 , 0xF); //[10MB;14MB), user memory
	

	//dword* page_table_33 = (dword*)init_pagedir_entry(&page_directory[32], 0xF);
	//for(int i =0; i<1024; i++)init_frame(&page_table_33[i], 0x8048000 + i*4*1024, 0xF); //128 meg
	
	//term_log_hex("0xA00001->",(dword)translate_virtual_to_physical(0xA00001),LOG_CRITICAL);
	//asm("hlt ;");
	//asm("hlt ;");
	/*
	if(!memres(0, 0xE00000))
	{
		term_log("Kernel mem reservation failed.", LOG_CRITICAL);
		kpanic("No sorry.");
	}
	term_log("Memory for kernel reserved, up to: ", 0xE00000, LOG_OK);*/

}

bool memres(addr_t lo, addr_t hi)
{
	for(size_t i = lo; i <= hi; i++ )
		if(pTracker.testFrame(i)){
			term_log("[MEMRES:] Failed to reserve: ", i, LOG_MINOR);
			return false;
		}
	for(size_t i = lo; i <= hi; i++ ){
		pTracker.setFrame(i);
		//init_frame()
	}
	return true;
}