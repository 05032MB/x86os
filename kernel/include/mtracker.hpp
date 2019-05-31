#ifndef _MTRACKER_H_
#define _MTRACKER_H_

#include <types.hpp>
#include <paging.hpp>

class mtracker{
	void *_bits;
	size_t size;

public:
	mtracker(size_t size) ;//= delete;
	mtracker() = default;
	~mtracker();

	size_t getSize()const{return this->size;}

	void init(size_t size);
	bool testFrame(addr_t addr)const;
	void resetFrame(addr_t addr);
	void setFrame(addr_t addr);

	addr_t firstFreeFrame();

};//pTracker;

extern mtracker pTracker;

#endif