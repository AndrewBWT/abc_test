#pragma once
#include "abc_test/matchers/generic_matcher.h"

_BEGIN_ABC_NS
class bool_matcher_t
	: public generic_matcher_t
{
	__constexpr
		bool_matcher_t(
			const bool _a_pass
		) noexcept;
	__constexpr
		virtual
		matcher_result_t
		run(
			test_runner_t& _a_test_runner
		);
private:
	bool _m_pass;
};
template<
	typename Assertion_Status
>
requires std::same_as<Assertion_Status,reports::pass_t> ||
std::same_as<Assertion_Status,reports::fail_t>
class static_matcher_t
	: public generic_matcher_t
{
public:
	__constexpr
		static_matcher_t(
			const std::string_view _a_fail_msg
		) noexcept;
};

struct true_matcher_t
	: public generic_matcher_t
{
public:
	__constexpr
		virtual
		matcher_result_t
		run(
			test_runner_t& _a_test_runner
		);
};
struct false_matcher_t
	: public generic_matcher_t
{
	__constexpr
		virtual
		matcher_result_t
		run(
			test_runner_t& _a_test_runner
		);
};
_END_ABC_NS

_BEGIN_ABC_NS
__constexpr_imp
bool_matcher_t::bool_matcher_t(
	const bool _a_pass
) noexcept
	: _m_pass(_a_pass)
{

}
__constexpr_imp
matcher_result_t
bool_matcher_t::run(
	test_runner_t& _a_test_runner
)
{
	return matcher_result_t(true, _m_pass, fmt::format("{0}", _m_pass));
}
__constexpr_imp
matcher_result_t
true_matcher_t::run(
	test_runner_t& _a_test_runner
)
{
	return matcher_result_t(true, true, "true");
}
__constexpr_imp
matcher_result_t
false_matcher_t::run(
	test_runner_t& _a_test_runner
)
{
	return matcher_result_t(true, false, "false");
}
_END_ABC_NS