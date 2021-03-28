#pragma once

namespace typ
{
	template<class type, class member_type>
	using member_pointer = member_type type::*;
	template<class t>
	struct OwnersipOrganizer
	{
		using pointer_type = t*;
		using pointer_to_const_type = const t*;
		using copying_type = const t&;
		using reference_type = t&;
	};
	template<class t>
	struct OwnersipOrganizer<t&>
	{
		using pointer_type = t*;
		using pointer_to_const_type = const t*;
		using copying_type = t&;
		using reference_type = t&;
	};

	template< class t>
	struct removeMemberPointer;
	template < class t, class base>
	struct removeMemberPointer<base t::*>
	{
		using type = base;
	};
	template <class t>
	using remove_m_ptr = typename removeMemberPointer<t>::type;

	template <class t>
	using removeMember = typename  removeMemberPointer<t>::type;

	template<class t>
	using is_non_const_lvalue_refeerence = std::integral_constant<bool, std::is_lvalue_reference_v<t> && (!std::is_const_v<std::remove_reference_t<t>>)>;

}