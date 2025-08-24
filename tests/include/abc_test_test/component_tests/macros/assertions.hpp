#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/included_instances/reporters/memoized_error_reporter.hpp"
#include "abc_test/included_instances/reporters/memoized_test_reporter.hpp"
#include "abc_test/utility/str/unicode.hpp"
#include "abc_test_test/testing_utilities/bootstrapped_testing_utilities.hpp"

namespace abc_test_test::utility
{
inline void
    check_matcher_result(
        const abc::matcher::bba_inner_assertion_type_t& _a_actual,
        const abc::matcher::bba_inner_assertion_type_t& _a_xptd
    )

{
    _CHECK_EXPR(_a_actual.annotation() == _a_xptd.annotation());
    _CHECK_EXPR(_a_actual.assertion_type() == _a_xptd.assertion_type());
    _CHECK_EXPR(_a_actual.can_terminate() == _a_xptd.can_terminate());
    auto& _l_matcher_res{_a_actual.matcher_result()};
    auto& _l_xptd_matcher_res{_a_xptd.matcher_result()};
    _CHECK_EXPR(_l_matcher_res.passed() == _l_xptd_matcher_res.passed());
    auto& actual_matcher_str{_l_matcher_res.str()};
    auto& xptd_matcher_str{_l_xptd_matcher_res.str()};
    _CHECK_EXPR(
        actual_matcher_str.get_vector() == xptd_matcher_str.get_vector()
    );
    _CHECK_EXPR(
        actual_matcher_str.primary_data() == xptd_matcher_str.primary_data()
    );
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
        _l_tree_checker,
        actual_matcher_str.get_tree(),
        xptd_matcher_str.get_tree()
    );
}

template <typename T>
inline void
    check_matcher(
        const abc::reports::assertion_base_ptr_t&       _a_pstr,
        const abc::reports::status_t<T>&                _a_expected_status,
        const bool                                      _a_passed,
        const bool                                      _a_terminated,
        const std::size_t                               _a_index,
        const abc::matcher::bba_inner_assertion_type_t& _a_xptd
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
        _CHECK_EXPR(_l_element.status() == _a_expected_status);
        _CHECK_EXPR(_l_element.get_pass_status() == _a_passed);
        _CHECK_EXPR(_l_element.terminated() == _a_terminated);
        _CHECK_EXPR(_l_element.assertion_index() == _a_index);
        auto& matcher1{_l_element.matcher_info()};
        check_matcher_result(matcher1, _a_xptd);
    }
    catch (const std::exception& _a_exception)
    {
        _REQUIRE_EXPR(true == false);
    }
}

template <typename T1, typename T2>
inline void
    check_multi_matcher(
        const abc::reports::assertion_base_ptr_t& _a_pstr,
        const std::variant<
            abc::reports::status_t<T1>,
            abc::reports::status_t<T2>>& _a_expected_status,
        const bool                       _a_passed,
        const bool                       _a_terminated,
        const std::size_t                _a_index,
        const std::string                _a_multi_matcher_name,
        const std::vector<abc::matcher::bba_inner_assertion_type_t>& _a_xptd
    )
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    using namespace abc::utility::io;
    using namespace _ABC_NS_UTILITY_STR;
    using namespace _ABC_NS_DS;
    using namespace _ABC_NS_MATCHER;
    using namespace _ABC_NS_REPORTS;
    bool _l_found_entity{false};
    auto _l_checker = [&]<typename T, size_t I>()
    {
        if (_l_found_entity)
        {
            return;
        }
        try
        {
            const multi_element_assertion_block_t<T>& _l_element{
                dynamic_cast<const multi_element_assertion_block_t<T>&>(
                    *_a_pstr.get()
                )
            };
            if (_a_expected_status.index() == I)
            {
                _l_found_entity = true;
            }
            else
            {
                return;
            }
            _CHECK_EXPR(_l_element.status() == get<I>(_a_expected_status));
            _CHECK_EXPR(_l_element.get_pass_status() == _a_passed);
            _CHECK_EXPR(_l_element.terminated() == _a_terminated);
            _CHECK_EXPR(_l_element.assertion_index() == _a_index);
            _CHECK_EXPR(_l_element.test_description() == _a_multi_matcher_name);
            auto& _l_matchers{_l_element.get_matcher()};
            _REQUIRE_EXPR(_l_matchers.size() == _a_xptd.size());
            for (size_t _l_idx{0}; _l_idx < _l_matchers.size(); ++_l_idx)
            {
                auto& _l_internal_matcher{_l_matchers[_l_idx]};
                auto& _l_xptd_matcher{_a_xptd[_l_idx]};
                check_matcher_result(_l_internal_matcher, _l_xptd_matcher);
            }
        }
        catch (const std::exception& _a_exception)
        {}
    };
    _l_checker.operator()<T1, 0>();
    _l_checker.operator()<T2, 1>();
    if (not _l_found_entity)
    {
        _REQUIRE_EXPR(true == false);
    }
}
} // namespace abc_test_test::utility

_BEGIN_ABC_UTILITY_NS
template <>
__constexpr enum_list_t<abc::matcher::enum_bba_inner_assertion_type_t>
            get_enum_list() noexcept
{
    using enum abc::matcher::enum_bba_inner_assertion_type_t;
    return {
        _ENUM_LIST_ENTRY(STATIC_ASSERTION),
        _ENUM_LIST_ENTRY(MATCHER_BASED_ASSERTION)
    };
}

_END_ABC_UTILITY_NS


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

template <>
struct default_printer_t<abc::matcher::matcher_result_t>
    : public printer_base_t<abc::matcher::matcher_result_t>
{
    __no_constexpr_imp std::u8string
                       run_printer(
                           const abc::matcher::matcher_result_t& _a_object
                       ) const
    {
        using namespace std;
        using namespace _ABC_NS_CLI;
        using namespace _ABC_NS_UTILITY_STR;
        using namespace _ABC_NS_MATCHER;
        return fmt::format(
            u8"({0},{1})",
            _a_object.passed(),
            default_printer_t<abc::matcher::matcher_result_infos_t>{}
                .run_printer(_a_object.str())
        );
    }
};

template <>
struct default_printer_t<abc::matcher::bba_inner_assertion_type_t>
    : public printer_base_t<abc::matcher::bba_inner_assertion_type_t>
{
    __no_constexpr_imp std::u8string
                       run_printer(
                           const abc::matcher::bba_inner_assertion_type_t& _a_object
                       ) const
    {
        using namespace std;
        using namespace _ABC_NS_CLI;
        using namespace _ABC_NS_UTILITY_STR;
        using namespace _ABC_NS_MATCHER;
        return fmt::format(
            u8"({0},{1},{2},{3})",
            default_printer_t<matcher_result_t>{}.run_printer(
                _a_object.matcher_result()
            ),
            _a_object.annotation(),
            default_printer_t<enum_bba_inner_assertion_type_t>{}.run_printer(
                _a_object.assertion_type()
            ),
            _a_object.can_terminate()
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
        auto _l_result = default_parser_t<tuple<
            vector<
                tuple<u8string, u8string, shared_ptr<matcher_result_infos_t>>>,
            u8string,
            vector<u8string>>>{}
                             .run_parser(_a_parse_input);
        if (_l_result.has_value())
        {
            auto& _l_res{_l_result.value()};
            return matcher_result_infos_t(
                get<1>(_l_res), get<2>(_l_res), get<0>(_l_res)
            );
        }
        else
        {
            return unexpected(_l_result.error());
        }
    }
};

template <>
struct default_parser_t<abc::matcher::matcher_result_t>
    : public parser_base_t<abc::matcher::matcher_result_t>
{
    __no_constexpr_imp result_t<abc::matcher::matcher_result_t>
                       run_parser(
                           parser_input_t& _a_parse_input
                       ) const
    {
        using namespace std;
        using namespace _ABC_NS_MATCHER;
        auto _l_result = default_parser_t<tuple<bool, matcher_result_infos_t>>{}
                             .run_parser(_a_parse_input);
        if (_l_result.has_value())
        {
            auto& _l_res{_l_result.value()};
            return matcher_result_t(get<0>(_l_res), get<1>(_l_res));
        }
        else
        {
            return unexpected<u8string>(_l_result.error());
        }
    }
};

template <>
struct default_parser_t<abc::matcher::bba_inner_assertion_type_t>
    : public parser_base_t<abc::matcher::bba_inner_assertion_type_t>
{
    __no_constexpr_imp result_t<abc::matcher::bba_inner_assertion_type_t>
                       run_parser(
                           parser_input_t& _a_parse_input
                       ) const
    {
        using namespace std;
        using namespace _ABC_NS_MATCHER;
        auto _l_result = default_parser_t<tuple<
            matcher_result_t,
            optional<u8string>,
            enum_bba_inner_assertion_type_t,
            bool>>{}
                             .run_parser(_a_parse_input);
        if (_l_result.has_value())
        {
            auto& _l_res{_l_result.value()};
            return bba_inner_assertion_type_t(
                abc::ds::log_infos_t(),
                get<3>(_l_res),
                get<0>(_l_res),
                std::nullopt,
                get<1>(_l_res),
                matcher_source_map_t(),
                get<2>(_l_res)
            );
        }
        else
        {
            return unexpected<u8string>(_l_result.error());
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
            auto _l_pair{ abc_test::utility::make_name_pair(_l_idx) };
            _l_funcs_to_run.push_back(make_tuple(
                _l_func,
                _l_pair.second,
                _l_pair.first,
                tdg_collection_stack_trie_t{}
            ));
            ++_l_idx;
        }
        auto _l_rv = abc_test::utility::abc_test_tests_internal_test_runner(
            1, true, _l_funcs_to_run
        );
        auto& _l_tests = _l_rv.memoized_test_reporter._m_tests;
        using internal_test_data_t
            = tuple<bool, bool, bool, size_t, bba_inner_assertion_type_t>;
        using test_data_t
            = tuple<size_t, size_t, size_t, vector<internal_test_data_t>>;
        for (auto& [_l_test_index, _l_test_info] :
             read_data_from_file<pair<size_t, test_data_t>>(_a_unit_test_name))
        {
            auto& [_l_passed, _l_failed, _l_recieved, _l_internal_test_data]{
                _l_test_info
            };
            auto _l_pair{ abc_test::utility::make_name_pair(_l_test_index) };
            auto& _l_test = _l_tests.at(_l_pair);
            _CHECK_EXPR(_l_test.assertions_passed == _l_passed);
            _CHECK_EXPR(_l_test.assertions_failed == _l_failed);
            _CHECK_EXPR(_l_test.assertions_recieved == _l_recieved);
            auto& _l_assertions{_l_test.assertions};
            _REQUIRE_EXPR(_l_assertions.size() == _l_internal_test_data.size());
            for (size_t _l_idx{0}; _l_idx < _l_assertions.size(); ++_l_idx)
            {
                auto& _l_assertion{_l_assertions[_l_idx]};
                auto& _l_xptd_test_data{_l_internal_test_data[_l_idx]};
                auto& [_l_status, _l_passed, _l_terminated, _l_xptd_idx, _l_bba]{
                    _l_xptd_test_data
                };
                _TVLOG(_l_xptd_test_data);
                abc_test_test::utility::check_matcher<T>(
                    _l_assertion,
                    _l_status,
                    _l_passed,
                    _l_terminated,
                    _l_xptd_idx,
                    _l_bba
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

_TEST_CASE(
    abc::test_case_t(
        {.name
         = "Checking _CHECK and _REQUIRE macros as part of multi-matchers",
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
    auto _l_run_func = [&]<typename T1, typename T2>(
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
            auto _l_name_pair{ abc_test::utility::make_name_pair(_l_idx) };
            _l_funcs_to_run.push_back(make_tuple(
                _l_func,
                _l_name_pair.second,
                _l_name_pair.first,
                tdg_collection_stack_trie_t{}
            ));
            ++_l_idx;
        }
        auto _l_rv = abc_test::utility::abc_test_tests_internal_test_runner(
            1, true, _l_funcs_to_run
        );
        auto& _l_tests             = _l_rv.memoized_test_reporter._m_tests;
        using internal_test_data_t = tuple<
            std::
                variant<abc::reports::status_t<T1>, abc::reports::status_t<T2>>,
            bool,
            bool,
            size_t,
            string,
            vector<bba_inner_assertion_type_t>>;
        using test_data_t
            = tuple<size_t, size_t, size_t, vector<internal_test_data_t>>;
        for (auto& [_l_test_index, _l_test_info] :
             read_data_from_file<pair<size_t, test_data_t>>(_a_unit_test_name))
        {
            auto& [_l_passed, _l_failed, _l_recieved, _l_internal_test_data]{
                _l_test_info
            };
            auto _l_name_pair{ abc_test::utility::make_name_pair(_l_test_index) };
            auto& _l_test = _l_tests.at(_l_name_pair);
            _CHECK_EXPR(_l_test.assertions_passed == _l_passed);
            _CHECK_EXPR(_l_test.assertions_failed == _l_failed);
            _CHECK_EXPR(_l_test.assertions_recieved == _l_recieved);
            auto& _l_assertions{_l_test.assertions};
            _REQUIRE_EXPR(_l_assertions.size() == _l_internal_test_data.size());
            for (size_t _l_idx{0}; _l_idx < _l_assertions.size(); ++_l_idx)
            {
                auto& _l_assertion{_l_assertions[_l_idx]};
                auto& _l_xptd_test_data{_l_internal_test_data[_l_idx]};
                auto& [_l_status, _l_passed, _l_terminated, _l_xptd_idx, _l_multi_matcher_name, _l_bbas]{
                    _l_xptd_test_data
                };
                _TVLOG(_l_xptd_test_data);
                abc_test_test::utility::check_multi_matcher<T1, T2>(
                    _l_assertion,
                    _l_status,
                    _l_passed,
                    _l_terminated,
                    _l_xptd_idx,
                    _l_multi_matcher_name,
                    _l_bbas
                );
            }
        }
    };
    vector<function<void()>> _l_check_funcs
        = {
           [&]()
           {
               auto multi_matcher = _MULTI_MATCHER("multi matcher");
               _CHECK(multi_matcher);
           },
           [&]()
           {
               auto multi_matcher = _MULTI_MATCHER("multi matcher");
               _REQUIRE(multi_matcher);
           },
           [&]()
           {
               auto multi_matcher = _MULTI_MATCHER("multi matcher");
               multi_matcher << _CHECK(true_matcher());
               multi_matcher << _CHECK(false_matcher());
               _CHECK(multi_matcher);
           },
           [&]()
           {
               auto multi_matcher = _MULTI_MATCHER("multi matcher");
               multi_matcher << _CHECK(true_matcher());
               multi_matcher << _REQUIRE(false_matcher());
               multi_matcher << _CHECK(true_matcher());
               _CHECK(multi_matcher);
           },
           [&]()
           {
               auto multi_matcher = _MULTI_MATCHER("multi matcher");
               multi_matcher << _CHECK(true_matcher());
               multi_matcher << _CHECK(false_matcher());
               multi_matcher << _CHECK(true_matcher());
               _REQUIRE(multi_matcher);
           },
           [&]()
           {
               auto multi_matcher = _MULTI_MATCHER("multi matcher");
               multi_matcher << _CHECK_EXPR(2 == 2);
               multi_matcher << _CHECK_EXPR(2 == 3);
               _CHECK(multi_matcher);
           },
           [&]()
           {
               auto multi_matcher = _MULTI_MATCHER("multi matcher");
               multi_matcher << _CHECK_EXPR(2 == 2);
               multi_matcher << _REQUIRE_EXPR(2 == 3);
               multi_matcher << _CHECK_EXPR(2 == 2);
               _CHECK(multi_matcher);
           },
           [&]()
           {
               auto multi_matcher = _MULTI_MATCHER("multi matcher");
               multi_matcher << _CHECK_EXPR(2 == 2);
               multi_matcher << _CHECK_EXPR(2 == 3);
               _REQUIRE(multi_matcher);
           },
           [&]()
           {
               auto multi_matcher = _MULTI_MATCHER("multi matcher");
               multi_matcher << _CHECK(true_matcher() && true_matcher());
               multi_matcher << _CHECK(true_matcher() && false_matcher());
               multi_matcher << _CHECK(false_matcher() && true_matcher());
               multi_matcher << _CHECK(false_matcher() && false_matcher());
               _CHECK(multi_matcher);
           },
           [&]()
           {
               auto multi_matcher = _MULTI_MATCHER("multi matcher");
               multi_matcher << _CHECK(true_matcher() && true_matcher());
               multi_matcher << _CHECK(true_matcher() && false_matcher());
               multi_matcher << _REQUIRE(false_matcher() && true_matcher());
               multi_matcher << _CHECK(false_matcher() && false_matcher());
               _CHECK(multi_matcher);
           },
           [&]()
           {
               auto multi_matcher = _MULTI_MATCHER("multi matcher");
               multi_matcher << _CHECK(true_matcher() && true_matcher());
               multi_matcher << _CHECK(true_matcher() && false_matcher());
               multi_matcher << _CHECK(false_matcher() && true_matcher());
               multi_matcher << _CHECK(false_matcher() && false_matcher());
               _REQUIRE(multi_matcher);
           },
           [&]()
           {
               auto multi_matcher = _MULTI_MATCHER("multi matcher");
               multi_matcher << _CHECK(true_matcher() || true_matcher());
               multi_matcher << _CHECK(true_matcher() || false_matcher());
               multi_matcher << _CHECK(false_matcher() || true_matcher());
               multi_matcher << _CHECK(false_matcher() || false_matcher());
               _CHECK(multi_matcher);
           },
           [&]()
           {
               auto multi_matcher = _MULTI_MATCHER("multi matcher");
               multi_matcher << _CHECK(true_matcher() || true_matcher());
               multi_matcher << _CHECK(true_matcher() || false_matcher());
               multi_matcher << _REQUIRE(false_matcher() || true_matcher());
               multi_matcher << _REQUIRE(false_matcher() || false_matcher());
               multi_matcher << _CHECK(true_matcher() || true_matcher());
               _CHECK(multi_matcher);
           },
           [&]()
           {
               auto multi_matcher = _MULTI_MATCHER("multi matcher");
               multi_matcher << _CHECK(true_matcher() || true_matcher());
               multi_matcher << _CHECK(true_matcher() || false_matcher());
               multi_matcher << _CHECK(false_matcher() || true_matcher());
               multi_matcher << _CHECK(false_matcher() || false_matcher());
               _REQUIRE(multi_matcher);
           },
           [&]()
           {
               auto multi_matcher = _MULTI_MATCHER("multi matcher");
               multi_matcher << _CHECK(! false_matcher());
               multi_matcher << _CHECK(! true_matcher());
               _CHECK(multi_matcher);
           },
           [&]()
           {
               auto multi_matcher = _MULTI_MATCHER("multi matcher");
               multi_matcher << _CHECK(! false_matcher());
               multi_matcher << _REQUIRE(! true_matcher());
               multi_matcher << _CHECK(! false_matcher());
               _CHECK(multi_matcher);
           },
           [&]()
           {
               auto multi_matcher = _MULTI_MATCHER("multi matcher");
               multi_matcher << _CHECK(! false_matcher());
               multi_matcher << _CHECK(! true_matcher());
               _REQUIRE(multi_matcher);
           }};
    _l_run_func.operator(
    )<pass_or_fail_t, pass_or_terminate_t>(_l_check_funcs, "check_tests");
}