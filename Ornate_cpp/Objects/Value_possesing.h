#pragma once

namespace obj
{
	enum class Value_possesing
	{
		optimal, first, second, both
	};
	enum class Value_possesing_concrete
	{
		none, first, second, both
	};

	template<class Complexity1, class Complexity2, class Value, Value_possesing possesing>
	struct Value_possesing_calculator
	{
		static constexpr Value_possesing_concrete calculate_possesing_concrete()
		{
			if constexpr (possesing == Value_possesing::first)
				return Value_possesing_concrete::first;
			else if constexpr (possesing == Value_possesing::second)
				return Value_possesing_concrete::second;
			else if constexpr (possesing == Value_possesing::both)
				return Value_possesing_concrete::both;
			else
			{
				size_t optimal_type_for_keeping_value = uti::Faster_complexity<Complexity1, Complexity2>::value;
				if (optimal_type_for_keeping_value == 0)
					return Value_possesing_concrete::first;
				else
					return Value_possesing_concrete::second;
			}
		}

		static constexpr Value_possesing_concrete possesing_concrete = calculate_possesing_concrete();

	}; 
	template<class Complexity1, class Complexity2, Value_possesing possesing>
	struct Value_possesing_calculator<Complexity1, Complexity2,void, possesing>
	{
		static constexpr Value_possesing_concrete possesing_concrete = Value_possesing_concrete::none;
	};



	template<Value_possesing_concrete possesing, class Value>
	struct Deduce_value_component;
	template<>
	struct Deduce_value_component<Value_possesing_concrete::none, void>
	{
		template<size_t I>
		using Value_component = void;
		static constexpr size_t type_keeping_value = 0;
	};
	template<class Value>
	struct Deduce_value_component<Value_possesing_concrete::first, Value>
	{
		template<size_t I>
		using Value_component = std::conditional_t<I == 0, Value, void >;
		static constexpr size_t type_keeping_value = 0;
	};
	template<class Value>
	struct Deduce_value_component<Value_possesing_concrete::second, Value>
	{
		template<size_t I>
		using Value_component = std::conditional_t<I == 1, Value, void >;
		static constexpr size_t type_keeping_value = 1;
	};
	template<class Value>
	struct Deduce_value_component<Value_possesing_concrete::both, Value>
	{
		template<size_t I>
		using Value_component = Value*;
		static constexpr size_t type_keeping_value = 0;
	};
}