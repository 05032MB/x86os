#include <paging.hpp>
#include <logger.hpp>
#include <kmemory.hpp>
#include <mtracker.hpp>

ALIGNED_4kB//4KiB in bytes; 4KiB aligned
dword page_directory[PTSIZE]; //4KiB /32 = 1024

ALIGNED_4kB
dword page_table_primary[PTSIZE]; //maps first 4MB, for kernel, stack

//dword *rest_of_page_tables[PTSIZE*PTSIZE - 1];

static void populate_page_directory()
{
	for(int i=0; i<PTSIZE; i++)
	{
		page_directory[i] = 2;//write enabled
	}
}

static void init_primary_page_table()
{
	//page_directory[0] = 2 + 5;//write enabled
	
	for(int i = 0; i < PTSIZE; i++)page_table_primary[i] = (i * 0x1000) | 0xB;//0xF; //supervisor, rw, present
	page_directory[0] |= ((dword)page_table_primary );
	page_directory[0] |= 0xB;//0xF;
}
void * translate_virtual_to_physical(void * virtualaddr)
{
	//unsigned long pti = (unsigned long)virtualaddr >> 12;

	//void * addr = (void *)(pt[pti] & 
}

void* init_pagedir_entry(dword* dir_entry, word flags) //creates aligned ENTRY in page directory with specified FLAGS
{
	void *pointer = (void*)sysheap.__aalloc(sizeof(dword)*PTSIZE,0x1000);
	
	*dir_entry = (dword)((to_addr_t(pointer)  & ~(0xFFF)) | flags);
	
	return pointer;

}

void init_frame(dword* tab_entry, addr_t phys_addr, word flags)
{
	phys_addr = phys_addr & ~(0xFFF); //assure it's 4KiB aligned
	
	*tab_entry = (dword)((to_addr_t(phys_addr)  & ~(0xFFF)) | flags);
	

}


void page_fault_handler(const int_iden ii)
{
	addr_t fault_loc;
	fault_loc = _get_cr2();
	
	
	
	#ifdef __k_debug
	term_log("Page Fault debug information: ", fault_loc, LOG_CRITICAL);
	kpanic("Page Fault");
	#endif //i assume that there would be no need to panic after more coding
}

void init_paging()
{
	//populate_page_directory();
	init_primary_page_table();
	

	//set_page_dir(page_directory);
	_write_cr3((dword)&page_directory);
	_write_cr0(0x80000000 | _get_cr0());

	/*for(int i = 1; i < PTSIZE; i++)
	{
		rest_of_page_tables[i] = nullptr;
	} */

}

void set_page_dir(page_dir_t *pg)
{
	_write_cr3(to_addr_t(pg) );
	_write_cr0(0x80000000 | _get_cr0());
}

__nooptimize
static void poke_addr(addr_t addr)
{
	int* pointer = (int*)addr;
	//dputs((unsigned int)pointer);
	int bckp = *pointer;
	*pointer = 5;
	*pointer = bckp;

}

/*void map_page(addr_t virt, addr_t phys, word flags)
{
	auto entr = virt / (PTSIZE*PTSIZE);
	auto off = virt % (PTSIZE*PTSIZE);
	if(page_directory[entr] == nullptr)
	{
		init_pagedir_entry(&page_directory[i], flags);
	}
	if(rest_of_page_tables[entr + off] == nullptr)
	{
		rest_of_page_tables[i] = reinterpret_cast<dword *>(__AALLOC(dword, 0x1000));
	}
	//init_frame(&rest_of_page_tables)

}*/

void init_paging_phase_2()
{	

	/*dword* page_table_2 = (dword*)init_pagedir_entry(&page_directory[1], 0xB);
	init_frame(&page_table_2[0], 0x400000 , 0xB); */

	dword* page_table_2 = (dword*)init_pagedir_entry(&page_directory[1], 0xF);//krnl
	for(int i =0; i<1024; i++)init_frame(&page_table_2[i], 0x400000 + i*4*1024 , 0xB); //[4MB;8MB), system memory
	
	dword* page_table_3 = (dword*)init_pagedir_entry(&page_directory[2], 0xF);//user
	for(int i =0; i<1024; i++)init_frame(&page_table_3[i], 0xA00000 + i*4*1024 , 0xF); //[10MB;14MB), user memory
	
	dword* page_table_33 = (dword*)init_pagedir_entry(&page_directory[32], 0xF);
	for(int i =0; i<1024; i++)init_frame(&page_table_33[i], 0x8048000 + i*4*1024, 0xF); //128 meg
	/*poke_addr(0xA00001);//ok
	poke_addr(0x400000);//ok
	poke_addr(0x799999);
	poke_addr(0x800001);//?
	poke_addr(0xA00000-0x2);//?
	//poke_addr(0xFFFFFFF);*/
	/*pTracker.init(PTSIZE*PTSIZE);
	term_log("MM tracker initialized.",LOG_OK);

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