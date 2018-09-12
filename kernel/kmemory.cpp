#include <kmemory.h>

ALIGNED_4kB//4KiB in bytes; 4KiB aligned
dword page_directory[PTSIZE]; //4KiB /32 = 1024

ALIGNED_4kB
dword page_table_primary[PTSIZE];


static void populate_page_directory()
{
	for(int i=0; i<PTSIZE; i++)
	{
		page_directory[i] = 2;//write enabled
	}
}

static void init_primary_page_table()
{
	for(int i = 0; i < PTSIZE; i++)page_table_primary[i] = (i * 0x1000) | 3; //supervisor, rw, present
	//page_directory[0] = ((dword)page_table_primary) |3; //+super, present
	page_directory[0] |= ((dword)page_table_primary );
	page_directory[0] |= 3;
}

void init_paging()
{
	populate_page_directory();
	init_primary_page_table();
	
	_write_cr3((dword)&page_directory);
	_write_cr0(0x80000000 | _get_cr0());

}