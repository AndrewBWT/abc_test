#pragma once
#include "abc_test/core/errors/test_library_exception.h"

#include "abc_test/matchers/generic_matcher.h"
#include "abc_test/matchers/logic/logic_matcher.h"

_BEGIN_ABC_NS
using matcher_internal_ptr_t = std::shared_ptr<generic_matcher_t>;
using matcher_internal_ptr_const_ref_t = const matcher_internal_ptr_t&;
struct matcher_t
{
public:
	__constexpr
		matcher_t(
		) noexcept;
	__constexpr
		matcher_t(
			matcher_internal_ptr_t _a_matcher_internal
		) noexcept;
	__constexpr
		matcher_t(
			const matcher_t& _a_matcher_internal,
			const std::string_view _a_str_representation,
			const std::source_location& _a_source_location = std::source_location::current()
		) noexcept;
	__constexpr
		matcher_internal_ptr_const_ref_t
		internal_matcher(
		) const noexcept;
	__constexpr
		bool
		or_statement(
			test_runner_t& _a_test_runner = abc::global::get_this_threads_test_runner_ref(),
			const std::source_location& _a_source_location = std::source_location::current()
		) noexcept;
	__constexpr
		bool
		and_statement(
			test_runner_t& _a_test_runner = abc::global::get_this_threads_test_runner_ref(),
			const std::source_location& _a_source_location = std::source_location::current()
		) noexcept;
	__constexpr
		void
		process(
			const matcher_t& _a_matcher
		);
	__constexpr
		matcher_t
		operator!(
			) const noexcept
	{
		return matcher_t(matcher_internal_ptr_t(
			new logic_matcher_t<NOT>(this->internal_matcher())));
	}
	__constexpr
		matcher_t
		operator&&(
			const matcher_t& _a_matcher
			) const noexcept;
	__constexpr
		matcher_t
		operator||(
			const matcher_t& _a_matcher
			) const noexcept;
private:
	matcher_internal_ptr_t _m_matcher_internal;
	template<
		logic_enum_t Logic_Enum
	>
	__constexpr
		bool
		process_(
			const matcher_t& _a_matcher
		) noexcept;
};
__constexpr
	matcher_t
	matcher(
		generic_matcher_t* _a_generic_matcher_ptr
	) noexcept;
_END_ABC_NS

_BEGIN_ABC_NS
__constexpr_imp
	matcher_t::matcher_t(
	) noexcept
	: _m_matcher_internal(matcher_internal_ptr_t(nullptr))
{}
__constexpr_imp
	matcher_t::matcher_t(
		matcher_internal_ptr_t _a_matcher_internal
	) noexcept
	: _m_matcher_internal(_a_matcher_internal)
{
}
__constexpr_imp
	matcher_t::matcher_t(
		const matcher_t& _a_matcher_internal,
		const std::string_view _a_str_representation,
		const std::source_location& _a_source_location
	) noexcept
	: _m_matcher_internal(_a_matcher_internal.internal_matcher())
{
	_m_matcher_internal->add_source_info(_a_str_representation, _a_source_location);
}
__constexpr_imp
	matcher_internal_ptr_const_ref_t
	matcher_t::internal_matcher(
	) const noexcept
{
	return _m_matcher_internal;
}
__constexpr_imp
	bool
	matcher_t::or_statement(
		test_runner_t& _a_test_runner,
		const std::source_location& _a_source_location
	) noexcept
{
	const bool _l_result{ _m_matcher_internal->run_test(_a_test_runner).passed() };
	*this = matcher(new logic_matcher_t<OR>(this->internal_matcher(),
		std::shared_ptr<generic_matcher_t>()));
	_m_matcher_internal->add_source_info("or_statement", _a_source_location);
	return not _l_result;
}
__constexpr_imp
	bool
	matcher_t::and_statement(
		test_runner_t& _a_test_runner,
		const std::source_location& _a_source_location
	) noexcept
{
	const bool _l_result{ _m_matcher_internal->run_test(_a_test_runner).passed() };
	*this = matcher(new logic_matcher_t<AND>(this->internal_matcher(),
		std::shared_ptr<generic_matcher_t>()));
	_m_matcher_internal->add_source_info("and_statement", _a_source_location);
	return _l_result;
}
__constexpr_imp
	void
	matcher_t::process(
		const matcher_t& _a_matcher
	)
{
	using enum logic_enum_t;
	if (not (process_<OR>(_a_matcher) || process_<AND>(_a_matcher)))
	{
	//	std::cout << "where" << std::endl;
		throw errors::test_library_exception_t(
			fmt::format("Could not run process; the parent node we are trying to add an OR or AND "
				"node to is not a logic_matcher_t. Please check your code, or contact the developer."));
	}
}
__constexpr_imp
	matcher_t
	matcher_t::operator&&(
		const matcher_t& _a_matcher
	) const noexcept
{
	return matcher(new logic_matcher_t<AND>(this->internal_matcher(),
		_a_matcher.internal_matcher()));
}
__constexpr_imp
	matcher_t
	matcher_t::operator||(
		const matcher_t& _a_matcher
	) const noexcept
{
	return matcher(new logic_matcher_t<OR>(this->internal_matcher(),
			_a_matcher.internal_matcher()));
}
template<
	logic_enum_t Logic_Enum
>
__constexpr_imp
bool
matcher_t::process_(
	const matcher_t& _a_matcher
) noexcept
{
	generic_matcher_t* _l_matcher_ptr{ this->internal_matcher().get() };
	logic_matcher_t<Logic_Enum>* _l_ptr_or{ dynamic_cast<logic_matcher_t<Logic_Enum>*>(_l_matcher_ptr) };
	if (_l_ptr_or != nullptr)
	{
		_l_ptr_or->set_right_child(_a_matcher.internal_matcher());
		return true;
	}
	else
	{
		return false;
	}
}
__constexpr_imp
	matcher_t
	matcher(
		generic_matcher_t* _a_generic_matcher_ptr
	) noexcept
{
	return matcher_t(matcher_internal_ptr_t(_a_generic_matcher_ptr));
}
_END_ABC_NS