#pragma once
#include <Utilities/containing_shared_ptr.h>

#include "Ordered_tree.h"

namespace agg
{
	template<class Suggested>
	struct Ordered_tree_view
	{
		using this_type = Ordered_tree_view<Suggested>;
		using decayed_type = std::decay_t<Suggested>;

		struct Sugested_node_base
		{
			virtual uti::containing_shared_ptr<Sugested_node_base> add_child() = 0;
			virtual Suggested get() = 0;
		};

		template<class T_specific, class = std::enable_if_t<std::is_convertible_v<T_specific&, Suggested>>>
		Ordered_tree_view(Ordered_tree<T_specific>& to_manipulate)
			:node_manipulator(std::make_shared<Sugested_node_concrete<T_specific>>(to_manipulate))
			, object(to_manipulate.get())
		{

		}
		Ordered_tree_view(uti::containing_shared_ptr<Sugested_node_base> child)
			:node_manipulator(child), object(child->get())
		{

		}
		template<class T_specific, class = std::enable_if_t<std::is_convertible_v<T_specific&, Suggested>>>
		Ordered_tree_view(Ordered_tree_view<T_specific>& manipulated)
			: node_manipulator(std::make_shared<Sugested_node_proxy<T_specific>>(manipulated))
			, object(manipulated.get())
		{

		}
		Ordered_tree_view(Ordered_tree_view<Suggested>& manipulated)
			: node_manipulator(manipulated.node_manipulator)
			, object(manipulated.object)
		{

		}

		this_type add_child()
		{
			return this_type(node_manipulator->add_child());
		}
		Suggested& get()
		{
			return object;
		};
		decayed_type* operator->() { return &get(); }

	private:
		template< class Realized>
		struct Sugested_node_concrete :Sugested_node_base
		{
			using this_type = Sugested_node_concrete< Realized>;

			Sugested_node_concrete(Ordered_tree<Realized>& node)
				:node(node)
			{
			}
			virtual Suggested get() { return node.get(); }
			Ordered_tree<Realized>& node;
			uti::containing_shared_ptr<Sugested_node_base>  add_child()
			{
				auto& added = node.add_child();
				return std::make_shared<this_type>(added);
			}
		};
		template< class Realized>
		struct Sugested_node_proxy :Sugested_node_base
		{
			using this_type = Sugested_node_proxy< Realized>;

			Sugested_node_proxy(Ordered_tree_view<Realized>& node)
				:node(node)
			{
			}
			virtual Suggested get()
			{
				return node.get();
			}
			Ordered_tree_view<Realized> node;
			uti::containing_shared_ptr<Sugested_node_base> add_child()
			{
				auto added = node.add_child();
				return std::make_shared<this_type>(added);
			}
		};

		Suggested object;
		uti::containing_shared_ptr<Sugested_node_base> node_manipulator;
	};
}