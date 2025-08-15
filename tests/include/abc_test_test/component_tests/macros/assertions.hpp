#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/included_instances/reporters/memoized_error_reporter.hpp"
#include "abc_test/included_instances/reporters/memoized_test_reporter.hpp"
#include "abc_test/utility/str/unicode.hpp"
#include "abc_test_test/testing_utilities/bootstrapped_testing_utilities.hpp"

namespace abc_test_test::utility
{

template <typename T>
inline void
    check_matcher(
        const abc::reports::assertion_base_ptr_t& _a_pstr,
        const abc::reports::status_t<T>&          _a_expected_status,
        const bool                                _a_passed,
        const bool                                _a_terminated,
        const std::size_t                         _a_index,
        const std::tuple<
            bool,
            bool,
            std::u8string,
            std::vector<std::u8string>,
            std::vector<abc::matcher::matcher_result_infos_t::tree_structure_t>,
            std::optional<std::u8string>>& _a_test_data
    )
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    using namespace abc::utility::io;
    using namespace _ABC_NS_UTILITY_STR;
    using namespace _ABC_NS_DS;
    try
    {
        const reports::matcher_based_assertion_single_line_t<T>& _l_element{
            dynamic_cast<
                const reports::matcher_based_assertion_single_line_t<T>&>(
                *_a_pstr.get()
            )
        };
        auto& [_a_matcher_passed, _a_matcher_can_terminate, _a_matcher_primary_str, _a_matcher_additional_info, _a_matcher_children, _a_matcher_annotation]{
            _a_test_data
        };
        _CHECK_EXPR(_l_element.status() == _a_expected_status);
        _CHECK_EXPR(_l_element.get_pass_status() == _a_passed);
        _CHECK_EXPR(_l_element.terminated() == _a_terminated);
        _CHECK_EXPR(_l_element.assertion_index() == _a_index);
        auto& matcher1{_l_element.matcher_info()};
        _CHECK_EXPR(matcher1.annotation() == _a_matcher_annotation);
        //_CHECK_EXPR(matcher1.assertion_type() ==
        // abc::matcher::enum_bba_inner_assertion_type_t::MATCHER_BASED_ASSERTION);
        _CHECK_EXPR(matcher1.terminate() == _a_matcher_can_terminate);
        auto& matcher1_res{matcher1.matcher_result()};
        _CHECK_EXPR(matcher1_res.passed() == _a_matcher_passed);
        auto& matcher1_str{matcher1_res.str()};
        _CHECK_EXPR(matcher1_str.get_vector() == _a_matcher_additional_info);
        _CHECK_EXPR(matcher1_str.primary_data() == _a_matcher_primary_str);
        auto _l_tree_checker
            = [&](auto _l_self,
                  const std::vector<
                      abc::matcher::matcher_result_infos_t::tree_structure_t>&
                      _a_expected,
                  const std::vector<
                      abc::matcher::matcher_result_infos_t::tree_structure_t>&
                      _a_to_check) -> void
        {
            _REQUIRE_EXPR(_a_expected.size() == _a_to_check.size());
            for (size_t _l_idx{0}; _l_idx < _a_expected.size(); ++_l_idx)
            {
                auto& _l_expected{_a_expected[_l_idx]};
                auto& _l_to_check{_a_to_check[_l_idx]};
                _CHECK_EXPR(get<0>(_l_expected) == get<0>(_l_to_check));
                _CHECK_EXPR(get<1>(_l_expected) == get<1>(_l_to_check));
                auto& _l_expected_matcher{*get<2>(_l_expected).get()};
                auto& _l_to_check_matcher{*get<2>(_l_to_check).get()};
                _CHECK_EXPR(
                    _l_expected_matcher.primary_data()
                    == _l_to_check_matcher.primary_data()
                );
                _CHECK_EXPR(
                    _l_expected_matcher.get_vector()
                    == _l_to_check_matcher.get_vector()
                );
                _l_self(
                    _l_self,
                    _l_expected_matcher.get_tree(),
                    _l_to_check_matcher.get_tree()
                );
            }
        };
        _l_tree_checker(
            _l_tree_checker, matcher1_str.get_tree(), _a_matcher_children
        );
    }
    catch (const std::exception& _a_exception)
    {
        _REQUIRE_EXPR(true == false);
    }
}

template <typename T>
inline void
    check_multi_matcher(
        const abc::reports::assertion_base_ptr_t& _a_pstr,
        const abc::reports::status_t<T>&          _a_expected_status,
        const bool                                _a_passed,
        const bool                                _a_terminated,
        const std::size_t                         _a_index,
        const std::string                         _a_multi_matcher_name,
        const std::vector<std::tuple<
            bool,
            bool,
            std::u8string,
            std::vector<std::u8string>,
            std::vector<abc::matcher::matcher_result_infos_t::tree_structure_t>,
            std::optional<std::u8string>>>&       _a_inner_data
    )
{
    /*using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    using namespace abc::utility::io;
    using namespace _ABC_NS_UTILITY_STR;
    using namespace _ABC_NS_DS;

    try
    {
        const reports::multi_element_assertion_block_t<T>& _l_element{
            dynamic_cast<const reports::multi_element_assertion_block_t<T>&>(
                *_a_pstr.get()
            )
        };
        _CHECK_EXPR(_l_element.status() == _a_expected_status);
        _CHECK_EXPR(_l_element.get_pass_status() == _a_passed);
        _CHECK_EXPR(_l_element.terminated() == _a_terminated);
        _CHECK_EXPR(_l_element.assertion_index() == _a_index);
        _CHECK_EXPR(_l_element.test_description() == _a_multi_matcher_name);
        auto& matcher1{_l_element.get_matcher()};
        _CHECK_EXPR(matcher1.annotation() == _a_annotation);
        _CHECK_EXPR(matcher1.terminate() == _a_terminates);
        auto& matcher1_res{matcher1.matcher_result()};
        _CHECK_EXPR(matcher1_res.passed() == _a_passed);
        auto& matcher1_str{matcher1_res.str()};
        auto  _l_tree_checker
            = [&](auto _l_self,
                  const std::vector<
                      abc::matcher::matcher_result_infos_t::tree_structure_t>&
                      _a_expected,
                  const std::vector<
                      abc::matcher::matcher_result_infos_t::tree_structure_t>&
                      _a_to_check) -> void
        {
            _REQUIRE_EXPR(_a_expected.size() == _a_to_check.size());
            for (size_t _l_idx{0}; _l_idx < _a_expected.size(); ++_l_idx)
            {
                auto& _l_expected{_a_expected[_l_idx]};
                auto& _l_to_check{_a_to_check[_l_idx]};
                _CHECK_EXPR(get<0>(_l_expected) == get<0>(_l_to_check));
                _CHECK_EXPR(get<1>(_l_expected) == get<1>(_l_to_check));
                auto& _l_expected_matcher{*get<2>(_l_expected).get()};
                auto& _l_to_check_matcher{*get<2>(_l_to_check).get()};
                _CHECK_EXPR(
                    _l_expected_matcher.primary_data()
                    == _l_to_check_matcher.primary_data()
                );
                _CHECK_EXPR(
                    _l_expected_matcher.get_vector()
                    == _l_to_check_matcher.get_vector()
                );
                _l_self(
                    _l_self,
                    _l_expected_matcher.get_tree(),
                    _l_to_check_matcher.get_tree()
                );
            }
        };
        _CHECK_EXPR(matcher1_str.primary_data() == _a_primary_data);
        _l_tree_checker(
            _l_tree_checker, matcher1_str.get_tree(), _a_child_data
        );
    }
    catch (const std::exception& _a_exception)
    {
        _REQUIRE_EXPR(true == false);
    }*/
}
} // namespace abc_test_test::utility

_BEGIN_ABC_UTILITY_PRINTER_NS

template <>
struct default_printer_t<abc::matcher::matcher_result_infos_t>
    : public printer_base_t<abc::matcher::matcher_result_infos_t>
{
    __no_constexpr_imp std::u8string
                run_printer(
                    const abc::matcher::matcher_result_infos_t& _a_object
                ) const
    {
        using namespace std;
        using namespace _ABC_NS_CLI;
        using namespace _ABC_NS_UTILITY_STR;
        using namespace _ABC_NS_MATCHER;
        default_printer_t<std::vector<std::tuple<
            std::u8string,
            std::u8string,
            std::shared_ptr<matcher_result_infos_t>>>>
             _l_printer;
        auto _l_result_1 = _l_printer.run_printer(_a_object.get_tree());
        auto _l_result_2 = default_printer_t<vector<u8string>>{}.run_printer(
            _a_object.get_vector()
        );
        auto _l_result_3 = _a_object.primary_data();
        return fmt::format(
            u8"({0}, {1}, {2})", _l_result_1, _l_result_2, _l_result_3
        );
    }
};

_END_ABC_UTILITY_PRINTER_NS

_BEGIN_ABC_UTILITY_PARSER_NS

template <>
struct default_parser_t<abc::matcher::matcher_result_infos_t>
    : public parser_base_t<abc::matcher::matcher_result_infos_t>
{
    __constexpr result_t<abc::matcher::matcher_result_infos_t>
                run_parser(
                    parser_input_t& _a_parse_input
                ) const
    {
        using namespace std;
        using namespace _ABC_NS_CLI;
        using namespace _ABC_NS_UTILITY_STR;
        using namespace _ABC_NS_MATCHER;
        /*
            using tree_structure_t = std::tuple<
        std::u8string,
        std::u8string,
        std::shared_ptr<matcher_result_infos_t>>;
    std::vector<tree_structure_t> _m_tree_info;
    std::u8string                 _m_primary_data;
    std::vector<std::u8string>    _m_additional_info;
        */
        _a_parse_input.check_advance_and_throw(U"(");
        default_parser_t<std::vector<std::tuple<
            std::u8string,
            std::u8string,
            std::shared_ptr<matcher_result_infos_t>>>>
             _l_parser;
        auto _l_result = _l_parser.run_parser(_a_parse_input);
        if (_l_result.has_value())
        {
            _a_parse_input.check_advance_and_throw(U",");
            auto _l_result_2
                = default_parser_t<u8string>{}.run_parser(_a_parse_input);
            if (_l_result_2.has_value())
            {
                _a_parse_input.check_advance_and_throw(U",");
                auto _l_result_3
                    = default_parser_t<vector<u8string>>{}.run_parser(
                        _a_parse_input
                    );
                if (_l_result_3.has_value())
                {
                    _a_parse_input.check_advance_and_throw(U")");
                    return matcher_result_infos_t(
                        _l_result_2.value(),
                        _l_result_3.value(),
                        _l_result.value()
                    );
                }
                else
                {
                    return unexpected(_l_result_3.error());
                }
            }
            else
            {
                return unexpected(_l_result_2.error());
            }
        }
        else
        {
            return unexpected(_l_result.error());
        }
    }
};

_END_ABC_UTILITY_PARSER_NS

_TEST_CASE(
    abc::test_case_t(
        {.name = "Checking standaline _CHECK and _REQUIRE macros",
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
    using namespace _ABC_NS_REPORTS;
    using namespace _ABC_NS_MATCHER;
    auto _l_run_func = [&]<typename T>(
                           vector<function<void()>>& _a_funcs,
                           const string_view         _a_unit_test_name
                       )
    {
        vector<tuple<
            function<void()>,
            string,
            string,
            tdg_collection_stack_trie_t>>
            _l_funcs_to_run;
        for (size_t _l_idx{0}; auto& _l_func : _a_funcs)
        {
            _l_funcs_to_run.push_back(make_tuple(
                _l_func,
                fmt::format("name{0}", _l_idx),
                fmt::format("path{0}", _l_idx),
                tdg_collection_stack_trie_t{}
            ));
            ++_l_idx;
        }
        auto _l_rv = abc_test::utility::abc_test_tests_internal_test_runner(
            1, true, _l_funcs_to_run
        );
        auto& _l_tests             = _l_rv.memoized_test_reporter._m_tests;
        using internal_test_data_t = tuple<
            bool,
            bool,
            bool,
            bool,
            size_t,
            u8string,
            vector<u8string>,
            vector<matcher_result_infos_t::tree_structure_t>,
            optional<u8string>>;
        using test_data_t
            = tuple<size_t, size_t, size_t, vector<internal_test_data_t>>;
        for (auto& [_l_test_index, _l_test_info] :
             read_data_from_file<pair<size_t, test_data_t>>(_a_unit_test_name))
        {
            auto& [_l_passed, _l_failed, _l_recieved, _l_internal_test_data]{
                _l_test_info
            };
            auto& _l_test = _l_tests.at(_l_test_index);
            _CHECK_EXPR(_l_test.assertions_passed == _l_passed);
            _CHECK_EXPR(_l_test.assertions_failed == _l_failed);
            _CHECK_EXPR(_l_test.assertions_recieved == _l_recieved);
            auto& _l_assertions{_l_test.assertions};
            _REQUIRE_EXPR(_l_assertions.size() == _l_internal_test_data.size());
            for (size_t _l_idx{0}; _l_idx < _l_assertions.size(); ++_l_idx)
            {
                auto& _l_assertion{_l_assertions[_l_idx]};
                auto& _l_xptd_test_data{_l_internal_test_data[_l_idx]};
                auto& [_l_status, _l_passed, _l_can_terminate, _l_terminated, _l_xptd_idx, _l_str, _l_additional_data, _l_tree, _l_annotation]{
                    _l_xptd_test_data
                };
                _TVLOG(_l_xptd_test_data);
                abc_test_test::utility::check_matcher<T>(
                    _l_assertion,
                    _l_status,
                    _l_passed,
                    _l_terminated,
                    _l_xptd_idx,
                    make_tuple(
                        _l_passed,
                        _l_can_terminate,
                        _l_str,
                        _l_additional_data,
                        _l_tree,
                        _l_annotation
                    )
                );
            }
        }
    };
    vector<function<void()>> _l_check_funcs
        = {[&]()
           {
               _CHECK(true_matcher());
               _CHECK(false_matcher());
           },
           [&]()
           {
               _CHECK_EXPR(2 == 2);
               _CHECK_EXPR(2 == 3);
           },
           [&]()
           {
               _CHECK(true_matcher() && true_matcher());
               _CHECK(true_matcher() && false_matcher());
              _CHECK(false_matcher() && true_matcher());
               _CHECK(false_matcher() && false_matcher());
           },
           [&]()
           {
               _CHECK(true_matcher() || true_matcher());
               _CHECK(true_matcher() || false_matcher());
               _CHECK(false_matcher() || true_matcher());
               _CHECK(false_matcher() || false_matcher());
           },
           [&]()
           {
               _CHECK(! false_matcher());
               _CHECK(! true_matcher());
           }};
    _l_run_func.operator()<pass_or_fail_t>(_l_check_funcs, "check_tests");

    vector<function<void()>> _l_require_funcs
        = {[&]()
           {
               _REQUIRE(true_matcher());
               _REQUIRE(false_matcher());
               _REQUIRE(true_matcher());
           },
           [&]()
           {
               _REQUIRE_EXPR(2 == 2);
               _REQUIRE_EXPR(2 == 3);
               _REQUIRE_EXPR(2 == 2);
           },
           [&]()
           {
               _REQUIRE(true_matcher() && true_matcher());
               _REQUIRE(true_matcher() && false_matcher());
               _REQUIRE(false_matcher() && true_matcher());
               _REQUIRE(false_matcher() && false_matcher());
           },
           [&]()
           {
               _REQUIRE(false_matcher() && true_matcher());
               _REQUIRE(false_matcher() && false_matcher());
           },
           [&]()
           {
               _REQUIRE(false_matcher() && false_matcher());
               _REQUIRE(false_matcher() && true_matcher());
           },
           [&]()
           {
               _REQUIRE(true_matcher() || true_matcher());
               _REQUIRE(true_matcher() || false_matcher());
               _REQUIRE(false_matcher() || true_matcher());
               _REQUIRE(false_matcher() || false_matcher());
           },
           [&]()
           {
               _REQUIRE(! false_matcher());
               _REQUIRE(! true_matcher());
               _REQUIRE(true_matcher());
           }};

    _l_run_func.operator(
    )<pass_or_terminate_t>(_l_require_funcs, "require_tests");
}

/*_TEST_CASE(
    abc::test_case_t(
        {.name = "Check _CHECK macro works in a multi-matcher correctly",
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
    using namespace _ABC_NS_REPORTS;
    using namespace _ABC_NS_MATCHER;
    function<void()> _l_test_func_1 = [&]()
    {
        auto multi_matcher = _MULTI_MATCHER("Multi matcher 1");
        multi_matcher << _CHECK(true_matcher());
        multi_matcher << _CHECK(false_matcher());
        _CHECK(multi_matcher);
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
    abc_test_test::utility::check_multi_matcher<pass_or_fail_t>(
        assertion_true,
        false,
        true,
        make_optional(u8"Multi matcher 1"),
        std::vector<std::tuple<
            bool,
            bool,
            std::u8string,
            std::vector<std::u8string>,
            std::vector<
                abc::matcher::matcher_result_infos_t::tree_structure_t>>>({})
    );
}*/