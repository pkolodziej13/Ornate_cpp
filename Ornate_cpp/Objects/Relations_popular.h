#pragma once
#include "Relation_with_events.h"
#include "Relation_symmetry.h"
#include "Relation_asymmetry.h"
#include "Relation_final.h"
#include "Relation_profiled.h"

#include "Relation_side_single.h"
#include "Relation_side_map.h"
#include "Relation_side_pool.h"
#include "Relation_side_dense_list.h"
#include "Relation_side_dense_indexed.h"

namespace obj
{
	template<class Relation1_type, class Relation2_type, class T1, class T2, class Value = void, Value_possesing possesing = Value_possesing::optimal>
	using Relation_asymmetric = Relation_final<Relation_asymmetry<Relation1_type, Relation2_type, T1, T2, Value, possesing>>;
	template<class Relation1_type, class Relation2_type, class T1, class T2, class Value = void, Value_possesing possesing = Value_possesing::optimal>
	using Relation_symmetric = Relation_final<Relation_symmetry<Relation1_type, Relation2_type, T1, T2, Value, possesing>>;

	template< class Eventer,class Relation1_type, class Relation2_type, class T1, class T2, class Value = void, Value_possesing possesing = Value_possesing::optimal>
	using Relation_profiled_symetric_events = obj::Relation_profiled <Relation_final<
		Relation_with_events<Eventer, Relation_symmetry<Relation1_type, Relation2_type, T1, T2, Value, possesing>>
		>>;

}