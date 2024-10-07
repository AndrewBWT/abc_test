#pragma once
#include "abc_test/core/errors/test_library_exception.h"
#include "abc_test/matchers/logic/logic_matcher.h"
#include "abc_test/matchers/basic_matchers.h"
#include "abc_test/global.h"

_BEGIN_ABC_NS
using matcher_internal_ptr_t = std::shared_ptr<generic_matcher_t>;
using matcher_internal_ptr_const_ref_t = const matcher_internal_ptr_t&;
struct matcher_t
{
public:
	__no_constexpr
		matcher_t(
		) noexcept;
	__no_constexpr
		explicit
		matcher_t(
			const bool _a_bool
		) noexcept;
	__no_constexpr
		matcher_t(
			generic_matcher_t* _a_matcher_internal
		) noexcept;
	__no_constexpr
		matcher_t(
			matcher_internal_ptr_t _a_matcher_internal
		) noexcept;
	__no_constexpr_imp
		matcher_t(
			const matcher_t& _a_matcher,
			const reports::single_source_t& _a_single_source
		)
		: _m_matcher_internal(_a_matcher.internal_matcher())
		, _m_initialised_with_source(true)
	{
		if (not _a_matcher._m_initialised_with_source)
		{
			_m_matcher_internal->add_source_info(_a_single_source);
		}
		else
		{
			std::cout << "Issued warning" << std::endl;
		}
	}
	__constexpr
		matcher_internal_ptr_const_ref_t
		internal_matcher(
		) const noexcept;
	__no_constexpr
		bool
		or_statement(
			const reports::single_source_t& _a_single_source,
			test_runner_t& _a_test_runner = abc::global::get_this_threads_test_runner_ref()
		) noexcept;
	__no_constexpr
		bool
		and_statement(
			const reports::single_source_t& _a_single_source,
			test_runner_t& _a_test_runner = abc::global::get_this_threads_test_runner_ref()
		) noexcept;
	__constexpr
		void
		process(
			const matcher_t& _a_matcher,
			const reports::single_source_t& _a_single_source
		);
	__no_constexpr_imp
		matcher_t
		operator!(
			) const noexcept
	{
		return matcher_t(matcher_internal_ptr_t(
			new logic_matcher_t<NOT>(this->internal_matcher())));
	}
	__no_constexpr
		matcher_t
		operator&&(
			const matcher_t& _a_matcher
			) const noexcept;
	__no_constexpr
		matcher_t
		operator||(
			const matcher_t& _a_matcher
			) const noexcept;
private:
	matcher_internal_ptr_t _m_matcher_internal;
	bool _m_initialised_with_source;
	template<
		logic_enum_t Logic_Enum
	>
	__constexpr
		bool
		process_(
			const matcher_t& _a_matcher
		) noexcept;
};
__no_constexpr_imp
matcher_t
matcher(
	generic_matcher_t* _a_generic_matcher_ptr
) noexcept;
__no_constexpr_imp
matcher_t
true_matcher(
) noexcept;
__no_constexpr_imp
matcher_t
false_matcher(
) noexcept;
__no_constexpr_imp
matcher_t
bool_matcher(
	const bool _a_bool
) noexcept;
_END_ABC_NS

_BEGIN_ABC_NS
__no_constexpr_imp
matcher_t::matcher_t(
) noexcept
	: matcher_t(true)
{
}
__no_constexpr_imp
matcher_t::matcher_t(
	const bool _a_bool
) noexcept
	: matcher_t(_a_bool ?
		matcher_internal_ptr_t(new true_matcher_t()) :
		matcher_internal_ptr_t(new false_matcher_t())
	)
{

}
__no_constexpr_imp
matcher_t::matcher_t(
	generic_matcher_t* _a_matcher_internal
) noexcept
	: matcher_t(matcher_internal_ptr_t(_a_matcher_internal))
{

}
__no_constexpr_imp
matcher_t::matcher_t(
	matcher_internal_ptr_t _a_matcher_internal
) noexcept
	: _m_matcher_internal(_a_matcher_internal)
	, _m_initialised_with_source(false)
{

}
/*__constexpr_imp
	matcher_t::matcher_t(
		matcher_internal_ptr_t _a_matcher_internal,
		const std::optional<reports::single_source_t>& _a_source
	) noexcept
	: _m_matcher_internal(_a_matcher_internal)
{
	_m_matcher_internal->add_source_info(_a_source);
	//if (_a_source.has_value())
	//{
	//	_m_matcher_internal->add_source_info(_a_source.value());
	//}
}*/
__constexpr_imp
	matcher_internal_ptr_const_ref_t
	matcher_t::internal_matcher(
	) const noexcept
{
	return _m_matcher_internal;
}
__no_constexpr_imp
	bool
	matcher_t::or_statement(
		const reports::single_source_t& _a_single_source,
		test_runner_t& _a_test_runner
	) noexcept
{
	const bool _l_result{ _m_matcher_internal->run_test(_a_test_runner).passed() };
	*this = matcher(new logic_matcher_t<OR>(this->internal_matcher(),
		std::shared_ptr<generic_matcher_t>()));
	_m_matcher_internal->add_source_info(_a_single_source);
	return not _l_result;
}
__no_constexpr_imp
	bool
	matcher_t::and_statement(
		const reports::single_source_t& _a_single_source,
		test_runner_t& _a_test_runner
	) noexcept
{
	const bool _l_result{ _m_matcher_internal->run_test(_a_test_runner).passed() };
	*this = matcher(new logic_matcher_t<AND>(this->internal_matcher(),
		std::shared_ptr<generic_matcher_t>()));
	_m_matcher_internal->add_source_info(_a_single_source);
	return _l_result;
}
__constexpr_imp
	void
	matcher_t::process(
		const matcher_t& _a_matcher,
		const reports::single_source_t& _a_single_source
	)
{
	using enum logic_enum_t;
	if (not (process_<OR>(_a_matcher) || process_<AND>(_a_matcher)))
	{
		throw errors::test_library_exception_t(
			fmt::format("Could not run process; the parent node we are trying to add an OR or AND "
				"node to is not a logic_matcher_t. Please check your code, or contact the developer."));
	}
	else
	{
		this->_m_matcher_internal->add_source_info(_a_single_source);
	}
}
__no_constexpr_imp
	matcher_t
	matcher_t::operator&&(
		const matcher_t& _a_matcher
	) const noexcept
{
	return matcher(new logic_matcher_t<AND>(this->internal_matcher(),
		_a_matcher.internal_matcher()));
}
__no_constexpr_imp
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
__no_constexpr_imp
	matcher_t
	matcher(
		generic_matcher_t* _a_generic_matcher_ptr
	) noexcept
{
	return matcher_t(matcher_internal_ptr_t(_a_generic_matcher_ptr));
}
__no_constexpr_imp
matcher_t
true_matcher(
) noexcept
{
	return matcher(new true_matcher_t());
}
__no_constexpr_imp
matcher_t
false_matcher(
) noexcept
{
	return matcher(new false_matcher_t());
}
__no_constexpr_imp
matcher_t
bool_matcher(
	const bool _a_bool
) noexcept
{
	return matcher(new bool_matcher_t(_a_bool));
}
_END_ABC_NS