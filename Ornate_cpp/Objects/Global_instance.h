#pragma once
#include <memory>
namespace obj
{
	template<class T>
	struct Global_instance
	{
		static T& get()
		{
			if (instance == nullptr)
				instance = std::make_unique<T>();
			return *instance;
		}
	private:
		static inline std::unique_ptr<T> instance{nullptr};
	};
	template<class T>
	using Singleton = Global_instance<T>;
}