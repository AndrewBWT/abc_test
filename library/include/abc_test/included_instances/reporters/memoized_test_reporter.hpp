#pragma once

#include "abc_test/core/reporters/test_reporter.hpp"

_BEGIN_ABC_REPORTERS_NS

struct memoized_test_result_t
{
    memoized_test_result_t(
        const ds::invoked_test_data_t& _a_test_data
    )
    {
        tests_path              = _a_test_data.path();
        auto post_data          = _a_test_data.post_setup_test_data();
        auto registered_data    = post_data.registered_test_data();
        user_defined_test_data  = registered_data._m_user_data;
        source_location         = registered_data._m_source;
        test_path_hierarchy     = post_data.test_path_hierarchy();
        is_test_to_be_ran       = post_data.is_test_to_be_ran();
        discovery_id            = post_data.discovery_id();
        has_for_loop_stack_trie = post_data.has_for_loop_stack_trie();
        if (has_for_loop_stack_trie)
        {
            tdg_collection_stack_trie = post_data.for_loop_stack_trie();
        }
        thread_resourses_required = post_data.thread_resourses_required();
        unique_id                 = std::u8string(post_data.unique_id());
        for_loop_stack_trie       = _a_test_data.for_loop_stack_trie();
        order_ran_id              = _a_test_data.order_ran_id();
        assertions_passed         = _a_test_data.assertions_passed();
        assertions_failed         = _a_test_data.assertions_failed();
        assertions_recieved       = _a_test_data.assertions_recieved();
        test_status               = _a_test_data.test_status();
        warnings_recieved         = _a_test_data.warnings_recieved();
        if (_a_test_data.unexpected_termination() != nullptr)
        {
            unexpected_termination
                = _a_test_data.unexpected_termination()->deep_copy();
        }
        assertions.reserve(_a_test_data.assertions().size());
        for (const auto& _l_element : _a_test_data.assertions())
        {
            assertions.push_back(_l_element->deep_copy());
        }
        time_begin = _a_test_data.time_begin();
        time_end   = _a_test_data.time_end();
    }

    memoized_test_result_t(
        const memoized_test_result_t& other
    )
    {
        tests_path                = other.tests_path;
        user_defined_test_data    = other.user_defined_test_data;
        source_location           = other.source_location;
        test_path_hierarchy       = other.test_path_hierarchy;
        is_test_to_be_ran         = other.is_test_to_be_ran;
        discovery_id              = other.discovery_id;
        has_for_loop_stack_trie   = other.has_for_loop_stack_trie;
        tdg_collection_stack_trie = other.tdg_collection_stack_trie;
        thread_resourses_required = other.thread_resourses_required;
        unique_id                 = other.unique_id;
        for_loop_stack_trie       = other.for_loop_stack_trie;
        order_ran_id              = other.order_ran_id;
        assertions_passed         = other.assertions_passed;
        assertions_failed         = other.assertions_failed;
        assertions_recieved       = other.assertions_recieved;
        test_status               = other.test_status;
        warnings_recieved         = other.warnings_recieved;
        unexpected_termination
            = (other.unexpected_termination == nullptr)
                  ? nullptr
                  : other.unexpected_termination->deep_copy();
        assertions.reserve(other.assertions.size());
        for (const auto& _l_element : other.assertions)
        {
            assertions.push_back(_l_element->deep_copy());
        }
        time_begin = other.time_begin;
        time_end   = other.time_begin;
    }

    memoized_test_result_t&
        operator=(
            const memoized_test_result_t& other
        )
    {
        tests_path                = other.tests_path;
        user_defined_test_data    = other.user_defined_test_data;
        source_location           = other.source_location;
        test_path_hierarchy       = other.test_path_hierarchy;
        is_test_to_be_ran         = other.is_test_to_be_ran;
        discovery_id              = other.discovery_id;
        has_for_loop_stack_trie   = other.has_for_loop_stack_trie;
        tdg_collection_stack_trie = other.tdg_collection_stack_trie;
        thread_resourses_required = other.thread_resourses_required;
        unique_id                 = other.unique_id;
        for_loop_stack_trie       = other.for_loop_stack_trie;
        order_ran_id              = other.order_ran_id;
        assertions_passed         = other.assertions_passed;
        assertions_failed         = other.assertions_failed;
        assertions_recieved       = other.assertions_recieved;
        test_status               = other.test_status;
        warnings_recieved         = other.warnings_recieved;
        unexpected_termination
            = (other.unexpected_termination == nullptr)
                  ? nullptr
                  : other.unexpected_termination->deep_copy();
        assertions.reserve(other.assertions.size());
        for (const auto& _l_element : other.assertions)
        {
            assertions.push_back(_l_element->deep_copy());
        }
        time_begin = other.time_begin;
        time_end   = other.time_begin;
        return *this;
    }

    memoized_test_result_t(memoized_test_result_t&&) noexcept = default;
    memoized_test_result_t&
        operator=(memoized_test_result_t&&) noexcept
        = default;
    std::filesystem::path              tests_path;
    abc::ds::user_defined_test_data_t  user_defined_test_data;
    std::optional<ds::single_source_t> source_location;
    abc::ds::test_path_hierarchy_t     test_path_hierarchy;
    bool                               is_test_to_be_ran;
    abc::ds::unique_id_t               discovery_id;
    bool                               has_for_loop_stack_trie;
    std::optional<abc::ds::tdg_collection_stack_trie_t>
                                              tdg_collection_stack_trie;
    size_t                                    thread_resourses_required;
    std::u8string                             unique_id;
    abc::ds::tdg_collection_stack_trie_t      for_loop_stack_trie;
    std::size_t                               order_ran_id;
    std::size_t                               assertions_passed;
    std::size_t                               assertions_failed;
    std::size_t                               assertions_recieved;
    abc::ds::enum_test_status_t               test_status;
    std::size_t                               warnings_recieved;
    abc::reports::opt_unexpected_report_t     unexpected_termination;
    abc::reports::assertion_base_collection_t assertions;
    std::chrono::time_point<std::chrono::high_resolution_clock> time_begin;
    std::chrono::time_point<std::chrono::high_resolution_clock> time_end;
};

struct memoized_test_reporter_t : public test_reporter_t
{
public:
    mutable std::optional<ds::pre_test_run_report_t>     _m_prrr;
    mutable std::optional<ds::finalised_test_set_data_t> _m_test_set_data;
    mutable std::vector<memoized_test_result_t>          _m_tests;
public:
    __constexpr virtual void
        report_test(
            const ds::invoked_test_data_t& _a_itd
        ) const override
    {
        _m_tests.push_back(memoized_test_result_t(_a_itd));
        // _m_itds = _a_itd;
    }

    __no_constexpr_imp virtual void
        final_report(
            ds::finalised_test_set_data_t& _a_test_set_data
        ) const noexcept override
    {
        _m_test_set_data = _a_test_set_data;
    }

    __no_constexpr_imp virtual void
        pre_test_run_report(
            ds::pre_test_run_report_t& _a_pre_test_run_report
        ) const noexcept override
    {
        _m_prrr = _a_pre_test_run_report;
    }
};

_END_ABC_REPORTERS_NS