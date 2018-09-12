#ifndef _KMEMORY_H
#define _KMEMORY_H

#include <types.h>
#include <critical.h>

#define PTSIZE 1024

#define ALIGNED_4kB __attribute__((aligned(4096)))
#define ALIGNED_4MB __attribute__((aligned(4194304)))

void init_paging();

//----------------------
	/*Page table ebtry structure explained
	-----------------------------------------------------------------------------------------------------------------------------------------
    | 31-12|                                  |11...9		 |8		|7  |6		   |5	    |4	        |3			 |2			  	 |1		    |0	    |bits
    |Address of frame (lower 12 bits set to 0)|Bits for me :)|Global|0  |DirtyBit  |Accessed|DisablCache|WriteThrough|Supervisor/User|Read/Write|Present|function
	-----------------------------------------------------------------------------------------------------------------------------------------
	*/
#define ENTR_GLOBAL(x) x<<8
#define ENTR_DIRTY(x) x<<6	


	/*Page directory entry structure explained
	-----------------------------------------------------------------------------------------------------------------------------------------------------------
    | 31-12|                                  |11...9		 |8			|7		   |6	|5		 |4	         |3			  |2		      |1		 |0	     |bits
    |Address of frame (lower 12 bits set to 0)|Bits for me :)|Ignored   |PageSize  |0	|Accessed|DisablCache|WriteThrough|User/Supervisor|Read/Write|Present|function
	-----------------------------------------------------------------------------------------------------------------------------------------------------------
	set 7 to 1 to align 4MiB, otherwise it's 4KiB aligned
	*/
#define ENTR_SIZE(x) x<<7
#define ENTR_ACCESSED(x) x<<5
#define ENTR_CACHE(x) x<<4
#define ENTR_WRITETHROUGH(x) x<<3
#define ENTR_USER(x) x<<2
#define ENTR_RW(x) x<<1
#define ENTR_PRESENT(x) x
//----------------


//---------------
extern ALIGNED_4kB dword page_directory[PTSIZE];
extern ALIGNED_4kB dword page_table_primary[PTSIZE];

bool set_page_dir(dword);

//assembly functions
extern "C"{
 dword _get_cr0(void);
 dword _get_cr3(void);
 
 void __attribute__((fastcall)) _write_cr0(dword);
 void __attribute__((fastcall)) _write_cr3(dword);
 
}

#endif