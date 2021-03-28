#pragma once
#include <type_traits>
#include <optional>
#include "../Types/Pack.h"
#include <Utilities/Index.h>

template<class ... Value_constuctor_argumets>
struct Value_creator
{
	Value_creator(Value_constuctor_argumets && ... arguments_v)
		:arguments(arguments_v...)
	{
	}
	template<class Lambda>
	void construct(Lambda && lambda)
	{
		call_construct(lambda, std::make_index_sequence<sizeof...(Value_constuctor_argumets)>{});
	}
private:
	template<class Lambda, size_t ... I>
	void call_construct(Lambda && lambda,std::index_sequence<I...>)
	{
		lambda(std::forward<Value_constuctor_argumets>(std::get<I>(arguments))...);
	}
	std::tuple<Value_constuctor_argumets...> arguments;
};



template<class Object, class Keys_p,class Accesses_p >
struct Relation_wing;
template<class owners_sequence,bool ordered_creating,class Keys_p>
struct Relation_value;



template<class Value,class Objects_p,class Value_keepers_s,class Value_parameters>
struct Relations_graph_resolver;

template<class Value,class ... Objects_v, class ... Keys_p_v, class ... Accesses_p_v, class ... Symetries_s_v, size_t ... Value_keepers, bool ordered_creating, class Keys_p>
struct Relations_graph_resolver
	<Value,typ::Pack<Relation_wing<Objects_v, Keys_p_v, Accesses_p_v>...>,Pack<Symetries_s_v...>,Relation_value< std::index_sequence<Value_keepers...>, ordered_creating, Keys_p>>
{
	//odpowiedzialnosci: rozwiazywanie grafu, zabezpiecznie, tworzenie wartosci

	template<class Responsibility = No_responsibility>
	typename Responsibility::template returner<std::add_lvalue_reference_t<Value>> create(Objects_v & ... objects)
	{
	}
	template<class Responsibility = No_responsibility>
	typename Responsibility::template returner<std::add_lvalue_reference_t<Value>> value(Objects_v & ... objects)
	{
		//find shortest tree over entire graph
	}

	bool exists(Objects_v & ... objects_individuals)
	{
		std::tuple<typename Objects_v::Individualist> individualists{ objects_individuals.individualist()... };
		size_t Shortest_to_start;
		//find shortest tree over entire graph
		//access with key is better than adress access
		//less quantity is faster
		//select object without key_access
		//select objects with the smallest quantity
	}

	template<class Responsibility = No_responsibility>
	typename Responsibility::template returner<void> remove(Objects_v & ... objects)
	{
	}
	template<class T,class Responsibility = No_responsibility>
	typename Responsibility::template returner<void> remove_object_relation(T & object)
	{
	}
};