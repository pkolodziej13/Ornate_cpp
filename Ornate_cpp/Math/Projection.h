#pragma once
#include "Range.h"
#include "Turn.h"
#include "Used_linear_algebra.h"


namespace mth::geo3
{
	struct Projection
	{
		Projection() = default;
		Projection(const mat4& m)
		{
			throw "not implemented";
			//near far
			//2n / (r - l) 0 0 0
			//	0 2n / (t - b) 0 0
			//	(l + r) / (r - l) (t + b) / (t - b) (n + f) / (n - f) - 1
			//	0 0 (2nf) / (n - f) 0

			//	So a = (n + f) / (n - f), b = (2nf) / (n - f).
			//	(1 / ((a + 1)(a - 1))) / b = c = (n - f) / 2
			//	(a + 1) / c = n, (a - 1) / c = f
		}
		Projection(const Range<float>& near_far, vec2 size = { 1.f,1.f })
			:near_far(near_far), alternative_size(size), angles(0, 0)
		{}
		Projection(const Range<float>& near_far, g_vec<Turn, 2> angles)
			:near_far(near_far), angles(angles)
		{}
		Projection(const Range<float>& near_far, vec2 port_proportions, g_vec<Turn, 2> angles_limits)
			:near_far(near_far)
		{
			vec2 prop{ (angles_limits.x / 2.f).sin(),(angles_limits.y / 2.f).sin() };
			vec2 propp = prop / prop.y;
			port_proportions = port_proportions / port_proportions.y;
			if (propp.x > port_proportions.x)
			{
				prop.x /= (propp.x / port_proportions.x);
			}
			else
			{
				prop.y /= (port_proportions.x / propp.x);
			}
			angles = g_vec<Turn, 2>(Turn::from_sin(prop.x), Turn::from_sin(prop.y)) * 2.f;
		}

		mat4 matrix()
		{
			if (angles.x == 0 || angles.y == 0)
			{
				vec2 xy = alternative_size / 2.f;
				return orthographic_projection<float>({ -xy.x, xy.x }, { -xy.y, xy.y }, { near_far.min, near_far.max });
			}
			else
			{
				vec2 xy{ (angles.x / 2.f).sin(),(angles.y / 2.f).sin() };
				xy = xy * near_far.min;
				return perspective_projection<float>({ -xy.x, xy.x }, { -xy.y, xy.y }, { near_far.min, near_far.max });
			}
			return {};
		}
		operator mat4()
		{
			return matrix();
		}

		g_vec<Turn, 2> angles{ 0.25f,0.25f };
		vec2 alternative_size{ 2,2 };
		Range<float> near_far{ 0.01f,100 };
	};
}