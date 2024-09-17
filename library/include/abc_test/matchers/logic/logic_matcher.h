#pragma once

#include <type_traits>
#include <memory>

#include "abc_test/matchers/operator_based_matcher.h"

_BEGIN_ABC_NS
enum logic_enum_t
{
	NOT,
	OR,
	AND
};
class logic_matcher_t : public operator_based_matcher_t
{
public:
	__constexpr
		logic_matcher_t(
		) noexcept = delete;
	__constexpr
		logic_matcher_t(
			const logic_enum_t _a_logic_enum,
			const std::shared_ptr<generic_matcher_t>& _a_arg
		) noexcept;
	__constexpr
		logic_matcher_t(
			const logic_enum_t _a_logic_enum,
			const std::shared_ptr<generic_matcher_t>& _a_matcher_l,
			const std::shared_ptr<generic_matcher_t>& _a_matcher_r
		) noexcept;
	__constexpr
		virtual
		matcher_result_t
		run(
			test_runner_ref_t& _a_test_runner
		) noexcept;
	__constexpr
		virtual
		precedence_t
		get_precedence(
		) const noexcept override final;
private:
	std::shared_ptr<generic_matcher_t> _m_matcher_l;
	std::shared_ptr<generic_matcher_t> _m_matcher_r;
	logic_enum_t _m_logic_enum;
};
_END_ABC_NS

_BEGIN_ABC_NS
__constexpr_imp
	logic_matcher_t::logic_matcher_t(
		const logic_enum_t _a_logic_enum,
		const std::shared_ptr<generic_matcher_t>& _a_matcher
	) noexcept
	: _m_matcher_l(_a_matcher)
	, _m_logic_enum(_a_logic_enum)
{

}
__constexpr_imp
	logic_matcher_t::logic_matcher_t(
		const logic_enum_t _a_logic_enum,
		const std::shared_ptr<generic_matcher_t>& _a_matcher_l,
		const std::shared_ptr<generic_matcher_t>& _a_matcher_r
	) noexcept
	: _m_logic_enum(_a_logic_enum)
	, _m_matcher_l(_a_matcher_l)
	, _m_matcher_r(_a_matcher_r)
{

}
__constexpr_imp
matcher_result_t
	logic_matcher_t::run(
		test_runner_ref_t& _a_test_runner
) noexcept
{
	using namespace std;
	using enum logic_enum_t;
	string _l_rv_str;
	matcher_result_t _l_mrv;
	bool _l_passed = true;
	switch (_m_logic_enum)
	{
	case NOT:
	{
		_l_mrv =  _m_matcher_l->run(_a_test_runner);
		_l_passed = not _l_mrv.passed();
		operator_based_matcher_t* _l_ptr{ dynamic_cast<operator_based_matcher_t*>(_m_matcher_l.get()) };
		if (_l_ptr != nullptr)
		{
			const size_t _l_child_precedence{ _l_ptr->get_precedence() };
			if (get_precedence() <= _l_child_precedence)
			{
				_l_rv_str = "!(" + std::string(_l_mrv.str()) + ")";
			}
			else
			{
				_l_rv_str = "!" + std::string(_l_mrv.str());
			}
		}
		break;
	}
	case AND:
	{
		matcher_result_t _l_ml{ _m_matcher_l->run(_a_test_runner) };
		if (not _l_ml.passed())
		{
			_l_mrv = _l_ml;
			_l_passed = false;
			_l_rv_str = std::string(_l_ml.str()) + " && <unevaluated>";
		}
		else
		{
			matcher_result_t _l_ml1{ _m_matcher_r->run(_a_test_runner) };
			_l_passed = _l_ml1.passed();
			_l_rv_str = std::string(_l_ml.str()) + " && " + std::string(_l_ml1.str());
		}
	}
	break;
	case OR:
	{
		matcher_result_t _l_ml{ _m_matcher_l->run(_a_test_runner) };
		if (_l_ml.passed())
		{
			_l_mrv = _l_ml;
			_l_passed = true;
			_l_rv_str = std::string(_l_ml.str()) + " || <unevaluated>";
		}
		else
		{
			matcher_result_t _l_ml1{ _m_matcher_r->run(_a_test_runner) };
			_l_passed = _l_ml1.passed();
			_l_rv_str = std::string(_l_ml.str()) + " || " + std::string(_l_ml1.str());
		}
	}
	break;
	}
	return matcher_result_t(true, _l_passed, _l_rv_str);
}
__constexpr_imp
	precedence_t
	logic_matcher_t::get_precedence(
	) const noexcept 
{
	using enum logic_enum_t;
	switch (_m_logic_enum)
	{
	case NOT:
		return 3;
	case OR:
		return 15;
	case AND:
		return 14;
	}
}
_END_ABC_NS