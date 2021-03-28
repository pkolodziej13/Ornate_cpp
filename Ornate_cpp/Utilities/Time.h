#pragma once
#include <chrono>
namespace uti
{
	template<class t = std::chrono::milliseconds>
	auto current_time()
	{
		return  std::chrono::duration_cast<t>(
			std::chrono::system_clock::now().time_since_epoch()).count();
	}

	struct Time_checkpoint
	{
		Time_checkpoint()
		{
			last_point = std::chrono::system_clock::now();
		}
		auto check_time()
		{
			return (std::chrono::system_clock::now() - last_point).count();
		}
	private:
		std::chrono::system_clock::time_point last_point;
	};
}