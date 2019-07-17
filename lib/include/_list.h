#ifndef _LIST_H_
#define _LIST_H_

template <class... Ts>
class list;

template <class T, class... Ts>
inline auto make_list(T, Ts...);

template <int, class... Ts>
inline auto get(const list<Ts...>&);

template <class T, class... Ts>
class list<T, Ts...>
{
    T head;
    list<Ts...> tail;
	friend auto make_list<T, Ts...>(T, Ts...);
    template <int, class... Tss> 
	friend auto get(const list<Tss...>&);
};

template <>
class list<> {};

inline auto make_list()
{
	return list<>();
}

template <class T, class... Ts>
inline auto make_list(T head, Ts... tail)
{
	list<T, Ts...> res;
	res.head = head;
	res.tail = make_list(tail...);
	return res;
}

template <int c, class... Ts>
inline auto get(const list<Ts...>& l)
{
	if constexpr (c == 0) return l.head;
	else return get<c-1>(l.tail);
}

#endif