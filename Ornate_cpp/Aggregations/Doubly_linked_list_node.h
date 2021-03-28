#pragma once

namespace agg
{
	template<class Individualist>
	struct Doubly_linked_list_node
	{
		using this_type = Doubly_linked_list_node<Individualist>;
		Doubly_linked_list_node()
		{
			connect_self();
		}
		Doubly_linked_list_node(const this_type& node_on_back)
		{
			this_type& node_on_front = node_on_back.next_node();
			const_cast<this_type&>(node_on_back).connect_next(*this);
			this->connect_next(node_on_front);
		}
		Doubly_linked_list_node(this_type&& node_to_repleace)
		{
			this_type& node_on_back  = node_to_repleace.back_node();
			this_type& node_on_front = node_to_repleace.next_node();
			node_on_back.connect_next(*this);
			this->connect_next(node_on_front);
			node_to_repleace.connect_self();
		}
		~Doubly_linked_list_node()
		{
			back->connect_next(*next);
		}
		void detach()
		{
			back->connect_next(*next);
			connect_self();
		}
		void Link_chain_as_next(Individualist& first_node_of_chain)
		{
			this_type& has_to_be_after_new = this->next_node();
			this_type& last_node_of_new = first_node_of_chain.back_node();

			this->connect_next(first_node_of_chain);
			last_node_of_new.connect_next(has_to_be_after_new);
		}
		void Link_chain_as_previous(Individualist& first_node_of_chain)
		{
			this->back_node().Link_chain_as_next(first_node_of_chain);
		}
		void swap_with_next()
		{
			if (back == next)
				return;

			this_type& has_to_be_first = *back;
			this_type& has_to_be_second = *next;
			this_type& has_to_be_fourth = has_to_be_second.next_node();

			has_to_be_first.connect_next(has_to_be_second);
			has_to_be_second.connect_next(*this);
			this->connect_next(has_to_be_fourth);
		}
		void swap_with_previoust()
		{
			this->back_node().swap_with_next();
		}

		bool single()
		{
			return next == this;
		}
		Individualist& next_node()const
		{
			return *next;
		}
		Individualist& back_node()const
		{
			return *back;
		}
	private:
		void connect_next(this_type& node)
		{
			this->next = &node.individualist();
			node.back = &individualist();
		}
		void connect_self()
		{
			back = &individualist();
			next = &individualist();
		}
		Individualist& individualist()
		{
			return static_cast<Individualist&>(*this);
		}

		Individualist* back;
		Individualist* next;
	};
}