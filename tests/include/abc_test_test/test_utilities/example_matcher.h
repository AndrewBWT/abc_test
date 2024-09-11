#pragma once

#include "abc_test/matcher/operator_based_matcher.h"

namespace ab::test
{
	struct example_matcher : public generic_matcher_t
	{
	public:
		inline constexpr example_matcher(
		) noexcept
			: generic_matcher_t()
			, _m_msg("")
			, _m_will_pass(false)
		{
		}
		inline constexpr example_matcher(
			const std::string_view _a_str,
			const bool _a_will_pass
		) noexcept
		{
			_m_msg = _a_str;
			_m_will_pass = _a_will_pass;
		}
	private:
		std::string _m_msg;
		bool _m_will_pass;
		inline constexpr
			error_return_val
			run(
				ab::test::test_runner_t&
			) noexcept
		{
			return { _m_will_pass, _m_msg };
		}
	};
	struct example_op_matcher : public operator_based_matcher_t
	{
		inline constexpr example_op_matcher(
		) noexcept
			: operator_based_matcher_t()
			, _m_msg("")
			, _m_will_pass(false)
			, _m_precedence(0)
		{
		}
		inline constexpr example_op_matcher(
			const std::string_view _a_str_l,
			const std::string_view _a_str_r,
			const bool _a_will_pass,
			const precedence_t _a_precedence
		) noexcept
		{
			using namespace std;
			_m_msg = string(_a_str_l) + " <" + to_string(_a_precedence) + "> " + string(_a_str_r);
			_m_will_pass = _a_will_pass;
			_m_precedence = _a_precedence;
		}
		inline constexpr precedence_t
			get_precedence(
			) const noexcept
		{
			return _m_precedence;
		}
	private:
		std::string _m_msg;
		bool _m_will_pass;
		precedence_t _m_precedence;
		inline constexpr
			error_return_val
			run(
				ab::test::test_runner_t&
			) noexcept
		{
			return { _m_will_pass, _m_msg };
		}
	};
}