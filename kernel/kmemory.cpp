#include <kmemory.hpp>

#ifdef __k_debug
#include <stdio.h>
#include <stdlib.h>
__attribute__((used))
static void dputs(int a)
{
	char tr[50];
	itoa(a, tr, 10);
	puts(tr);
}
#endif

static void* memset(void* bufptr, int value, size_t size) {
	unsigned char* buf = (unsigned char*) bufptr;
	for (size_t i = 0; i < size; i++)
		buf[i] = (unsigned char) value;
	return bufptr;
}
//////heaparray////////
template<typename T>
void heaparray<T>::swap(T& a, T& b)
{
	T p = a;
	a = b;
	b = p;
}

template<typename T>
static sword less_than(T a, T b)
{
	return a > b ? 1 : 0;
}
template<typename T>
heaparray<T>::heaparray(T* addr, size_t maxsize, comparator_func_t comparator_method)
{
	this->init_no_alloc(addr, maxsize, comparator_method); 
}
template<typename T>
void heaparray<T>::init_no_alloc(T* addr, size_t maxsize, comparator_func_t comparator_method)
{
	memset(addr, 0, maxsize*sizeof(T));
	this->pointer = addr;
	this->size = 0;
	this->maxsize = maxsize;
	this->standard_comparator = comparator_method;
}
template<typename T>
bool heaparray<T>::insert(T elem)
{
	//assert(this->comparator_method);
	if(size+1 > maxsize)return false;//kpanic("System Heap Array larger than declared! (heaparray)");
	
	size_t iterator = 0;
	
	while(iterator < this->size && this->standard_comparator(elem, pointer[iterator]))
	{
		iterator++;
	}		
	if(iterator == this->size)pointer[iterator] = elem;
	while(iterator < this->size+1)
	{

		this->swap(pointer[iterator], elem);
		iterator++;
	}
	this->size++;
	return true;
}
template<typename T>
T heaparray<T>::lookup(size_t index)
{
	if(index >= this->size)kpanic("This system is not supporting exceptions, so it panics");//__asm__("ret");what was that
	return this->pointer[index];
}
template<typename T>
T heaparray<T>::remove(size_t index)
{
	T pom = this->pointer[index];
	while(index++ < this->size)
	{
		swap(this->pointer[index-1], this->pointer[index]);
	}
	this->size--;
	return pom;

}
template<typename T>
heaparray<T>::~heaparray()
{

}

////alloc inc.///

heap::heap(addr_t a, size_t s){
	this->blocks_empty.init_no_alloc((heap::block *)a, s, &heap::less_than_heap);
	a = (a+sizeof(heaparray<heap::block>)*s);	
	this->blocks_full.init_no_alloc((heap::block *)a, s, &heap::less_than_heap);
	begheap = (void*)(a+sizeof(heaparray<heap::block>)*s);
	currheap = begheap;
}
bool heap::add_block(size_t s, addr_t address, bool empty)
{
	if(address < to_addr_t(begheap))return false ;
	if(s == 0)return false;
	heap::block temp;
	temp.size = s;
	temp.address = (void*)address;
	//temp.empty = empty;
	if(empty)blocks_empty.insert(temp);
	else blocks_full.insert(temp);
	
	return true;
}
bool heap::move_block_to_empty(size_t num)
{
	if(num >= blocks_full.get_size())return false;
	
	return blocks_empty.insert(blocks_full.remove(num));
}
bool heap::move_block_to_full(size_t num)
{
	if(num >= blocks_empty.get_size())return false;
	
	return blocks_full.insert(blocks_empty.remove(num));
}
bool heap::destroy_block(size_t num, bool empty)
{
	if(empty)
	{
		if(num >= blocks_empty.get_size())return false;
		blocks_empty.remove(num);
		return true;
	}
	
	if(num >= blocks_full.get_size())return false;
	blocks_full.remove(num);
	return true;

}

void heap::free(void* ptr)
{
	this->__dealloc(ptr);
}


#pragma GCC diagnostic ignored "-Wpointer-arith" //until better solution is found
void * heap::__alloc(size_t size)
{
	for(size_t i =0; i<blocks_empty.get_size(); i++)
	{
		if(blocks_empty.lookup(i).size >= size)
		{
			heap::block temp = blocks_empty.lookup(i);
			
			//dputs(temp.size - size);
			if(temp.size - size >= MINSPLITSIZE)
			{
				destroy_block(i, true);
				add_block(size, to_addr_t(temp.address), false);
				add_block(temp.size-size, to_addr_t(temp.address)+size, true);
			}else move_block_to_full(i);
			
		
			
			return temp.address;
		}
	}
	
	add_block(size, to_addr_t(currheap), false);
	addr_t ret = to_addr_t(currheap);
	currheap = (void*) size+to_addr_t(currheap);
	return (void*)ret;
}

void * heap::__aalloc(size_t size, size_t alignment) //only works with 2^n alignment
{
	for(size_t i =0; i<blocks_empty.get_size(); i++)
	{
		if(blocks_empty.lookup(i).size >= size && (to_addr_t(blocks_empty.lookup(i).address)%alignment == 0))
		{
			heap::block temp = blocks_empty.lookup(i);
			
			if(temp.size - size >= MINSPLITSIZE)
			{
				destroy_block(i, true);
				add_block(size, to_addr_t(temp.address), false);
				add_block(temp.size-size, to_addr_t(temp.address)+size, true);
			}else move_block_to_full(i);
			
			return temp.address;
		}
	}
	
	addr_t nearest_aligned = to_addr_t(currheap) & ~(alignment-1);
	
	while(nearest_aligned<=to_addr_t(currheap)){nearest_aligned += alignment;}
	
	add_block(nearest_aligned - to_addr_t(currheap), to_addr_t(currheap) ,true);
	add_block(size, nearest_aligned, false);
	addr_t ret = to_addr_t(nearest_aligned);
	currheap = (void*) size+to_addr_t(nearest_aligned);
	
	return (void*)ret;
}


void heap::__dealloc(void * ptr)
{

 	for(size_t i=0; i<blocks_full.get_size(); i++)
	{
		if(to_addr_t(blocks_full.lookup(i).address) == to_addr_t(ptr))
		{
			heap::block temp = blocks_full.lookup(i);
			addr_t nextaddr = to_addr_t(temp.address) + temp.size;
			
			bool merged = false;
			
			for(size_t z = 0; z<blocks_empty.get_size();z++)
			{
				if(to_addr_t(blocks_empty.lookup(z).address) == nextaddr){
					heap::block tmp2 = blocks_empty.lookup(z);
				
					destroy_block(z, true);
					add_block(temp.size + tmp2.size, to_addr_t(temp.address), true);
					
					merged = true;
					
					break;
				}
			} 
			
			if(!merged)this->move_block_to_empty(i);
			//find out how to efficintly do the same on 'left' side
			
			return;
		}
	}

}

///////////////
void * kalloc(void* currheap,size_t size, bool align, addr_t begheap) //dangerous alloc
{	
	if(align)//align to begheap (recommended to page-align it)
	{
		currheap = (void*)((addr_t)currheap & (addr_t)begheap);
		currheap += 0x1000; //page size
	}
	memset(currheap, 0, size);
	return currheap;
}
#pragma GCC diagnostic pop

heap sysheap;
	
void init_heap()
{
	sysheap = heap(to_addr_t(endkernel),300); //start address, max blocks amount	

	/*int *ptr3 = (int*)sysheap.__alloc(8);
	int *ptr = (int*)sysheap.__aalloc(8,1024);
	int *ptr2 = (int*)sysheap.__alloc(8);
	sysheap.__dealloc(ptr2);
	ptr2 = (int*)sysheap.__alloc(100);
	int *ptr4 = (int*)sysheap.__alloc(8);

	dputs(to_addr_t(ptr3));
	dputs(to_addr_t(ptr));
	dputs(to_addr_t(ptr2));
	dputs(to_addr_t(ptr4));*/

}