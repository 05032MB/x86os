#include <mtracker.hpp>
#include <logger.hpp>
#include <kmemory.hpp>

mtracker pTracker;

/*
Important
1 means free
0 means taken
*/

template <typename T, typename S>
__nooptimize static T ceil(S s)
{
	//term_log("...", s, LOG_WARNING);
	return (s +   (((s - static_cast<T>(s) ) > 0 ) ? 1 : 0) );
}

void mtracker::init(size_t size)
{
	term_log("Tracking started: ",size, LOG_CRITICAL);
	if(size % 8 != 0)this->size = size + 8 - (size % 8);
	else this->size = size;
	_bits = reinterpret_cast<void *>( __ALLOC( size) );

	term_log("[MM:] Order is for=", size, LOG_MINOR);
	term_log("[MM:] This tracking will take bytes=",  size/ 8 , LOG_MINOR);

	for(unsigned  i=0; i < ( size/ 8 ); i++)(reinterpret_cast<byte *>(_bits))[i] = 0xFF;
}

bool mtracker::testFrame(addr_t addr)const
{
	unsigned absolute_frame = addr >> 12;
	unsigned real_frame = absolute_frame >> 3;// / 8;
	unsigned byte_offset = absolute_frame & 0x7;// % 8;

	auto _helper = reinterpret_cast<byte *> (_bits);
	return (_helper[real_frame] & (1 << byte_offset) );
}
void mtracker::setFrame(addr_t addr)
{
	unsigned absolute_frame = addr >> 12;
	unsigned real_frame = absolute_frame >> 3;// / 8;
	unsigned byte_offset = absolute_frame & 0x7;// % 8;

	auto _helper = reinterpret_cast<byte *> (_bits);
	 _helper[real_frame] |= (1 << byte_offset);
}
void mtracker::resetFrame(addr_t addr)
{
	unsigned absolute_frame = addr >> 12;
	unsigned real_frame = absolute_frame >> 3;// / 8;
	unsigned byte_offset = absolute_frame & 0x7;// % 8;

	auto _helper = reinterpret_cast<byte *> (_bits);
	 _helper[real_frame] &= ~(1 << byte_offset);
}

void* mtracker::findFreeFrame()
{
	for(unsigned i = 0; i < size / 8; i++)
	{
		auto pack = (reinterpret_cast<byte *>(_bits))[i];
		//term_log("", pack, LOG_MINOR);
		for(unsigned f = 1; pack != 0; f++ )
		{
			if( (pack & (1<<f) ) == 1){
				return voidcast(i * 8 + f - 1);
			}
		}
	}
	return nullptr;
}

//void * mtracker::findRegionOfSize

mtracker::~mtracker()
{
	__FREE(_bits);
}
