#ifndef _KHEAP_H
#define _KHEAP_H

#include <logger.hpp>
#include <critical.hpp>

#define KHEAP_DEBUG

#ifdef __GNUC__ //silence irritating warnings
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#pragma GCC diagnostic ignored "-Wpointer-arith"
#endif

template<class T>
struct remove_const
{
	using type = T;
};

template<class T>
struct remove_const<const T>
{
	using type = T;
};

template<class T>
using remove_const_t = typename remove_const<T>::type;

template<class T>
T* upper_bound(T* first, T* last, const remove_const_t<T>& value)
{
    T* it;
    size_t count, step;
    count = last - first;
 
    while (count > 0) {
        it = first; 
        step = count / 2; 
        it += step;
        if (!(value < *it)) {
            first = ++it;
            count -= step + 1;
        } 
        else
            count = step;
    }
    return first;
}

template<class T>
T* lower_bound(T* first, T* last, const remove_const_t<T>& value)
{
    T* it;
    size_t count, step;
    count = last - first;
 
    while (count > 0) {
        it = first; 
        step = count / 2; 
        it += step;
        if (*it < value) {
            first = ++it;
            count -= step + 1;
        } 
        else
            count = step;
    }
    return first;
}

template<class T>
inline T gt_eq_2_power(const T& t)
{
	T res = 1;
	while (res < t)
		res *= 2;
	return res;
}

inline void fill(byte* first, byte* last, const byte& value)
{
    for (; first != last; ++first)
        *first = value;
}

template<class T>
void mem_forward(T* begin, T* end)
{
	for (T* it = end; it != begin; it--)
	{
		*it=*(it-1);
	}
}

template<class T>
void mem_backward(T* begin, T* end)
{
	for (T* it = begin; it != end; it++)
	{
		*(it-1)=*it;
	}
}

template <class T, int N>
class static_sorted_array
{
public:
	using iterator = T*;
	static_sorted_array()
		: _begin(_storage), _end(_storage) {}
	void insert(const T& t)
	{
		if (_end - _begin == N)
			kpanic("static_sorted_array overflow");
		iterator it = upper_bound(_begin, _end, t);
		mem_forward(it, _end);
		_end++;
		*it = t;
	}
	void erase(const T& t)
	{
		mem_backward(upper_bound(_begin, _end, t), _end);
		_end--;
	}
	//USUSED
	/*bool contains(const T& t)
	{
		iterator it = lower_bound(_begin, _end, t);
		return it != _end && *it == t;
	}*/
	const iterator begin() { return _begin; }
	const iterator end() { return _end; }
	
	private:
	T _storage[N];
	iterator _begin;
	iterator _end;
};

template <class T>
using static_sorted_array_1000 = static_sorted_array<T, 1000>;

template <template <class> class multiset_type>
class kheap
{
	public:
	kheap(void* begin = nullptr, size_t length = 0u)
		: _begin(begin), _end(begin + length)
	{
		empty_spaces.insert({begin, length});
		#ifdef KHEAP_DEBUG
		term_log("kheap begin:  ", (int)begin, LOG_MINOR);
		term_log("kheap length: ", length, LOG_MINOR);
		#endif
	}
	class length_entry
	{
		public:
		bool operator<(const length_entry& rhs) const
		{
			return length<rhs.length;
		}
		void* ptr;
		size_t length;
	};
	class ptr_entry
	{
		public:
		bool operator<(const ptr_entry& rhs) const
		{
			return ptr<rhs.ptr;
		}
		void* ptr;
		size_t length;
	};
	void* create_buddies(size_t length, length_entry e)
	{
		if (e.length/2 >= length)
		{
			term_log("buddy: ", e.length/2, LOG_MINOR);
			empty_spaces.erase(e);
			//buddy of e
			empty_spaces.insert({e.ptr + e.length/2, e.length/2});
			//new e
			e.length = e.length/2;
			empty_spaces.insert(e);
			return create_buddies(length, e);
		}
		empty_spaces.erase(e);
		not_empty_spaces.insert({e.ptr, e.length});
		fill(static_cast<byte*>(e.ptr), static_cast<byte*>(e.ptr + e.length), 0);
		return e.ptr;
	}
	void* alloc(size_t length)
	{
		//  \_^-^_/
		typename multiset_type<length_entry>::iterator it;
		it = lower_bound(empty_spaces.begin(), empty_spaces.end(), {nullptr, length});
		#ifdef KHEAP_DEBUG
		term_log("requested: ", length, LOG_MINOR);
		#endif
		if (it == empty_spaces.end())
		{
			term_log("requested: ", length, LOG_CRITICAL);
			trace();
			kpanic("kheap overflow");
		}
		return create_buddies(length, *it);
	}
	void* alloc(size_t length, size_t alignment)
	{
		// alignment must be power of 2
		while (length > alignment)
			alignment *= 2;
		#ifdef KHEAP_DEBUG
		term_log("alignment: ", alignment, LOG_MINOR);
		#endif
		return alloc(alignment);
	}
	void free(void* ptr)
	{
		ptr_entry e = *lower_bound(not_empty_spaces.begin(), not_empty_spaces.end(), {ptr, 0});
		not_empty_spaces.erase(e);
		#ifdef KHEAP_DEBUG
		term_log("freed: ", e.length, LOG_MINOR);
		#endif
		empty_spaces.insert({e.ptr, e.length});
	}
	void trace()
	{
		term_log("Empty:\n", LOG_NORMAL);
		for(auto it = empty_spaces.begin(); it != empty_spaces.end(); it++)
		{
            term_log("   ptr:\n", (int)it->ptr, LOG_NORMAL);
            term_log("   length:\n", it->length, LOG_NORMAL);
		}
        term_log("Not empty:\n", LOG_NORMAL);
		for(auto it = not_empty_spaces.begin(); it != not_empty_spaces.end(); it++)
		{
			term_log("   ptr:\n", (int)it->ptr, LOG_NORMAL);
            term_log("   length:\n", it->length, LOG_NORMAL);
		}
	}

	void* begin() { return _begin; }
	void* end() { return _end; }

	private:
	multiset_type<length_entry> empty_spaces;
	multiset_type<ptr_entry> not_empty_spaces;
	void* _begin;
	void* _end;
};

using kheap_default = kheap<static_sorted_array_1000>;
extern kheap_default kheap_instance;

// It is possible to make kheap constructor private,
// and this function can be it's friend.

inline void init_heap(addr_t begin, size_t length = 1048576u * 2u)
{
// WARNING!
// TODO: ???
#warning begin = 1048576u * 2u;
	begin = 1048576u * 2u;
	kheap_instance = kheap_default((void*)begin, length);
}

#define __FREE(s) kheap_instance.free(s)
#define __AALLOC(x, y) kheap_instance.alloc(x,y)
#define __ALLOC(s) kheap_instance.alloc(s) 							//alloc macro
#define __SALLOC(obj) __ALLOC(sizeof(obj))							//simple alloc macro
#define __SCALLOC(obj, obj2) reinterpret_cast<obj2>(__SALLOC(obj))	//simple casting allocation
#define _SCALLOC(obj) reinterpret_cast<obj*>(__SALLOC(obj))			//even simpler casting allocation (so only one underscore)

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#endif