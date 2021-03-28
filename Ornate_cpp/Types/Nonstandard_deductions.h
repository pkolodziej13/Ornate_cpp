#pragma once

namespace typ
{
	template<class T>
	struct Template_of_template_deduction;
	template<template<template<class >class  >class Temp_external, template<class >class Temp_internal>
	struct Template_of_template_deduction<Temp_external<Temp_internal>>
	{
		template<class T>
		using template_type = Temp_internal<T>;
	};
}