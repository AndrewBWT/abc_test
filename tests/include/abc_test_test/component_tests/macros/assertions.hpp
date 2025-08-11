#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/included_instances/reporters/memoized_error_reporter.hpp"
#include "abc_test/included_instances/reporters/memoized_test_reporter.hpp"
#include "abc_test/utility/str/unicode.hpp"
#include "abc_test_test/testing_utilities/bootstrapped_testing_utilities.hpp"

_TEST_CASE(
    abc::test_case_t(
        {.name = "Check _CHECK macro works correctly",
         .path = "abc_test_test::component_tests::macros::assertions",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    using namespace abc::utility::io;
    using namespace _ABC_NS_UTILITY_STR;
    using namespace _ABC_NS_DS;
    function<void()> _l_test_func_1 = [&]()
    {
        _CHECK(true_matcher());
        _CHECK(false_matcher());
    };
    std::vector<std::tuple<
        std::function<void()>,
        std::string,
        std::string,
        abc::ds::tdg_collection_stack_trie_t>>
                                _l_funcs_to_run;
    tdg_collection_stack_trie_t _l_stack;
    string                      _l_name_1{"name"};
    string                      _l_path{"path"};
    _l_funcs_to_run.push_back(
        make_tuple(_l_test_func_1, _l_name_1, _l_path, _l_stack)
    );
    auto _l_rv = abc_test::utility::abc_test_tests_internal_test_runner(
        1, true, _l_funcs_to_run
    );
    auto& _l_assertion = _l_rv.memoized_test_reporter._m_tests;
    _CHECK_EXPR(_l_assertion.size() == 1);
    auto& _l_only_assertion = _l_assertion.at(0);
    _CHECK_EXPR(_l_only_assertion.assertions_failed == 1);
    _CHECK_EXPR(_l_only_assertion.assertions_passed == 1);
    _CHECK_EXPR(_l_only_assertion.assertions_recieved == 2);
    auto& assertions{_l_only_assertion.assertions};
    _CHECK_EXPR(assertions.size() == 2);
    auto& assertion_true{assertions.at(0)};
    auto& assertion_false{assertions.at(1)};
    const reports::matcher_based_assertion_single_line_t<
        abc::reports::pass_or_fail_t>& _l_element{
        dynamic_cast<const reports::matcher_based_assertion_single_line_t<
            abc::reports::pass_or_fail_t>&>(*assertion_true.get())
    };
    _CHECK_EXPR(_l_element.get_pass_status() == true);
    _CHECK_EXPR(_l_element.terminated() == false);
    const reports::matcher_based_assertion_single_line_t<
        abc::reports::pass_or_fail_t>& _l_element2{
        dynamic_cast<const reports::matcher_based_assertion_single_line_t<
            abc::reports::pass_or_fail_t>&>(*assertion_false.get())
    };
    _CHECK_EXPR(_l_element2.get_pass_status() == false);
    _CHECK_EXPR(_l_element2.terminated() == false);
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Check _CHECK_EXPR macro works correctly",
         .path = "abc_test_test::component_tests::macros::assertions",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    using namespace abc::utility::io;
    using namespace _ABC_NS_UTILITY_STR;
    using namespace _ABC_NS_DS;
    function<void()> _l_test_func_1 = [&]()
    {
        _CHECK_EXPR(2 == 2);
        _CHECK_EXPR(2 == 3);
    };
    std::vector<std::tuple<
        std::function<void()>,
        std::string,
        std::string,
        abc::ds::tdg_collection_stack_trie_t>>
                                _l_funcs_to_run;
    tdg_collection_stack_trie_t _l_stack;
    string                      _l_name_1{"name"};
    string                      _l_path{"path"};
    _l_funcs_to_run.push_back(
        make_tuple(_l_test_func_1, _l_name_1, _l_path, _l_stack)
    );
    auto _l_rv = abc_test::utility::abc_test_tests_internal_test_runner(
        1, true, _l_funcs_to_run
    );
    auto& _l_assertion = _l_rv.memoized_test_reporter._m_tests;
    _CHECK_EXPR(_l_assertion.size() == 1);
    auto& _l_only_assertion = _l_assertion.at(0);
    _CHECK_EXPR(_l_only_assertion.assertions_failed == 1);
    _CHECK_EXPR(_l_only_assertion.assertions_passed == 1);
    _CHECK_EXPR(_l_only_assertion.assertions_recieved == 2);
    auto& assertions{_l_only_assertion.assertions};
    _CHECK_EXPR(assertions.size() == 2);
    auto& assertion_true{assertions.at(0)};
    auto& assertion_false{assertions.at(1)};
    const reports::matcher_based_assertion_single_line_t<
        abc::reports::pass_or_fail_t>& _l_element{
        dynamic_cast<const reports::matcher_based_assertion_single_line_t<
            abc::reports::pass_or_fail_t>&>(*assertion_true.get())
    };
    _CHECK_EXPR(_l_element.get_pass_status() == true);
    _CHECK_EXPR(_l_element.terminated() == false);
    const reports::matcher_based_assertion_single_line_t<
        abc::reports::pass_or_fail_t>& _l_element2{
        dynamic_cast<const reports::matcher_based_assertion_single_line_t<
            abc::reports::pass_or_fail_t>&>(*assertion_false.get())
    };
    _CHECK_EXPR(_l_element2.get_pass_status() == false);
    _CHECK_EXPR(_l_element2.terminated() == false);
    auto& matcher1{ _l_element.matcher_info() };
    _CHECK_EXPR(matcher1.annotation().has_value() == false);
    //_CHECK_EXPR(matcher1.assertion_type() == abc::matcher::enum_bba_inner_assertion_type_t::MATCHER_BASED_ASSERTION);
    _CHECK_EXPR(matcher1.terminate() == false);
    auto& matcher1_res{ matcher1.matcher_result() };
    _CHECK_EXPR(matcher1_res.passed() == true);
    auto& matcher1_str{ matcher1_res.str() };
    _CHECK_EXPR(matcher1_str.get_vector().size() == 0);
    _CHECK_EXPR(matcher1_str.get_tree().size() == 0);
    _CHECK_EXPR(matcher1_str.primary_data() == u8"2 == 2");

    auto& matcher2{ _l_element2.matcher_info() };
    _CHECK_EXPR(matcher2.annotation().has_value() == false);
   // _CHECK_EXPR(matcher2.assertion_type() == abc::matcher::enum_bba_inner_assertion_type_t::MATCHER_BASED_ASSERTION);
    _CHECK_EXPR(matcher2.terminate() == false);
    auto& matcher2_res{ matcher2.matcher_result() };
    _CHECK_EXPR(matcher2_res.passed() == false);
    auto& matcher2_str{ matcher2_res.str() };
    _CHECK_EXPR(matcher2_str.get_vector().size() == 0);
    _CHECK_EXPR(matcher2_str.get_tree().size() == 0);
    _CHECK_EXPR(matcher2_str.primary_data() == u8"2 != 3");
}

_TEST_CASE(
    abc::test_case_t(
        { .name = "Check _REQUIRE macro works correctly",
         .path = "abc_test_test::component_tests::macros::assertions",
         .threads_required = 1 }
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    using namespace abc::utility::io;
    using namespace _ABC_NS_UTILITY_STR;
    using namespace _ABC_NS_DS;
    function<void()> _l_test_func_1 = [&]()
        {
            _REQUIRE(true_matcher());
            _REQUIRE(false_matcher());
            _REQUIRE(true_matcher());
        };
    std::vector<std::tuple<
        std::function<void()>,
        std::string,
        std::string,
        abc::ds::tdg_collection_stack_trie_t>>
        _l_funcs_to_run;
    tdg_collection_stack_trie_t _l_stack;
    string                      _l_name_1{ "name" };
    string                      _l_path{ "path" };
    _l_funcs_to_run.push_back(
        make_tuple(_l_test_func_1, _l_name_1, _l_path, _l_stack)
    );
    auto _l_rv = abc_test::utility::abc_test_tests_internal_test_runner(
        1, true, _l_funcs_to_run
    );
    auto& _l_assertion = _l_rv.memoized_test_reporter._m_tests;
    _CHECK_EXPR(_l_assertion.size() == 1);
    auto& _l_only_assertion = _l_assertion.at(0);
    _CHECK_EXPR(_l_only_assertion.assertions_failed == 1);
    _CHECK_EXPR(_l_only_assertion.assertions_passed == 1);
    _CHECK_EXPR(_l_only_assertion.assertions_recieved == 2);
    auto& assertions{ _l_only_assertion.assertions };
    _CHECK_EXPR(assertions.size() == 2);
    auto& assertion_true{ assertions.at(0) };
    auto& assertion_false{ assertions.at(1) };
    const reports::matcher_based_assertion_single_line_t<
        abc::reports::pass_or_terminate_t>& _l_element{
        dynamic_cast<const reports::matcher_based_assertion_single_line_t<
            abc::reports::pass_or_terminate_t>&>(*assertion_true.get())
    };
    _CHECK_EXPR(_l_element.get_pass_status() == true);
    _CHECK_EXPR(_l_element.terminated() == false);
    const reports::matcher_based_assertion_single_line_t<
        abc::reports::pass_or_terminate_t>& _l_element2{
        dynamic_cast<const reports::matcher_based_assertion_single_line_t<
            abc::reports::pass_or_terminate_t>&>(*assertion_false.get())
    };
    _CHECK_EXPR(_l_element2.get_pass_status() == false);
    _CHECK_EXPR(_l_element2.terminated() == true);
}

_TEST_CASE(
    abc::test_case_t(
        { .name = "Check _REQUIRE_EXPR macro works correctly",
         .path = "abc_test_test::component_tests::macros::assertions",
         .threads_required = 1 }
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    using namespace abc::utility::io;
    using namespace _ABC_NS_UTILITY_STR;
    using namespace _ABC_NS_DS;
    function<void()> _l_test_func_1 = [&]()
        {
            _REQUIRE_EXPR(2 == 2);
            _REQUIRE_EXPR(2 == 3);
            _REQUIRE_EXPR(2 == 2);
        };
    std::vector<std::tuple<
        std::function<void()>,
        std::string,
        std::string,
        abc::ds::tdg_collection_stack_trie_t>>
        _l_funcs_to_run;
    tdg_collection_stack_trie_t _l_stack;
    string                      _l_name_1{ "name" };
    string                      _l_path{ "path" };
    _l_funcs_to_run.push_back(
        make_tuple(_l_test_func_1, _l_name_1, _l_path, _l_stack)
    );
    auto _l_rv = abc_test::utility::abc_test_tests_internal_test_runner(
        1, true, _l_funcs_to_run
    );
    auto& _l_assertion = _l_rv.memoized_test_reporter._m_tests;
    _CHECK_EXPR(_l_assertion.size() == 1);
    auto& _l_only_assertion = _l_assertion.at(0);
    _CHECK_EXPR(_l_only_assertion.assertions_failed == 1);
    _CHECK_EXPR(_l_only_assertion.assertions_passed == 1);
    _CHECK_EXPR(_l_only_assertion.assertions_recieved == 2);
    auto& assertions{ _l_only_assertion.assertions };
    _CHECK_EXPR(assertions.size() == 2);
    auto& assertion_true{ assertions.at(0) };
    auto& assertion_false{ assertions.at(1) };
    const reports::matcher_based_assertion_single_line_t<
        abc::reports::pass_or_terminate_t>& _l_element{
        dynamic_cast<const reports::matcher_based_assertion_single_line_t<
            abc::reports::pass_or_terminate_t>&>(*assertion_true.get())
    };
    _CHECK_EXPR(_l_element.get_pass_status() == true);
    _CHECK_EXPR(_l_element.terminated() == false);
    const reports::matcher_based_assertion_single_line_t<
        abc::reports::pass_or_terminate_t>& _l_element2{
        dynamic_cast<const reports::matcher_based_assertion_single_line_t<
            abc::reports::pass_or_terminate_t>&>(*assertion_false.get())
    };
    _CHECK_EXPR(_l_element2.get_pass_status() == false);
    _CHECK_EXPR(_l_element2.terminated() == true);
    auto& matcher1{ _l_element.matcher_info() };
    _CHECK_EXPR(matcher1.annotation().has_value() == false);
    //_CHECK_EXPR(matcher1.assertion_type() == abc::matcher::enum_bba_inner_assertion_type_t::MATCHER_BASED_ASSERTION);
    _CHECK_EXPR(matcher1.terminate() == true);
    auto& matcher1_res{ matcher1.matcher_result() };
    _CHECK_EXPR(matcher1_res.passed() == true);
    auto& matcher1_str{ matcher1_res.str() };
    _CHECK_EXPR(matcher1_str.get_vector().size() == 0);
    _CHECK_EXPR(matcher1_str.get_tree().size() == 0);
    _CHECK_EXPR(matcher1_str.primary_data() == u8"2 == 2");

    auto& matcher2{ _l_element2.matcher_info() };
    _CHECK_EXPR(matcher2.annotation().has_value() == false);
    // _CHECK_EXPR(matcher2.assertion_type() == abc::matcher::enum_bba_inner_assertion_type_t::MATCHER_BASED_ASSERTION);
    _CHECK_EXPR(matcher2.terminate() == true);
    auto& matcher2_res{ matcher2.matcher_result() };
    _CHECK_EXPR(matcher2_res.passed() == false);
    auto& matcher2_str{ matcher2_res.str() };
    _CHECK_EXPR(matcher2_str.get_vector().size() == 0);
    _CHECK_EXPR(matcher2_str.get_tree().size() == 0);
    _CHECK_EXPR(matcher2_str.primary_data() == u8"2 != 3");
}