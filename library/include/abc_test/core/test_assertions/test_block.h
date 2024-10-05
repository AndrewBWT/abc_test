#pragma once
#include "abc_test/utility/internal/macros.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/single_source.h"
#include <optional>

_BEGIN_ABC_NS
template<
	typename T,
	typename Assertion_Type
>
class test_block_t
{
public:
	__constexpr
		test_block_t(
		) noexcept = delete;
	__constexpr
		test_block_t(
			const reports::single_source_t& _a_source
		) noexcept;
	__constexpr
		~test_block_t(
		) noexcept;
	__constexpr_imp
		test_block_t&
		operator=(
			const T& _a_element
			) noexcept;
	__constexpr_imp
		void
		set_message(
			const std::string_view _a_message
		) noexcept;
	__constexpr
		const T&
		inner_value(
		) const noexcept;
	__constexpr
		const std::optional<std::string>&
		message(
		) const noexcept;
	__constexpr
		void
		register_end(
			const reports::single_source_t& _a_end_source
		) noexcept;
	__constexpr
		const reports::source_pair_t&
		source(
		) const noexcept;
private:
	reports::source_pair_t _m_source;
	std::optional<std::string> _m_message;
	T _m_inner_element;
	bool _m_processed;
};
_END_ABC_NS

_BEGIN_ABC_NS
template<
	typename T,
	typename Assertion_Type
>
__constexpr_imp
test_block_t<T, Assertion_Type>::test_block_t(
	const reports::single_source_t& _a_source
) noexcept
	: _m_source(reports::source_pair_t(_a_source))
{

}
template<
	typename T,
	typename Assertion_Type
>
__constexpr_imp
test_block_t<T,Assertion_Type>::~test_block_t(
) noexcept
{
	if (not _m_processed)
	{
		std::cout << "unprocessed" << std::endl;
	}
}
template<
	typename T,
	typename Assertion_Type
>
__constexpr_imp
test_block_t<T,Assertion_Type>&
test_block_t<T, Assertion_Type>::operator=(
	const T& _a_element
	) noexcept
{
	this->_m_inner_element = _a_element;
	return *this;
}
template<
	typename T,
	typename Assertion_Type
>
__constexpr_imp
void
test_block_t<T, Assertion_Type>::set_message(
	const std::string_view _a_message
) noexcept
{
	_m_message = _a_message;
}
template<
	typename T,
	typename Assertion_Type
>
__constexpr_imp
const T&
test_block_t<T, Assertion_Type>::inner_value(
) const noexcept
{
	return _m_inner_element;
}
template<
	typename T,
	typename Assertion_Type
>
__constexpr_imp
const std::optional<std::string>&
test_block_t<T, Assertion_Type>::message(
) const noexcept
{
	return _m_message;
}
template<
	typename T,
	typename Assertion_Type
>
__constexpr_imp
void
test_block_t<T, Assertion_Type>::register_end(
	const reports::single_source_t& _a_end_source
) noexcept
{
	_m_source = reports::source_pair_t(_m_source.begin_source(),_a_end_source);
}
template<
	typename T,
	typename Assertion_Type
>
__constexpr_imp
const reports::source_pair_t&
test_block_t<T, Assertion_Type>::source(
) const noexcept
{
	using namespace reports;
	return _m_source;
}
_END_ABC_NS
