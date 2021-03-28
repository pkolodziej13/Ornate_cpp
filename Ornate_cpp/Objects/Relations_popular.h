#pragma once
#include "Relation_with_events.h"
#include "Relation_profiled.h"
#include "Relation_symetric.h"
#include "Relation_final.h"
namespace obj
{
	template< class Eventer,class Relation1_type, class Relation2_type, class T1, class T2, class Value = void, Value_possesing possesing = Value_possesing::optimal>
	using Relation_profiled_symetric_events = obj::Relation_profiled <Relation_final<
		Relation_with_events<Eventer, Relation_symetric_wings<Relation1_type, Relation2_type, T1, T2, Value, possesing>>
		>>;

}