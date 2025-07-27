#pragma once

#include "abc_test/core/reporters/test_reporter.hpp"

_BEGIN_ABC_REPORTERS_NS
struct memoized_test_reporter_t : public test_reporter_t
{
private:
    mutable std::optional<ds::invoked_test_data_t> _m_itds;
    mutable std::optional<ds::pre_test_run_report_t> _m_prrr;
    mutable std::optional<ds::finalised_test_set_data_t> _m_test_set_data;
public:
    __constexpr virtual void
        report_test(const ds::invoked_test_data_t& _a_itd) const override
    {
     //   _m_itds = _a_itd;
    }
    __no_constexpr_imp virtual void
        final_report(ds::finalised_test_set_data_t& _a_test_set_data
        ) const noexcept override
    {
        _m_test_set_data = _a_test_set_data;
    }
    __no_constexpr_imp virtual void
        pre_test_run_report(ds::pre_test_run_report_t& _a_pre_test_run_report
        ) const noexcept override
    {
        _m_prrr = _a_pre_test_run_report;
    }
};
_END_ABC_REPORTERS_NS