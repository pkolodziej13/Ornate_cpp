#pragma once
#include <unordered_map>
#include <unordered_set>

#include <Types/Pack.h>
#include <Types/Pack_utility.h>
#include <Types/Functions_parametrized_pointers.h>

#include "Contacts_transfering.h"

namespace col
{
	template<class Comparer,template<size_t I>class Nested>
	struct Grouping_colliders_regard_layers
	{
		using this_type = Grouping_colliders_regard_layers<Comparer, Nested>;

		using Layers_p = typename Comparer::Layers_p;
		using key_type  = std::bitset<Layers_p::size>;

		template<size_t i> using make_container_for_collider = Nested<i+1>;
		using Kinds_p = typ::p_from_index_squence<make_container_for_collider, Layers_p::size>;
		template<class T>
		using make_unordered_map = std::unordered_map<key_type, T>;

		using Kinds_in_hash_p = typ::p_detail< make_unordered_map , Kinds_p>;
		using Kinds = typ::p_tuple<Kinds_in_hash_p>;

		static size_t key_to_kind(const key_type& key)
		{
			return key.count() - 1;
		}
		static key_type interfering_key(const key_type& key)
		{
			key_type result;
			auto collect_collides = [&](auto par)
			{
				using type = decl_parameter(par);
				if(key.test(typ::p_index < Layers_p, type>))
					result |= Contacts_transfering<Comparer>::template colliding_map_of_type<type>();
			};
			Layers_p::for_each(collect_collides);
			return result;
		}
		static bool keys_collides(const key_type& a, const key_type& b)
		{
			auto aa = interfering_key(a);
			auto innn = (aa & b);
			auto innn2 = innn.any();
			return (interfering_key(a)& b).any();
		}

		template<class Access_info>
		struct Calling_add_element_of_kind
		{
			template<size_t I>
			static void call(this_type& context,key_type key, Access_info& access_info)
			{
				if (std::get<I>(context.elements).count(key) == 0)
				{
					context.currently_used_keys.insert(key);
				}
				std::get<I>(context.elements)[key].add_element(access_info);
			}
		};
		template<class Access_info>
		void add_element(key_type key,  Access_info& access_info)
		{
			using callers_type = typ::Functions_parametrized_pointers<Calling_add_element_of_kind<Access_info>, Layers_p::size>;
			(*callers_type::value[key_to_kind(key)])(*this,key, access_info);
		}

		template<class Access_info>
		struct Calling_remove_element_of_kind
		{
			template<size_t I>
			static void call(this_type& context, key_type key, Access_info& access_info)
			{
				std::get<I>(context.elements)[key].remove_element(access_info);
				if (!std::get<I>(context.elements)[key].is_used())
				{
					std::get<I>(context.elements).erase(key);
					context.currently_used_keys.erase(key);
				}
			}
		};
		template<class Access_info>
		void remove_element(key_type key, Access_info& access_info)
		{
			using callers_type = typ::Functions_parametrized_pointers<Calling_remove_element_of_kind<Access_info>, Layers_p::size>;
			(*callers_type::value[key_to_kind(key)])(*this, key, access_info);
		}

		template<class Lambda>
		struct Calling_action_on_element_element_of_kind
		{
			template<size_t I>
			static void call(this_type& context, key_type key, Lambda&& lambda)
			{
				lambda(std::get<I>(context.elements)[key]);
			}
		};
		template<class Lambda>
		auto action_on_element(key_type key,Lambda&& lambda)
		{
			using callers_type = typ::Functions_parametrized_pointers<Calling_action_on_element_element_of_kind<Lambda>, Layers_p::size>;
			(*callers_type::value[key_to_kind(key)])(*this, key, std::forward<Lambda>(lambda));
		}

		template<class Lambda>
		void evaluate_colliders_segregator(Lambda&& lambda)
		{
			for (auto iter_a = currently_used_keys.begin();iter_a!= currently_used_keys.end();++iter_a)
				for (auto iter_b = iter_a; iter_b != currently_used_keys.end(); ++iter_b)
					if (keys_collides((*iter_a), (*iter_b)))
					{
						action_on_element((*iter_a), [&](auto& elemnts1)
						{
							action_on_element((*iter_b), [&](auto& elemnts2)
							{
								lambda((*iter_a), (*iter_b), elemnts1, elemnts2);
							});
						});
					}

		}
		bool is_used()
		{
			return !currently_used_keys.empty();
		}
		template<class Lambda>
		void for_each(Lambda&& lambda)
		{
			for (key_type a : currently_used_keys)
				action_on_element(a, [&](auto & element) 
			{
				lambda(a, element);
			});
		}

	private:
		Kinds elements;
		std::unordered_set<key_type> currently_used_keys;
	};
}