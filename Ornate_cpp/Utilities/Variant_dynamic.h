#pragma once
#include <variant>
#include <memory>
#include "Non_replicable.h"

namespace uti
{
	template<class ... T_v>
	struct Varaint_dynamic :uti::non_replicable
	{
		template<class T, class ... args>
		void emplace(const args & ... args_a)
		{
			element.emplace<std::unique_ptr<T>>(std::make_unique<T>(args_a...));
		}
		template<class T>
		bool holds_alternative()
		{
			return element.holds_alternative<std::unique_ptr<T>>();
		}
		size_t index()
		{
			return element.index();
		}
		template<class Lambda>
		auto visit(Lambda&& lambda)
		{
			return std::visit([&](auto& a) {
				return lambda(*a);
			}, element);
		}
		template<class T>
		T& get()
		{
			return *std::get<std::unique_ptr<T>>(element);
		}
	private:

		std::variant<std::unique_ptr<T_v>... > element;
	};

}