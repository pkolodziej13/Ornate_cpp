#pragma once
namespace uti
{
	template<class t, size_t number>
	struct Numbered_component
	{
		Numbered_component() {}
		Numbered_component(const t& element) :element(element)
		{}
		t element;
		t& get()
		{
			return element;
		}
		const t& get()const
		{
			return element;
		}
		using type = t;
		static constexpr size_t value = number;
	};
	template<class t>
	struct Numbered_component<t, 0>
	{
		Numbered_component() {}
		Numbered_component(const t& element) :first(element)
		{}
		t first;
		t& get()
		{
			return first;
		}
		const t& get()const
		{
			return first;
		}
		using type = t;
		static constexpr size_t value = 0;
	};
	template<class t>
	struct Numbered_component<t, 1>
	{
		Numbered_component() {}
		Numbered_component(const t& element) :second(element)
		{}
		t second;
		t& get()
		{
			return second;
		}
		const t& get()const
		{
			return second;
		}
		using type = t;
		static constexpr size_t value = 1;
	};
	template<class t>
	struct Numbered_component<t, 2>
	{
		Numbered_component() {}
		Numbered_component(const t &  element) :third(element)
		{}
		t third;
		t& get()
		{
			return third;
		}
		const t& get()const
		{
			return third;
		}
		using type = t;
		static constexpr size_t value = 2;
	};
}