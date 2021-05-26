#pragma once
#include <memory>

#include <Utilities/containing_shared_ptr.h>
#include <Types/Utility.h>
#include <Aggregations/Loose_List.h>


namespace uti
{
	template<class T>
	struct Provider;

	template<class T>
	struct Receptor
	{
		using this_type = Receptor<T>;
		template<class T_specific, class... arguments_v>
		Receptor(typ::Overtype<T_specific>, arguments_v && ... arguments_av)
			:received(std::make_shared<T_specific>(std::forward<arguments_v>(arguments_av)...))
		{
		}
		Receptor(const T& prov) :received(std::make_shared<T>(prov))
		{
		}
		Receptor() :received(std::make_shared<T>())
		{
		}
		Receptor(Provider<T>& prov)
			:received(prov.get_observator())
		{
			provider_connection = prov.receptors.create_as_last(*this);
		}
		Receptor(const this_type& to_copy)
			:received(to_copy.received)
		{
			if (to_copy.provider_connection)
				provider_connection = to_copy.provider_connection->Create_as_last(*this);
		}
		void set(const T& val)
		{
			if (received.use_count() == 1 && !provider_connection)
			{
				(*std::const_pointer_cast<T>(received.shared())) = val;
			}
			else
			{
				provider_connection.reset();
				received = std::make_shared<T>(val);
			}
			proceed();
		}
		virtual void proceed() = 0;
		const T& value()
		{
			return received.get();
		}
	private:
		friend Provider<T>;
		uti::containing_shared_ptr<const T> received;
		agg::Loose_element_shared<std::reference_wrapper<Receptor<T>>> provider_connection;
	};

	template<class T>
	struct Provider
	{
		using this_type = Provider<T>;
		using type = T;
		template<class ... Arguments_v>
		Provider(Arguments_v && ... arguments_av) :provided(typ::Overtype<T>{}, std::forward<Arguments_v>(arguments_av)...)
		{
		}
		Provider(const this_type& to_copy)
			:provided(typ::Overtype<T>{}, to_copy.provided.get())
		{
		}
		std::shared_ptr<const T> get_observator()
		{
			return provided.shared();
		}
		std::shared_ptr< T> danger_observator()
		{
			return provided.shared();
		}
		void add_receptor(Receptor<T>& rec)
		{
			rec.received = get_observator();
			rec.provider_connection = receptors.create_as_last(rec);
			rec.proceed();
		}

		const T& value()
		{
			return *provided;
		}
	protected:
		void set(const T& new_value)
		{
			if (new_value == provided.get())
				return;
			provided.get() = new_value;
			for (auto& a : receptors.save_range())
			{
				a.get().proceed();
			}
		}
	private:
		friend Receptor<T>;
		uti::containing_shared_ptr<T> provided;
		agg::Loose_list<std::reference_wrapper<Receptor<T>>> receptors;
	};
}