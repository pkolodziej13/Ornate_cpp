#pragma once
#include <Utilities/pointers.h>
#include "Ordered_tree.h"

namespace agg
{
	template<class suggested>
	struct Ordered_tree_view
	{
		using this_type = Ordered_tree_view<suggested>;
		using decayed_type = std::decay_t<suggested>;

		struct sugested_node_base
		{
			virtual uti::containing_shared_ptr<sugested_node_base> add_child() = 0;
			virtual suggested get() = 0;
		};

		template<class t, class = std::enable_if_t<std::is_convertible_v<t&, suggested>>>
		Ordered_tree_view(Ordered_tree<t>& to_manipulate)
			:node_manipulator(std::make_shared<sugested_node_concrete<t>>(to_manipulate))
			, object(to_manipulate.get())
		{

		}
		Ordered_tree_view(uti::containing_shared_ptr<sugested_node_base> child)
			:node_manipulator(child), object(child->get())
		{

		}
		template<class t, class = std::enable_if_t<std::is_convertible_v<t&, suggested>>>
		Ordered_tree_view(Ordered_tree_view<t>& manipulated)
			: node_manipulator(std::make_shared<sugested_node_proxy<t>>(manipulated))
			, object(manipulated.get())
		{

		}
		Ordered_tree_view(Ordered_tree_view<suggested>& manipulated)
			: node_manipulator(manipulated.node_manipulator)
			, object(manipulated.object)
		{

		}

		this_type add_child()
		{
			return this_type(node_manipulator->add_child());
		}
		suggested& get()
		{
			return object;
		};
		decayed_type* operator->() { return &get(); }

	private:
		template< class realized>
		struct sugested_node_concrete :sugested_node_base
		{
			using this_type = sugested_node_concrete< realized>;

			sugested_node_concrete(Ordered_tree<realized>& node)
				:node(node)
			{
			}
			virtual suggested get() { return node.get(); }
			Ordered_tree<realized>& node;
			uti::containing_shared_ptr<sugested_node_base>  add_child()
			{
				auto& added = node.add_child();
				return std::make_shared<this_type>(added);
			}
		};
		template< class realized>
		struct sugested_node_proxy :sugested_node_base
		{
			using this_type = sugested_node_proxy< realized>;

			sugested_node_proxy(Ordered_tree_view<realized>& node)
				:node(node)
			{
			}
			virtual suggested get()
			{
				return node.get();
			}
			Ordered_tree_view<realized> node;
			uti::containing_shared_ptr<sugested_node_base> add_child()
			{
				auto added = node.add_child();
				return std::make_shared<this_type>(added);
			}
		};

		suggested object;
		uti::containing_shared_ptr<sugested_node_base> node_manipulator;
	};
}