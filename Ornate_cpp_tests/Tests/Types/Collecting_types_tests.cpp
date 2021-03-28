#include "pch.h"
#include "CppUnitTest.h"

#include <Types/Utility.h>
#include <Types/Pack_utility.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


template< class t, class tr, class = void >
struct is_type_existing_temp :std::false_type {};
template<class t, class tr >
struct  is_type_existing_temp<t, tr, decltype(typeid(t), void())> :std::true_type
{
};
template<size_t i>
struct packing;
template<size_t i, class tt>
constexpr size_t next_pack_element()
{
	if constexpr (is_type_existing_temp<packing<i>, tt>::value)
		return next_pack_element<i + 1, tt>();
	else return i;
}


struct A {};
struct B {};
struct C {};

template<>
struct packing<next_pack_element<0, A>()>
{
	using type = A;

};
template<>
struct packing<next_pack_element<0, B>()>
{
	using type = B;
};
template<>
struct packing<next_pack_element<0, C>()>
{
	using type = C;
};

template<size_t i, class Enable = void>
struct Collected_types;

template< size_t i>
struct Collected_types<  i, std::enable_if_t<typ::is_type_existing<packing<i>>::value>>
{
	using type = typ::p_add<typename packing<i>::type,typename Collected_types<i+1>::type >;
};
template< size_t i>
struct Collected_types<  i, std::enable_if_t<!typ::is_type_existing<packing<i>>::value>>
{
	using type = typ::Pack<>;
};

TEST_CLASS(Ordered_hiererchy_test)
{
public:

	TEST_METHOD(packing_test)
	{
		static_assert( typ::is_type_existing<packing<0>>::value);
		static_assert(typ::is_type_existing<packing<1>>::value);
		static_assert(typ::is_type_existing<packing<2>>::value);
		static_assert(!typ::is_type_existing<packing<3>>::value);
		static_assert(!typ::is_type_existing<packing<4>>::value);


		using all_collected_types = typename Collected_types<0>::type;
		static_assert(std::is_same_v<all_collected_types,typ::Pack<A,B,C>>);
	}

};