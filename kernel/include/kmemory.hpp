#ifndef _KMEMORY_H
#define _KMEMORY_H

#include <types.hpp>

//#define to_addr_t(x) (addr_t)x

template<typename T>
sword less_than(T a, T b);

template<typename T>
class heaparray{
	
	public:
	typedef sword (*comparator_func_t)(T, T);
	
	private:
	size_t size;
	size_t maxsize;
	T* pointer;
	comparator_func_t standard_comparator; //comparator
	

	static void swap(T&, T&);
	
	public:
	heaparray(size_t maxsize, comparator_func_t comparator_method); //alloc exists (not implemented)
	heaparray(T* addr, size_t maxsize, comparator_func_t comparator_method); //no alloc
	heaparray(){};
	
	void init_no_alloc(T* addr, size_t maxsize, comparator_func_t comparator_method); 
	
	bool insert(T sth);
	T lookup(size_t element);
	T remove(size_t element);
	
	size_t get_size(){return this->size;}
	size_t get_max_size(){return this->maxsize;}
	
	~heaparray();

};

class heap{
	void * begheap;
	void * endheap; //unused
	void * currheap;
	
	public:
	 struct block{
		size_t size;
		void* address;
	
	}__packed;
	
	private:
	heaparray<block> blocks_empty;	
	heaparray<block> blocks_full;	

	bool _lock = true;
	
	#define MINSPLITSIZE 1
	
	static sword less_than_heap(block a, block b){ //increasing order
			return a.size > b.size ? 1 : 0;
	}
	size_t maxsize;	
	
	bool add_block(size_t s, addr_t address, bool empty);
	bool move_block_to_empty(size_t num);
	bool move_block_to_full(size_t num);
	heap::block find_by_address(addr_t address);
	
	bool destroy_block(size_t num, bool empty);
	
	public:
	
	heap(addr_t, size_t);
	heap() {}; //nothing
	
	void *__alloc(size_t size);
	void *__aalloc(size_t size, size_t align); //align alloc
	void __dealloc(void* ptr);
	void __realloc(void* ptr, size_t size); //not yet implemented
	
	size_t get_heap_size(){return to_addr_t(currheap)-to_addr_t(begheap);};
	
	void free(void* ptr);

};
/*
Wywalić ten szit.
Prawdziwy malloc niżej.
*/
void * kalloc(void* a,size_t size, bool align, addr_t);

/*
Only legit internal malloc working without system heap
Params:
->Target Address
->Size of gap
->Align to lower page boundary?
Returns:
->start address (return)
->end address (fin)

*/
addr_t ubermalloc(addr_t where, size_t size, bool p_align, addr_t *fin );

void init_heap(void);

extern heap sysheap;

#define __FREE(s) sysheap.__dealloc(s)

#define __ALLOC(s) sysheap.__alloc(s) //alloc macro
#define __SALLOC(obj) __ALLOC(sizeof(obj))	//simple alloc macro
#define __SCALLOC(obj, obj2) reinterpret_cast<obj2>(__SALLOC(obj))//simple casting allocation
#define _SCALLOC(obj) reinterpret_cast<obj*>(__SALLOC(obj))//even simpler casting allocation (so only one underscore)

extern void* endkernel; //from linkerscript, address of kernel ending
extern void* begkernel; //from linkerscript, address of kernel beginning
extern void* prekernel; //0,propably

#endif