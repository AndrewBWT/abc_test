#pragma once
#include "abc_test/utility/internal/macros.h"
#include "abc_test/core/ds/source/single_source.h"
#include <optional>
#include "abc_test/matchers/matcher.h"
#include "abc_test/matchers/annotation.h"

_BEGIN_ABC_NS
template<
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
			const std::string_view _a_test_annotation,
			const ds::single_source_t& _a_source
		) noexcept;
	__constexpr
		~test_block_t(
		) noexcept;
	__constexpr_imp
		test_block_t&
		operator=(
			const matcher_t& _a_element
			) noexcept;
	__constexpr_imp
		test_block_t&
		operator=(
			const matcher_with_annotation_t& _a_element
			) noexcept;
	__constexpr
		const matcher_t&
		matcher(
		) const noexcept;
	__constexpr
		const std::optional<std::string>&
		test_annotation(
		) const noexcept;
	__constexpr
		const std::optional<std::string>&
		matcher_annotation(
		) const noexcept;
	__constexpr
		void
		register_end(
			const ds::single_source_t& _a_end_source
		) noexcept;
	__constexpr
		const ds::source_pair_t&
		source(
		) const noexcept;
	__constexpr
		void
		set_processed(
		) noexcept;
private:
	ds::source_pair_t _m_source;
	std::optional<std::string> _m_test_annotation;
	std::optional<std::string> _m_matcher_annotation;
	matcher_t _m_matcher;
	bool _m_processed;
};
_END_ABC_NS

_BEGIN_ABC_NS
template<
	typename Assertion_Type
>
__constexpr_imp
test_block_t<Assertion_Type>::test_block_t(
	const std::string_view _a_test_annotation,
	const ds::single_source_t& _a_source
) noexcept
	: _m_source(ds::source_pair_t(_a_source))
	, _m_test_annotation(_a_test_annotation)
	, _m_processed(false)
{

}
template<
	typename Assertion_Type
>
__constexpr_imp
test_block_t<Assertion_Type>::~test_block_t(
) noexcept
{
	if (not _m_processed)
	{
		global::get_this_threads_test_runner_ref().add_warning(
			fmt::format("test_block_t is being deleted, however it has not been "
				"processed for its true or false status. "
			));
	}
}
template<
	typename Assertion_Type
>
__constexpr_imp
test_block_t<Assertion_Type>&
test_block_t<Assertion_Type>::operator=(
	const matcher_t& _a_element
	) noexcept
{
	this->_m_matcher = _a_element;
	this->_m_matcher_annotation = std::optional<std::string>();
	return *this;
}
template<
	typename Assertion_Type
>
__constexpr_imp
test_block_t<Assertion_Type>&
test_block_t<Assertion_Type>::operator=(
	const matcher_with_annotation_t& _a_element
	) noexcept
{
	this->_m_matcher = _a_element.matcher();
	this->_m_matcher_annotation = _a_element.annotation();
	return *this;
}
template<
	typename Assertion_Type
>
__constexpr_imp
const matcher_t&
test_block_t<Assertion_Type>::matcher(
) const noexcept
{
	return _m_matcher;
}
template<
	typename Assertion_Type
>
__constexpr_imp
const std::optional<std::string>&
test_block_t<Assertion_Type>::test_annotation(
) const noexcept
{
	return _m_test_annotation;
}
template<
	typename Assertion_Type
>
__constexpr_imp
const std::optional<std::string>&
test_block_t<Assertion_Type>::matcher_annotation(
) const noexcept
{
	return _m_matcher_annotation;
}
template<
	typename Assertion_Type
>
__constexpr_imp
void
test_block_t<Assertion_Type>::register_end(
	const ds::single_source_t& _a_end_source
) noexcept
{
	_m_source.set_end_source(_a_end_source);
}
template<
	typename Assertion_Type
>
__constexpr_imp
const ds::source_pair_t&
test_block_t<Assertion_Type>::source(
) const noexcept
{
	using namespace reports;
	return _m_source;
}
template<
	typename Assertion_Type
>
__constexpr_imp
void
test_block_t<Assertion_Type>::set_processed(
) noexcept
{
	_m_processed = true;
}
_END_ABC_NS
