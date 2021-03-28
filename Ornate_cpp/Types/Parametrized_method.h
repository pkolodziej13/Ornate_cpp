#pragma once
#include "Pack.h"
namespace typ
{
	template<class Method_pointer_type>
	struct Parametrized_method_no_specifier;

	template<class Result_t,class Parent_t,class ... Arguments_type_v>
	struct Parametrized_method_no_specifier<Result_t(Parent_t::*)(Arguments_type_v...)>
	{
		using Result_type = Result_t;
		using Parent_type = Parent_t;
		using Arguments_pack = typ::Pack<Arguments_type_v...>;
	};


	template<class Method_pointer_type>
	struct Parametrized_method;

	template<class Result_type, class Parent_type, class ...Arguments_type_v>
	struct Parametrized_method<Result_type(Parent_type::*)(Arguments_type_v...)const noexcept(true)>
		:Parametrized_method_no_specifier<Result_type(Parent_type::*)(Arguments_type_v...)>
	{
		static constexpr bool is_noexcept = true;
		static constexpr bool is_const = true;
	};
	template<class Result_type, class Parent_type, class ...Arguments_type_v>
	struct Parametrized_method<Result_type(Parent_type::*)(Arguments_type_v...)const noexcept(false)>
		:Parametrized_method_no_specifier<Result_type(Parent_type::*)(Arguments_type_v...)>
	{
		static constexpr bool is_noexcept = false;
		static constexpr bool is_const = true;
	};
	template<class Result_type, class Parent_type, class ...Arguments_type_v>
	struct Parametrized_method<Result_type(Parent_type::*)(Arguments_type_v...)noexcept(true)>
		:Parametrized_method_no_specifier<Result_type(Parent_type::*)(Arguments_type_v...)>
	{
		static constexpr bool is_noexcept = true;
		static constexpr bool is_const = false;
	};
	template<class Result_type, class Parent_type, class ...Arguments_type_v>
	struct Parametrized_method<Result_type(Parent_type::*)(Arguments_type_v...)noexcept(false)>
		:Parametrized_method_no_specifier<Result_type(Parent_type::*)(Arguments_type_v...)>
	{
		static constexpr bool is_noexcept = false;
		static constexpr bool is_const = false;
	};
}