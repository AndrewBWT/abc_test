#pragma once
#include "abc_test/core/errors/test_library_exception.h"

#include "abc_test/matchers/generic_matcher.h"
#include "abc_test/matchers/logic/logic_matcher.h"

_BEGIN_ABC_NS
using matcher_internal_ptr_t = std::shared_ptr<generic_matcher_t>;
using matcher_internal_ptr_const_ref_t = const matcher_internal_ptr_t&;
struct matcher_t : public generic_matcher_t
{
public:
	__constexpr
		matcher_t(
		) noexcept;
	/*__constexpr
		matcher_t(
			matcher_internal_ptr_t _a_matcher_internal
		) noexcept;*/
	__constexpr
		matcher_t(
			matcher_internal_ptr_t _a_matcher_internal,
			const std::optional<std::source_location>& _a_source_location = 
			std::optional<std::source_location>(std::source_location::current())
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
			new logic_matcher_t<NOT>(this->internal_matcher())),
			std::optional<std::source_location>());
	}
	__constexpr
		matcher_t
		operator&&(
			const matcher_t& _a_matcher
			) const noexcept
	{
		return matcher_t(matcher_internal_ptr_t(
			new logic_matcher_t<AND>(this->internal_matcher(),
				_a_matcher.internal_matcher())));
	}
	__constexpr
		matcher_t
		operator||(
			const matcher_t& _a_matcher
			) const noexcept
	{
		return matcher_t(matcher_internal_ptr_t(
			new logic_matcher_t<OR>(this->internal_matcher(),
				_a_matcher.internal_matcher())));
	}
	__constexpr
		virtual
		void
		gather_map_source(
			matcher_source_map_t& _a_matcher_source_map
		) const noexcept override final;
private:
	matcher_internal_ptr_t _m_matcher_internal;
	__constexpr
		virtual
		matcher_result_t
		run(
			test_runner_t&
		) override;
	template<
		logic_enum_t Logic_Enum
	>
	__constexpr
		bool
		process_(
			const matcher_t& _a_matcher
		) noexcept;
};
_END_ABC_NS

_BEGIN_ABC_NS
__constexpr_imp
	matcher_result_t
	matcher_t::run(
		test_runner_t& _a_test_runner
	)
{
	using namespace std;
	using namespace errors;
	if (_m_matcher_internal.get() != nullptr)
	{
		return _m_matcher_internal->run_test(_a_test_runner);
	//	return _m_matcher_internal->test_result();
	}
	else
	{
		throw test_library_exception_t(
			"_m_matcher_internal contains a nullptr, meaning that it contains no internal matcher. "
			"Therefore, the test cannot be run as it is invalid."
		);
	}
}
__constexpr_imp
	matcher_t::matcher_t(
	) noexcept
	: generic_matcher_t()
	, _m_matcher_internal(matcher_internal_ptr_t(nullptr))
{}
/*__constexpr_imp
	matcher_t::matcher_t(
		matcher_internal_ptr_t _a_matcher_internal
	) noexcept
	: generic_matcher_t()
	, _m_matcher_internal(_a_matcher_internal)
{

}*/
__constexpr_imp
	matcher_t::matcher_t(
		matcher_internal_ptr_t _a_matcher_internal,
		const std::optional<std::source_location>& _a_source_location
	) noexcept
	: generic_matcher_t()
	, _m_matcher_internal(_a_matcher_internal)
{
	if (_a_source_location.has_value())
	{
		_m_matcher_internal->add_source_info(_a_source_location.value());
	}
}
__constexpr_imp
	matcher_t::matcher_t(
		const matcher_t& _a_matcher_internal,
		const std::string_view _a_str_representation,
		const std::source_location& _a_source_location
	) noexcept
	: generic_matcher_t()
	, _m_matcher_internal(_a_matcher_internal.internal_matcher())
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
		const std::source_location& _a_source_location
	) noexcept
{
	*this = matcher_t(matcher_internal_ptr_t(
		new logic_matcher_t<OR>(this->internal_matcher(),
			std::shared_ptr<generic_matcher_t>())), "or_statement()",
		_a_source_location);
	return this->internal_matcher().get();
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
		throw errors::test_library_exception_t(
			fmt::format("Could not run process; the parent node we are trying to add an OR or AND "
				"node to is not a logic_matcher_t. Please check your code, or contact the developer."));
	}
}
__constexpr_imp
	void
	matcher_t::gather_map_source(
		matcher_source_map_t& _a_matcher_source_map
	) const noexcept 
{
	generic_matcher_t::gather_map_source(_a_matcher_source_map);
	this->internal_matcher()->gather_map_source(_a_matcher_source_map);
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
_END_ABC_NS