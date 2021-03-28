#pragma once
#include <memory>

namespace obj
{
	template<class t>
	struct Prototype
	{
	private:
		struct Abstract_accessor_copier
		{
			virtual std::shared_ptr<Abstract_accessor_copier> copy()const = 0;
			virtual const t& get()const = 0;
			virtual t& get() = 0;
		};
		template<class inherited>
		struct Accessor_copier :Abstract_accessor_copier
		{
			Accessor_copier(const inherited& to_copy) :object(to_copy) {}
			inherited object;
			virtual std::shared_ptr<Abstract_accessor_copier> copy()const
			{
				return std::make_shared<Accessor_copier<inherited>>(object);
			}
			virtual const t& get()const
			{
				return object;
			}
			virtual t& get()
			{
				return object;
			}
		};

		std::shared_ptr<Abstract_accessor_copier> proto_object;
	public:
		Prototype() :proto_object(std::make_shared<Accessor_copier<t>>(t{})) {}
		template<class inherited, class Enable = std::enable_if_t<std::is_base_of_v<t, inherited>>>
		Prototype(const inherited& to_copy) : proto_object(std::make_shared<Accessor_copier<inherited>>(to_copy)) {}
		Prototype(const t& to_copy) :proto_object(std::make_shared<Accessor_copier<t>>(to_copy)) {}
		Prototype(const Prototype<t>& to_copy) :proto_object(to_copy.proto_object->copy()) {}
		Prototype& operator=(const Prototype<t>& to_copy)
		{
			proto_object = to_copy.proto_object->copy();
			return *this;
		}


		template<class inherited, class Enable = std::enable_if_t<std::is_base_of_v<t, inherited>>>
		void emplace(const inherited& to_copy) { proto_object = std::make_shared<Accessor_copier<inherited>>(to_copy); }
		t* operator->()
		{
			return &proto_object->get();
		}
		t& get()
		{
			return proto_object->get();
		}
	};
}