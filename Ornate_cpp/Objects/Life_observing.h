#pragma once
#include <memory>

namespace obj
{
	template<class T>
	struct Life_observer;

	template<class Individualist>
	struct Life_observalable
	{
		Life_observalable()
		{
			static_assert(std::is_base_of_v<Life_observalable<Individualist>, Individualist>);

			life_observer = std::make_shared<std::reference_wrapper<Individualist>>(individualist());
		}
	private:
		inline Individualist& individualist()
		{
			return static_cast<Individualist&>(*this);
		}
		friend Life_observer<Individualist>;

		std::shared_ptr<std::reference_wrapper<Individualist>> life_observer;
	};

	template<class T>
	struct Life_observer
	{
		Life_observer(T& obseverd_object)
			:observer(obseverd_object.life_observer)
		{
		}

		operator bool()const
		{
			return !observer.expired();
		}
		T* get()const
		{
			if (!observer.expired())
			{
				std::shared_ptr<std::reference_wrapper<T>> shared{ observer };
				return &shared->get();
			}
			return nullptr;
		}
	private:

		std::weak_ptr<std::reference_wrapper<T>> observer;
	};

}