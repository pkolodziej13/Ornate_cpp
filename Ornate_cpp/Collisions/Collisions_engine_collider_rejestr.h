#pragma once
#include <variant>
#include <Types/Pack.h>
#include <Objects/Polymorphism.h>

#include "Checking_collisions.h"
#include "Collisions_requirements.h"

namespace col
{
	template<class Collider_type>
	struct Collider_memorized
	{
		Collider_memorized() = default;
		Collider_memorized(Collider<Collider_type>& to_set)
			:collider{ &to_set } {}
		void memorize()
		{
			memorized = collider->current;
		}
		const Collider_type& get_memorized()const
		{
			return memorized;
		}
	private:
		Collider<Collider_type>* collider = nullptr;
		Collider_type memorized;
	};


	template<class Standard_colliders_p>
	struct Colliders_rejestr
	{
		template<size_t I>
		struct Rejestr
		{
			using this_type = Rejestr<I>;

			using Collider_type = typ::p_transform<std::variant, typ::p_detail<Collider_memorized, Standard_colliders_p>>;

			template<class Access_info>
			Rejestr(Access_info& access_info)
				:collider(create_variant(obj::deduce_type<typ::p_detail<Collider, Standard_colliders_p>>(*access_info.collider).value, access_info))
			{
				for_i(I)
					objects[i] = access_info.connected_objects[i];
			}

			template<class Access_info,size_t I_local>
			Collider_type create_variant_on_position(Access_info& access_info)
			{
				using Local_collider_type = typ::p_element< I_local, Standard_colliders_p>;

				return { Collider_memorized<typ::p_element< I_local,Standard_colliders_p>>{*dynamic_cast<Collider<Local_collider_type>*>(access_info.collider)} };
			}
			template<class Access_info>
			using Varaint_creator_table = std::array<Collider_type (this_type::*)(Access_info&), Standard_colliders_p::size>;
			template<class Access_info,size_t ... I_v>
			static Varaint_creator_table<Access_info> create_table(std::index_sequence<I_v...>)
			{
				return { &this_type::create_variant_on_position<Access_info,I_v>... };
			}
			template<class Access_info>
			Collider_type create_variant(size_t index, Access_info& access_info)
			{
				static Varaint_creator_table<Access_info> table = create_table<Access_info>(std::make_index_sequence<Standard_colliders_p::size>());
				return (this->*table[index])(access_info);
			}
			template<size_t i_specific>
			bool is_colliding(Rejestr<i_specific> & r)
			{
				return std::visit([&](auto& c1)
				{
					return std::visit([&](auto& c2)
					{
						return colliders_collides(c1.get_memorized(), c2.get_memorized());
					}, r.get_collider());
				}, collider);
			}
			void synchronize()
			{
				std::visit([](auto& c) 
				{
					c.memorize();
				}, collider);
			}
			std::array<Object_unified*, I> const& get_objects()const
			{
				return objects;
			}
			Collider_type const& get_collider()const
			{
				return collider;
			}
		private:

			std::array<Object_unified*, I> objects;
			Collider_type collider;
		};
	};
}