#pragma once

#include "abc_test/matchers/operator_based_matcher.h"

#include "abc_test/matchers/comparison/comparison_enum.h"

_BEGIN_ABC_NS
template<
	typename T1,
	typename T2,
	comparison_enum_t Cmp
>
struct comparison_matcher_t : public operator_based_matcher_t
{
public:
	/*!
	* Constructor for entity, taking one element of T1 and one of T2.
	*/
	__constexpr
		comparison_matcher_t(
			T1&& _a_l,
			T2&& _a_r
		) noexcept;
	/*!
	* Gets the precedence of the templated comparison_enum_type.
	*/
	__constexpr
		precedence_t
		get_precedence(
		) const noexcept;
private:
	T1 _m_l;
	T2 _m_r;
	__constexpr
		virtual
		matcher_result_t
		run(
			test_runner_t&
		) noexcept;
};
_END_ABC_NS

_BEGIN_ABC_NS
template<
	typename T1,
	typename T2,
	comparison_enum_t Cmp
>
__constexpr_imp
	comparison_matcher_t<T1, T2, Cmp>::comparison_matcher_t(
		T1&& _a_l,
		T2&& _a_r
	) noexcept
	: _m_l(_a_l)
	, _m_r(_a_r)
{

}
template<
	typename T1,
	typename T2,
	comparison_enum_t Cmp
>
__constexpr_imp
	precedence_t
	comparison_matcher_t<T1, T2, Cmp>::get_precedence(
	) const noexcept
{
	return cmp_precedence<Cmp>();
}
template<
	typename T1,
	typename T2,
	comparison_enum_t Cmp
>
__constexpr_imp
	matcher_result_t
	comparison_matcher_t<T1, T2, Cmp>::run(
		test_runner_t&
	) noexcept
{
	using namespace std;
	string _l_left_str{ "[?]" };
	string _l_right_str{ "[?]" };
	if constexpr (fmt::formattable<T1>)
	{
		_l_left_str = fmt::format("{}", _m_l);
	}
	if constexpr (fmt::formattable<T2>)
	{
		_l_right_str = fmt::format("{}", _m_r);
	}
	return matcher_result_t(true,
		cmp<T1, T2, Cmp>(forward<T1>(_m_l), forward<T2>(_m_r)),
		fmt::format(
			"{0} {1} {2}",
			_l_left_str,
			cmp_str<Cmp>(),
			_l_right_str)
	);
}
_END_ABC_NS