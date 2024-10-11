#pragma once
#include "abc_test/matchers/matcher.h"

_BEGIN_ABC_NS
class matcher_with_annotation_t
{
public:
	__no_constexpr_imp
		matcher_with_annotation_t(
			const matcher_t& _a_matcher,
			const std::string_view _a_annotation
		) noexcept
		: _m_matcher(_a_matcher)
		, _m_annotation(_a_annotation)
		, _m_initialised_with_source(false)
	{

	}
	__no_constexpr_imp
		matcher_with_annotation_t(
			const matcher_with_annotation_t& _a_matcher,
			const ds::single_source_t& _a_single_source
		)
		: _m_matcher(_a_matcher.matcher())
		, _m_initialised_with_source(true)
		, _m_annotation(_a_matcher.annotation())
	{
		if (not _a_matcher._m_initialised_with_source)
		{
			_a_matcher.matcher().internal_matcher()->add_source_info(_a_single_source);
		}
		else
		{
			std::cout << "Issued warning" << std::endl;
		}
	}
	__constexpr_imp
		const matcher_t&
		matcher(
		) const noexcept
	{
		return _m_matcher;
	}
	__constexpr_imp
		const std::string_view
		annotation(
		) const noexcept
	{
		return _m_annotation;
	}
private:
	std::string _m_annotation;
	matcher_t _m_matcher;
	bool _m_initialised_with_source;
};
__no_constexpr_imp
matcher_with_annotation_t
annotate(
	const matcher_t& _a_matcher,
	const std::string_view _a_annotation
) noexcept
{
	return matcher_with_annotation_t(_a_matcher, _a_annotation);
}
__no_constexpr_imp
matcher_with_annotation_t
annotate(
	const std::string_view _a_annotation,
	const matcher_t& _a_matcher
) noexcept
{
	return matcher_with_annotation_t(_a_matcher, _a_annotation);
}
_END_ABC_NS

_BEGIN_ABC_NS
_END_ABC_NS