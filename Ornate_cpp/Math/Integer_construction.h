#pragma once
#include <cstdint>

namespace mth
{
	template<bool sign, size_t bits>
	struct Integer_construction;
	template<> struct Integer_construction<false, 8> { using type = std::uint8_t; };
	template<> struct Integer_construction<false, 16> { using type = std::uint16_t; };
	template<> struct Integer_construction<false, 32> { using type = std::uint32_t; };
	template<> struct Integer_construction<false, 64> { using type = std::uint64_t; };

	template<> struct Integer_construction<true, 8> { using type = std::int8_t; };
	template<> struct Integer_construction<true, 16> { using type = std::int16_t; };
	template<> struct Integer_construction<true, 32> { using type = std::int32_t; };
	template<> struct Integer_construction<true, 64> { using type = std::int64_t; };
}