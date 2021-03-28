#pragma once
#include <variant>
#include <memory>
#include "Non_replicable.h"

namespace uti
{
	template<class ... tt>
	struct varaint_dynamic :uti::non_replicable
	{
		template<class t, class ... args>
		void emplace(const args & ... args_a)
		{
			object.emplace<std::unique_ptr<t>>(std::make_unique<t>(args_a...));
		}
		std::variant<std::unique_ptr<tt>... > object;
		template<class t>
		bool holds_alternative()
		{
			return object.holds_alternative<std::unique_ptr<t>>();
		}
		size_t index()
		{
			return object.index();
		}
		template<class lam>
		auto visit(const lam& func)
		{
			return std::visit([&](auto& a) {
				return func(*a);
			}, object);
		}
		template<class t>
		t& get()
		{
			return *std::get<std::unique_ptr<t>>(object);
		}
	private:
		template<class t, class lam>
		auto call_visit(const lam& func)
		{
			return func(this->get<t>());
		}

	};

}