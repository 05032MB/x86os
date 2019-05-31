#include <mtracker.hpp>
#include <logger.hpp>

mtracker pTracker;

template <typename T, typename S>
static T ceil(S s)
{
	//term_log("...", s, LOG_WARNING);
	return (s +   (((s - static_cast<T>(s) ) > 0 ) ? 1 : 0) );
}

mtracker::mtracker(size_t size)
{
	term_log("Tracking started: ",size, LOG_MINOR);
	halt();
	this->size = size;
	_bits = reinterpret_cast<void *>( __ALLOC( size) );
	for(unsigned  i=0; i < ( ceil<decltype(i), double>(size/sizeof(byte) ) ); i++)(reinterpret_cast<byte *>(_bits))[i] = 0;
}

void mtracker::init(size_t size)
{
	term_log("Tracking started: ",size, LOG_CRITICAL);
	this->size = size;
	_bits = reinterpret_cast<void *>( __ALLOC( size) );

	term_log("[MM:] Order is for=", size, LOG_MINOR);
	term_log("[MM:] This tracking will take bytes=",  ceil<unsigned , double>(size/sizeof(byte) ) , LOG_MINOR);

	for(unsigned  i=0; i < ( ceil<decltype(i), double>(size/sizeof(byte) ) ); i++)(reinterpret_cast<byte *>(_bits))[i] = 0;
}

bool mtracker::testFrame(addr_t addr)const
{
	unsigned absolute_frame = addr / (4*1024);
	unsigned byteBias = ceil<decltype(byteBias), double>(absolute_frame / sizeof(byte) );
	unsigned byteOffset = absolute_frame - byteBias;

	auto _helper = reinterpret_cast<byte *> (_bits);
	return (_helper[byteBias] & (1 << byteOffset) );
}
void mtracker::setFrame(addr_t addr)
{
	unsigned absolute_frame = addr / (4*1024);
	unsigned byteBias = ceil<decltype(byteBias), double>(absolute_frame / sizeof(byte) );
	unsigned byteOffset = absolute_frame - byteBias;

	auto _helper = reinterpret_cast<byte *> (_bits);
	 _helper[byteBias] |= (1 << byteOffset);
}
void mtracker::resetFrame(addr_t addr)
{
	unsigned absolute_frame = addr / (4*1024);
	unsigned byteBias = ceil<decltype(byteBias), double>(absolute_frame / sizeof(byte) );
	unsigned byteOffset = absolute_frame - byteBias;

	auto _helper = reinterpret_cast<byte *> (_bits);
	 _helper[byteBias] &= ~(1 << byteOffset);
}

mtracker::~mtracker()
{
	__FREE(_bits);
}
