#pragma once

#include "abc_test/core/test_reports/unexpected_report.hpp"

_BEGIN_ABC_REPORTS_NS

/*!
 * @brief Concrete class used to describe text-based warnings. These are
 * unexpected reports which do not require the test to terminate, however they
 * are worth drawing the attention of the user to.
 */
class basic_text_warning_t : public unexpected_report_t<false>
{
public:
    /*!
     * @brief Constructor.
     * @param _a_source The source of the warning
     * @param _a_exact_source Bool denoting whether its an exact source or not.
     * @param _a_str The message.
     */
    __constexpr
    basic_text_warning_t(
        const std::optional<_ABC_NS_DS::single_source_t>& _a_source,
        const bool                 _a_exact_source,
        const std::string_view     _a_str
    ) noexcept;
    __constexpr virtual std::unique_ptr<unexpected_report_t>
        deep_copy() const noexcept override
    {
        return make_unique<basic_text_warning_t>(
            source(),
            exact_source(),
            _m_str
        );
    }
private:
    std::string _m_str;
};

_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS
__constexpr_imp
    basic_text_warning_t::basic_text_warning_t(
        const std::optional<_ABC_NS_DS::single_source_t>& _a_source,
        const bool                 _a_exact_source,
        const std::string_view     _a_str
    ) noexcept
    : unexpected_report_t<false>(_a_source, _a_exact_source), _m_str(_a_str)
{}

_END_ABC_REPORTS_NS