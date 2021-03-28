#pragma once

#define Optional_member_variable(member)\
template<class Type_of_member>\
struct Optional_member_##member\
{Type_of_member member{};};\
template<>struct Optional_member_##member<void>{};