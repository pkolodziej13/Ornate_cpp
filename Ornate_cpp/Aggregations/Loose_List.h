#pragma once
#include <Utilities/Non_replicable.h>
#include <Utilities/loops.h>
#include <Utilities/Range_for_loop.h>

#include "Doubly_linked_list_node.h"

namespace agg
{
	template<class T>
	struct Loose_element;
	template<class T>
	struct Loose_node :Doubly_linked_list_node<Loose_node<T>>,uti::non_replicable
	{
		using type = T;
		using Element_node = Loose_element<T>;
	};
	template<class T>
	struct Loose_element :Loose_node<T>
	{
		template<class ... Args_v>
		Loose_element(Args_v && ... args_v) :element(std::forward<Args_v>(args_v)...)
		{

		}
		T& get()
		{
			return element;
		}
		T& get()const
		{
			return element;
		}
	private:
		T element;
	};


	template<class T>
	struct Loose_list:Loose_node<T>
	{

		template<class ... Args_v>
		std::shared_ptr<Loose_element<T> > create_as_first(Args_v && ... args_v)
		{
			auto result= std::make_shared<Loose_element<T>>(std::forward<Args_v>(args_v)...);
			this->Link_chain_as_next(*result);
			return result;
		}
		template<class ... Args_v>
		std::shared_ptr<Loose_element<T> > create_as_last(Args_v && ... args_v)
		{
			auto result = std::make_shared<Loose_element<T>>(std::forward<Args_v>(args_v)...);
			this->link_chain_as_previous(*result);
			return result;
		}

		struct Fast_loop_range:uti::non_replicable
		{
			static constexpr auto features = uti::Loop_features::aggregator_unintacted | uti::Loop_features::order_unintacted;
		
			Fast_loop_range(Loose_list & list)
				:list_node(&list), currently_evaluated(&list.next_node())
			{}
			bool finished()const
			{
				return list_node == currently_evaluated;
			}
			T& current_element()const 
			{
				return static_cast<Loose_element<T>*>(currently_evaluated)->get();
			}
			void increment()
			{
				currently_evaluated = &currently_evaluated->next_node();
			}
			void finish_after_increment()
			{
				currently_evaluated = &list_node->back_node();
			}
		private:
			Loose_list* list_node;
			Loose_node<T>* currently_evaluated;
		};
		struct Save_loop_range :uti::non_replicable
		{
			static constexpr auto features =
				uti::Loop_features::excludes_added |
				uti::Loop_features::remove_save |
				uti::Loop_features::aggregator_remove_save |
				uti::Loop_features::order_unintacted;


			Save_loop_range(Loose_list& list)
			{
				list.link_chain_as_next(begin_node_mark);
				list.link_chain_as_previous(end_node_mark);
				last_selected_element = &begin_node_mark.next_node();
			}
			bool finished()const
			{
				return &begin_node_mark.next_node() == &end_node_mark;
			}
			T& current_element()const
			{
				return static_cast<Loose_element<T>&>(begin_node_mark.next_node()).get();
			}
			void increment()
			{
				if (last_selected_element == &begin_node_mark.next_node()&& !finished())
				{
					begin_node_mark.swap_with_next();
					last_selected_element = &begin_node_mark.next_node();
				}
			}
			void finish_after_increment()
			{
				begin_node_mark.detach();
				end_node_mark.Link_chain_as_previous(begin_node_mark);
			}
		private:
			Loose_node<T> begin_node_mark;
			Loose_node<T> end_node_mark;
			Loose_node<T>* last_selected_element = nullptr;
		};

		auto fast_range()
		{
			return uti::Range_for_loop<Fast_loop_range>(*this);
		}
		auto save_range()
		{
			return uti::Range_for_loop<Save_loop_range>(*this);
		}

	};
	template<class t>
	using Loose_element_shared = std::shared_ptr<Loose_element<t>>;
}


