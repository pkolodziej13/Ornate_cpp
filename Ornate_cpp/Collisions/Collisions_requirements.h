#pragma once

namespace col
{

	struct Colider_unified;
	template<class T>
	struct Collider;

	struct Object_unified;
	template<class T>
	struct Object_individual;

	struct Engine_unified;
	template<class T>
	struct Engine;
	template<class T, class Name_braid_base_t>
	struct Engine_braid_fiber;
	template<class Objects_p, class Coliders_p>
	struct Engine_braid;
}