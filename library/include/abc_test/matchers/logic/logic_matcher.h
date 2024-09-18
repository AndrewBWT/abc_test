#pragma once

#include <type_traits>
#include <memory>

#include "abc_test/matchers/logic/logic_enum.h"

_BEGIN_ABC_NS
template<
	logic_enum_t Logic_Enum
>
class logic_matcher_t : public operator_based_matcher_t
{
public:
	__constexpr
		logic_matcher_t(
		) noexcept = delete;
	__constexpr
		logic_matcher_t(
			const std::shared_ptr<generic_matcher_t>& _a_arg
		) noexcept;
	__constexpr
		logic_matcher_t(
			const std::shared_ptr<generic_matcher_t>& _a_matcher_l,
			const std::shared_ptr<generic_matcher_t>& _a_matcher_r
		) noexcept;
	__constexpr
		virtual
		precedence_t
		get_precedence(
		) const noexcept override final;
	__constexpr
		void
		set_right_child(
			const std::shared_ptr<generic_matcher_t>& _a_matcher
		) noexcept;
	__constexpr
		virtual
		void
		gather_map_source(
			matcher_source_map_t& _a_matcher_source_map
		) const noexcept override final;
private:
	std::shared_ptr<generic_matcher_t> _m_matcher_l;
	std::shared_ptr<generic_matcher_t> _m_matcher_r;
	__constexpr
		virtual
		matcher_result_t
		run(
			test_runner_t& _a_test_runner
		) override;
	template<
		bool Result_On_First_Mathcer_To_Move_Into_If
	>
	__constexpr
		matcher_result_t
		run_binary_logic_matcher(
			test_runner_t& _a_test_runner
		);
};
namespace 
{
	std::pair<const char*, const char*> _c_normal_bracket_pair{ "(",")" };
	std::pair<const char*, const char*> _c_empty_bracket_pair{ "","" };
	template<
		logic_enum_t Logic_Enum
	>
	__constexpr
		std::string
		make_str(
			std::shared_ptr<generic_matcher_t>& _a_ptr,
			const std::string_view _a_str
		) noexcept;
}
_END_ABC_NS

_BEGIN_ABC_NS
template<
	logic_enum_t Logic_Enum
>
__constexpr_imp
	logic_matcher_t<Logic_Enum>::logic_matcher_t(
		//const logic_enum_t _a_logic_enum,
		const std::shared_ptr<generic_matcher_t>& _a_matcher
	) noexcept
	: _m_matcher_l(_a_matcher)
	//, _m_logic_enum(_a_logic_enum)
{

}
template<
	logic_enum_t Logic_Enum
>
__constexpr_imp
	logic_matcher_t<Logic_Enum>::logic_matcher_t(
	//	const logic_enum_t _a_logic_enum,
		const std::shared_ptr<generic_matcher_t>& _a_matcher_l,
		const std::shared_ptr<generic_matcher_t>& _a_matcher_r
	) noexcept
	// _m_logic_enum(_a_logic_enum)
	: _m_matcher_l(_a_matcher_l)
	, _m_matcher_r(_a_matcher_r)
{

}
template<
	logic_enum_t Logic_Enum
>
__constexpr_imp
	precedence_t
	logic_matcher_t< Logic_Enum>::get_precedence(
	) const noexcept 
{
	return logic_precedence<Logic_Enum>();
}
template<
	logic_enum_t Logic_Enum
>
__constexpr_imp
	void
	logic_matcher_t<Logic_Enum>::set_right_child(
		const std::shared_ptr<generic_matcher_t>& _a_matcher
	) noexcept
{
	if constexpr (Logic_Enum == NOT)
	{
		static_assert(false, "Cannot set the right child of a NOT logic statement");
	}
	else
	{
		_m_matcher_r = _a_matcher;
	}
}
template<
	logic_enum_t Logic_Enum
>
__constexpr_imp
	matcher_result_t
	logic_matcher_t<Logic_Enum>::run(
		test_runner_t& _a_test_runner
	)
{
	using namespace std;
	using enum logic_enum_t;
	if constexpr (Logic_Enum == NOT)
	{
		const matcher_result_t& _l_child_res{ _m_matcher_l->run_test(_a_test_runner)};
		return matcher_result_t(true, not _l_child_res.passed(),
			fmt::format("{0}{1}",
				logic_str<Logic_Enum>(), make_str<Logic_Enum>(_m_matcher_l, _l_child_res.str())));
	}
	else if constexpr (Logic_Enum == OR)
	{
		return run_binary_logic_matcher<false>(_a_test_runner);
	}
	else if constexpr (Logic_Enum == AND)
	{
		return run_binary_logic_matcher<true>(_a_test_runner);
	}
	else
	{
		static_assert(false, "logic_matcher_t::run function does not work for given template type.");
		return matcher_result_t();
	}
}
template<
	logic_enum_t Logic_Enum
>
__constexpr_imp
void
	logic_matcher_t< Logic_Enum>::gather_map_source(
	matcher_source_map_t& _a_matcher_source_map
) const noexcept
{
	generic_matcher_t::gather_map_source(_a_matcher_source_map);
	_m_matcher_l->gather_map_source(_a_matcher_source_map);
	if (_m_matcher_r != nullptr)
	{
		_m_matcher_r->gather_map_source(_a_matcher_source_map);
	}
}
template<
	logic_enum_t Logic_Enum
>
template<
	bool Result_On_First_Mathcer_To_Move_Into_If
>
__constexpr_imp
	matcher_result_t
	logic_matcher_t< Logic_Enum>::run_binary_logic_matcher(
		test_runner_t& _a_test_runner
	)
{
	using namespace std;
	const matcher_result_t& _l_child_res_l{ _m_matcher_l->run_test(_a_test_runner)};
	bool _l_passed{ true };
	string _l_right_str{ "<unevaluated>" };
	if (_l_child_res_l.passed() == Result_On_First_Mathcer_To_Move_Into_If)
	{
		if (_m_matcher_r.get() == nullptr)
		{
			throw errors::test_library_exception_t(
				fmt::format("right matcher in binary expression has no value, when one "
					" is required.")
			);
		}
		else
		{
			_m_matcher_r->run_test(_a_test_runner);
			const matcher_result_t& _l_child_res_r{ _m_matcher_r->run_test(_a_test_runner) };
			_l_passed = _l_child_res_r.passed();
			_l_right_str = make_str<Logic_Enum>(_m_matcher_r, _l_child_res_r.str());
		}
	}
	return matcher_result_t(true, _l_passed,
		fmt::format("{0} {1} {2}",
			make_str<Logic_Enum>(_m_matcher_l, _l_child_res_l.str()),
			logic_str<Logic_Enum>(), _l_right_str));
}
namespace
{
	template<
		logic_enum_t Logic_Enum
	>
	__constexpr_imp
		std::string
		make_str(
			std::shared_ptr<generic_matcher_t>& _a_ptr,
			const std::string_view _a_str
		) noexcept
	{
		using namespace std;
		operator_based_matcher_t* _l_ptr{ dynamic_cast<operator_based_matcher_t*>(_a_ptr.get()) };
		const pair<const char*, const char*>& _l_bracket_pair{
			_l_ptr != nullptr && logic_precedence<Logic_Enum>() <= _l_ptr->get_precedence() ?
			_c_normal_bracket_pair :
			_c_empty_bracket_pair };
		return fmt::format("{0}{1}{2}",_l_bracket_pair.first, 
			_a_str, _l_bracket_pair.second);
	}
}
_END_ABC_NS