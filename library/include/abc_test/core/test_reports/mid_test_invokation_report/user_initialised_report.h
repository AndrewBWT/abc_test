#pragma once

#include "abc_test/utility/internal/macros.h"

#include <type_traits>
//#include "abc_test/core/log_test_msg.h"
#include "abc_test/core/ds/source/source_pair.h"

_BEGIN_ABC_REPORTS_NS
using log_infos_t = std::vector<std::string>;
/*class generic_user_report_t
{
public:
	__constexpr_imp
		virtual
		~generic_user_report_t(
		) noexcept
	{

	}
};
using generic_user_report_ptr_t = std::shared_ptr<const generic_user_report_t>;
using generic_user_report_collection_t = std::vector<generic_user_report_ptr_t>;*/
template<
	bool Single_Source
>
class user_initialised_report_t
{
public:
	using source_t = std::conditional_t<Single_Source, ds::single_source_t, ds::source_pair_t>;
	__constexpr
		const source_t&
		source(
		) const noexcept;
	__constexpr
		const log_infos_t&
		log_infos(
		) const noexcept;
	__constexpr
		const ds::single_source_t&
		last_source(
		) const noexcept;
protected:
	__constexpr
		user_initialised_report_t(
		) noexcept = delete;
	__constexpr
		user_initialised_report_t(
			const source_t& _a_source,
			const log_infos_t& _a_log_infos
		) noexcept;
	__constexpr
		virtual
		~user_initialised_report_t(
		) noexcept;
	source_t _m_source;
	log_infos_t _m_log_infos;
};
_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS

template<
	bool Single_Source
>
__constexpr_imp
const user_initialised_report_t<Single_Source>::source_t&
user_initialised_report_t<Single_Source>::source(
) const noexcept
{
	return _m_source;
}
template<
	bool Single_Source
>
__constexpr_imp
const log_infos_t&
user_initialised_report_t<Single_Source>::log_infos(
) const noexcept
{
	return _m_log_infos;
}
template<
	bool Single_Source
>
__constexpr_imp
const ds::single_source_t&
user_initialised_report_t<Single_Source>::last_source(
) const noexcept
{
	if constexpr (Single_Source)
	{
		return _m_source;
	}
	else
	{
		return _m_source.last_source();
	}
}
template<
	bool Single_Source
>
__constexpr_imp
user_initialised_report_t<Single_Source>::user_initialised_report_t(
	const source_t& _a_source,
	const log_infos_t& _a_log_infos
) noexcept
	: _m_source(_a_source)
	, _m_log_infos(_a_log_infos)
{

}
template<
	bool Single_Source
>
__constexpr_imp
user_initialised_report_t<Single_Source>::~user_initialised_report_t(
) noexcept
{

}
_END_ABC_REPORTS_NS