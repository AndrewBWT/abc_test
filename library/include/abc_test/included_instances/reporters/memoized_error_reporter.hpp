#pragma once
#include "abc_test/core/reporters/error_reporter.hpp"

_BEGIN_ABC_REPORTERS_NS

/*!
 * @brief Abstract object which processes errors taken as the test framework is
 * running.
 */
    struct memoized_error_reporter_t : public error_reporter_t
{
private:
    mutable std::vector<_ABC_NS_ERRORS::abc_test_error_t> _m_errors;
    mutable std::vector<std::string> _m_info_reports;
public:
    __constexpr const std::vector<_ABC_NS_ERRORS::abc_test_error_t>& errors() const noexcept
    {
        return _m_errors;
    }
    __constexpr virtual void
        process_error(const _ABC_NS_ERRORS::abc_test_error_t& _a_error
        ) const noexcept
    {
        _m_errors.push_back(_a_error);
    }
    __constexpr virtual void
        report_information(const std::string_view _a_str) const noexcept
        override
    {
        _m_info_reports.push_back(std::string(_a_str));
    }
    __constexpr virtual void
        exit() const noexcept override
    {
        
    }
};
/*!
 * @brief Type synonym for a collection of either shared or raw pointers of type
 * error_reproter_t.
 */
using error_reporters_t
= std::vector<std::reference_wrapper<const error_reporter_t>>;
_END_ABC_REPORTERS_NS