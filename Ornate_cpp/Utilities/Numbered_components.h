#pragma once
namespace uti
{
	template<class T, size_t number>
	struct Numbered_component
	{
		using type = T;
		static constexpr size_t value = number;

		Numbered_component() {}
		Numbered_component(const T& element) :element(element)
		{}
		T& get()
		{
			return element;
		}
		const T& get()const
		{
			return element;
		}

		T element;
	};
	template<class T>
	struct Numbered_component<T, 0>
	{
		using type = T;
		static constexpr size_t value = 0;

		Numbered_component() {}
		Numbered_component(const T& element) :first(element)
		{}
		T& get()
		{
			return first;
		}
		const T& get()const
		{
			return first;
		}

		T first;
	};
	template<class T>
	struct Numbered_component<T, 1>
	{
		using type = T;
		static constexpr size_t value = 1;

		Numbered_component() {}
		Numbered_component(const T& element) :second(element)
		{}
		T& get()
		{
			return second;
		}
		const T& get()const
		{
			return second;
		}

		T second;
	};
	template<class T>
	struct Numbered_component<T, 2>
	{
		using type = T;
		static constexpr size_t value = 2;

		Numbered_component() {}
		Numbered_component(const T&  element) :third(element)
		{}
		T& get()
		{
			return third;
		}
		const T& get()const
		{
			return third;
		}

		T third;
	};
}