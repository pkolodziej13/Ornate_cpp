#pragma once
#include <memory>

namespace obj
{
	template<class T>
	struct Prototype
	{
	private:
		struct Abstract_accessor_copier
		{
			virtual std::shared_ptr<Abstract_accessor_copier> copy()const = 0;
			virtual const T& get()const = 0;
			virtual T& get() = 0;
		};
		template<class Inherited>
		struct Accessor_copier :Abstract_accessor_copier
		{
			Accessor_copier(const Inherited& to_copy) 
				:object(to_copy) {}
			virtual std::shared_ptr<Abstract_accessor_copier> copy()const
			{
				return std::make_shared<Accessor_copier<Inherited>>(object);
			}
			virtual const T& get()const
			{
				return object;
			}
			virtual T& get()
			{
				return object;
			}
		private:
			Inherited object;
		};

	public:
		Prototype() 
			:prototype_object(std::make_shared<Accessor_copier<T>>(T{})) {}
		template<class Inherited, class Enable = std::enable_if_t<std::is_base_of_v<T, Inherited>>>
		Prototype(const Inherited& to_copy) 
			: prototype_object(std::make_shared<Accessor_copier<Inherited>>(to_copy)) {}
		Prototype(const T& to_copy) 
			:prototype_object(std::make_shared<Accessor_copier<T>>(to_copy)) {}
		Prototype(const Prototype<T>& to_copy) 
			:prototype_object(to_copy.prototype_object->copy()) {}

		Prototype& operator=(const Prototype<T>& to_copy)
		{
			prototype_object = to_copy.prototype_object->copy();
			return *this;
		}
		template<class Inherited, class Enable = std::enable_if_t<std::is_base_of_v<T, Inherited>>>
		void emplace(const Inherited& to_copy) 
		{ 
			prototype_object = std::make_shared<Accessor_copier<Inherited>>(to_copy); 
		}
		T* operator->()
		{
			return &prototype_object->get();
		}
		T& get()
		{
			return prototype_object->get();
		}
	private:
		std::shared_ptr<Abstract_accessor_copier> prototype_object;

	};
}