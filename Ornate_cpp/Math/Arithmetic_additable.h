#pragma once

namespace mth
{
	template<class Individualist,class Target>
	struct Arithmetic_additable
	{


		Individualist operator+(const Target& target)const
		{
			return Individualist::algebraic_pair_operation(
				individualist(),
				target,
				[](auto&& a, auto&& b) { return  a + b; });
		}
		Individualist operator-(const Target& target)const
		{
			return Individualist::algebraic_pair_operation(
				individualist(),
				target,
				[](auto&& a, auto&& b) { return  a - b; });
		}
		Individualist & operator+=(const Target& target)
		{
			individualist() = individualist() + target;
			return individualist();
		}
		Individualist& operator-=(const Target& target)
		{
			individualist() = individualist() - target;
			return individualist();
		}

		Individualist addition(const Target& target)const
		{
			return individualist() + target;
		}
		Individualist subtraction(const Target& target)const
		{
			return individualist()- target;
		}
	private:
		const Individualist& individualist()const
		{
			return static_cast<const Individualist&>(*this);
		}
		Individualist& individualist()
		{
			return static_cast<Individualist&>(*this);
		}
	};
}