#pragma once

namespace mth
{
	template<class T>
	constexpr T pi_t = static_cast<T>(3.14159265358979323846);
	constexpr float pi = pi_t<float>;
	constexpr float pi2 = pi * 2;
	constexpr double pi_d = pi_t<double> * 2;

	inline float trigonometric_identity(float to_revert)
	{
		return std::sqrtf(1 - to_revert * to_revert);
	}
	inline double trigonometric_identity(double to_revert)
	{
		return std::sqrt(1 - to_revert * to_revert);
	}
}