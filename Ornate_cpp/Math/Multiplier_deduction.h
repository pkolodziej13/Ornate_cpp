#pragma once
#include <Types/Parametrized_method.h>
namespace mth
{

	template<typename T>
	concept Default_multiplication = requires(T a, T b)
	{
		a* b;
	};

	template<class Result_type, class Parent_type, class Argument_type>
	typ::Parametrized_method<Result_type(Parent_type::*)(Argument_type)const noexcept> multilying_operator_trial(Result_type(Parent_type::*)(Argument_type)const noexcept)
	{}
	template<class Result_type, class Parent_type, class Argument_type>
	typ::Parametrized_method<Result_type(Parent_type::*)(Argument_type)const> multilying_operator_trial(Result_type(Parent_type::*)(Argument_type)const)
	{}

	template<typename T>
	concept Member_multiplication = requires
	{
		multilying_operator_trial(&T::operator *);
	};

	template<class Left_type>
	struct Multiplier_deduction;
	template<Default_multiplication Left_type>
	struct Multiplier_deduction<Left_type>
	{
		using Right_type = Left_type;
	};
	template<Member_multiplication Left_type>
		requires (!Default_multiplication<Left_type>)
	struct Multiplier_deduction<Left_type>
	{
		using trial_result = decltype(multilying_operator_trial(&Left_type::operator *));
		using Right_type = typ::p_element<0,typename trial_result::Arguments_pack>;
	};

	template<class T>
	using Multiplier_of = std::decay_t<typename Multiplier_deduction<T>::Right_type>;
}