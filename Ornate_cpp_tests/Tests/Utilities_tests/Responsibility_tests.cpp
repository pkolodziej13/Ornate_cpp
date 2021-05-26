#include "pch.h"
#include "CppUnitTest.h"

#include <Utilities/Responsibility.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

template<class Responsibility>
uti::responsibility_result<Responsibility, int> verifying_correct_value_function()
{
	int component_a = 10;
	if (!Responsibility::verify([&]() {return component_a < 20; }))
		if constexpr (Responsibility::manual_return)
			return Responsibility::template fail_result<int>();

	int component_b = 15;
	if (!Responsibility::verify([&]() {return component_a < 30; }))
		if constexpr (Responsibility::manual_return)
			return Responsibility::template fail_result<int>();

	auto result = Responsibility::template prepare_result<int>(0);
	Responsibility::template value_of_result<int>(result) += 10;
	return result;
}

template<class Responsibility>
uti::responsibility_result<Responsibility, void> verifying_correct_void_function()
{
	int component_a = 10;
	if (!Responsibility::verify([&]() {return component_a < 20; }))
		if constexpr (Responsibility::manual_return)
			return Responsibility::template fail_result<void>();

	return Responsibility::template prepare_result<void>();
}

template<class Responsibility>
uti::responsibility_result<Responsibility, void> verifying_faulty_void_function()
{
	int component_a = 40;
	if (!Responsibility::verify([&]() {return component_a < 20; }))
		if constexpr (Responsibility::manual_return)
			return Responsibility::template fail_result<void>();

	return Responsibility::template prepare_result<void>();
}




TEST_CLASS(Responsibility_tests)
{
public:

	TEST_METHOD(test_correct_value_function)
	{
		int a = verifying_correct_value_function<uti::Certain>();
		int b = verifying_correct_value_function<uti::Programmer_responsibility>();
		int c = verifying_correct_value_function<uti::Input_responsibility>();
		std::optional<int> d = verifying_correct_value_function<uti::No_responsibility>();

		Assert::AreEqual(a, 10);
		Assert::AreEqual(b, 10);
		Assert::AreEqual(c, 10);
		Assert::AreEqual(d.value(), 10);
	}
	TEST_METHOD(test_correct_void_function)
	{
		verifying_correct_void_function<uti::Certain>();
		verifying_correct_void_function<uti::Programmer_responsibility>();
		verifying_correct_void_function<uti::Input_responsibility>();
		bool d = verifying_correct_void_function<uti::No_responsibility>();
	}

	TEST_METHOD(test_faulty_void_input_function)
	{
		bool caught_exception = false;
		try
		{
			verifying_faulty_void_function<uti::Input_responsibility>();
		}
		catch(...)
		{
			caught_exception = true;
		}
		Assert::IsTrue(caught_exception);
	}
	TEST_METHOD(test_faulty_void_no_responsibility_function)
	{
		bool d = verifying_faulty_void_function<uti::No_responsibility>();
		Assert::IsFalse(d);
	}
};