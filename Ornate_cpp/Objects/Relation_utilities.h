#pragma once
#include <Utilities\Result_as_success.h>
#include <Utilities\Optional_member_variable.h>

namespace obj
{
	Optional_member_variable(key);
	Optional_member_variable(value);
	template<class Key, class Value>
	struct Relation_concomitant :Optional_member_key<Key>, Optional_member_value<Value>
	{};

	template<class Related, class Concomitant>
	struct Relation_keepment :Concomitant
	{
		Relation_keepment() = default;
		Relation_keepment(Related & related) :related(&related) {}
		Relation_keepment(Related & related, const Concomitant &concomitant) :related(&related), Concomitant(concomitant) {}

		bool connected()
		{
			return related != nullptr;
		}

		Related * related{};
	};

	template<class Key,class Result>
	struct Key_with_result
	{
		Key key;
		uti::Result_as_success<Result> value;
	};
	template< class Result>
	struct Key_with_result<void, Result>
	{
		uti::Result_as_success<Result> value;
	};	
}