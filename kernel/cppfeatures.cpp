#include <cppfeatures.hpp>
#include <types.hpp>
#include <logger.hpp>

int __cxa_atexit(void (*func) (void *), void * arg, void * dso_handle)
{
	//will be empty until I find out how to properly implement it. For now it makes code work
	//so better pretend this abomination does not exists

}
void __cxa_pure_virtual()
{
	term_log("Odwołanie do czysto wirtualnej funkcji. Coś jest nie ok.", LOG_MINOR);
	while(1){};
	#ifdef __GNUC__
	__builtin_unreachable();
	#endif
}

static void _set_been_initialised(addr_t& guardian)
{
	guardian = guardian | 1;
}
static bool _check_if_been_initialised(addr_t& guardian)
{
	return guardian & 1;
}

static void _mark_being_used(addr_t &guardian)
{

}
static void _lift_being_used(addr_t &guardian)
{
	
}

//dodać muteksy itd, kiedy będą wątki
bool __cxa_guard_acquire(addr_t& guardian)
{
	if(_check_if_been_initialised(guardian))return false;

	_mark_being_used(guardian);
	return true;
}
void __cxa_guard_release(addr_t& guardian)
{
	_set_been_initialised(guardian);

	_lift_being_used(guardian);
}