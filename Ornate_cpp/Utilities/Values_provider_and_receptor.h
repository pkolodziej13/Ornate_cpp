#pragma once
#include <memory>

#include <Types/Utility.h>
#include <Aggregations/Loose_List.h>

#include "pointers.h"

namespace uti
{
	template<class t>
	struct provider;

	template<class t>
	struct receptor
	{
		using this_type = receptor<t>;
		template<class t2, class... arguments_v>
		receptor(typ::Overtype<t2>, arguments_v && ... arguments_av)
			:received(std::make_shared<t>(std::forward<arguments_v>(arguments_av)...))
		{
		}
		receptor(const t& prov) :received(std::make_shared<t>(prov))
		{
		}
		receptor() :received(std::make_shared<t>())
		{
		}
		receptor(provider<t>& prov)
			:received(prov.get_observator())
		{
			provider_connection = prov.receptors.Create_as_last(*this);
		}
		receptor(const this_type& to_copy)
			:received(to_copy.received)
		{
			if (to_copy.provider_connection)
				provider_connection = to_copy.provider_connection->Create_as_last(*this);
		}
		void set(const t& val)
		{
			if (received.use_count() == 1 && !provider_connection)
			{
				(*std::const_pointer_cast<t>(received.shared())) = val;
			}
			else
			{
				provider_connection.reset();
				received = std::make_shared<t>(val);
			}
			proceed();
		}
		virtual void proceed() = 0;
		const t& value()
		{
			return received.get();
		}
	private:
		friend provider<t>;
		uti::containing_shared_ptr<const t> received;
		agg::Loose_element_shared<std::reference_wrapper<receptor<t>>> provider_connection;
	};

	template<class t>
	struct provider
	{
		using this_type = provider<t>;
		using type = t;
		template<class ... arguments_v>
		provider(arguments_v && ... arguments_av) :provided(typ::Overtype<t>{}, std::forward<arguments_v>(arguments_av)...)
		{
		}
		provider(const this_type& to_copy)
			:provided(typ::Overtype<t>{}, to_copy.provided.get())
		{
		}
		std::shared_ptr<const t> get_observator()
		{
			return provided.shared();
		}
		std::shared_ptr< t> danger_observator()
		{
			return provided.shared();
		}
		void add_receptor(receptor<t>& rec)
		{
			rec.received = get_observator();
			rec.provider_connection = receptors.Create_as_last(rec);
			rec.proceed();
		}

		const t& value()
		{
			return *provided;
		}
	protected:
		void set(const t& new_value)
		{
			if (new_value == provided.get())
				return;
			provided.get() = new_value;
			for (auto& a : receptors.Save_range())
			{
				a.get().proceed();
			}
		}
	private:
		friend receptor<t>;
		uti::containing_shared_ptr<t> provided;
		agg::Loose_list<std::reference_wrapper<receptor<t>>> receptors;
	};
}