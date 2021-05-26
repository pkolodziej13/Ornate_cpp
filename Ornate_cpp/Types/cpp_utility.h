#pragma once
#include <type_traits>

namespace typ
{
	template<class Type, class Member_type>
	using member_pointer = Member_type Type::*;
	template<class T>
	struct OwnersipOrganizer
	{
		using pointer_type = T*;
		using pointer_to_const_type = const T*;
		using copying_type = const T&;
		using reference_type = T&;
	};
	template<class T>
	struct OwnersipOrganizer<T&>
	{
		using pointer_type = T*;
		using pointer_to_const_type = const T*;
		using copying_type = T&;
		using reference_type = T&;
	};

	template< class T>
	struct removeMemberPointer;
	template < class T, class Base>
	struct removeMemberPointer<Base T::*>
	{
		using type = Base;
	};
	template <class T>
	using remove_m_ptr = typename removeMemberPointer<T>::type;

	template <class T>
	using removeMember = typename  removeMemberPointer<T>::type;

	template<class T>
	using is_non_const_lvalue_refeerence = std::integral_constant<bool, std::is_lvalue_reference_v<T> && (!std::is_const_v<std::remove_reference_t<T>>)>;

}