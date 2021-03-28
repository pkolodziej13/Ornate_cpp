#pragma once
namespace uti
{
	struct non_copyable
	{
		non_copyable() = default;
		non_copyable(const non_copyable&) = delete;
		non_copyable& operator =(const non_copyable&) = delete;
	};
	struct non_movable
	{
		non_movable() = default;
		non_movable(non_movable&&) = delete;
		non_movable& operator =(non_movable&&) = delete;
	};
	struct non_replicable :non_copyable, non_movable
	{
	};
}