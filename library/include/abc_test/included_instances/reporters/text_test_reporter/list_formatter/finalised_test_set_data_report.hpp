#pragma once
#include "abc_test/core/ds/test_data/finalised_test_set_data.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/finalised_test_set_data_report.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/list_formatter.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/print_config.hpp"
_BEGIN_ABC_REPORTERS_NS

struct finalised_test_set_data_list_formatter
    : public list_formattable_t<
          ds::finalised_test_set_data_t,
          enum_finalised_test_set_data_fields_t,
          print_config_t>
{
public:
    __constexpr virtual bool
        check_data(
            const enum_finalised_test_set_data_fields_t& _a_fid,
            const ds::finalised_test_set_data_t&         _a_element
        ) const;
    __no_constexpr virtual void
        get_data(
            const enum_finalised_test_set_data_fields_t&             _a_fid,
            const ds::finalised_test_set_data_t&                     _a_element,
            const print_config_t&                                    _a_pc,
            const abc::utility::io::threated_text_output_reporter_t& _a_ttor,
            const std::size_t _a_idx
        ) const;
};

_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__constexpr_imp bool
    finalised_test_set_data_list_formatter::check_data(
        const enum_finalised_test_set_data_fields_t& _a_fid,
        const ds::finalised_test_set_data_t&         _a_element
    ) const
{
    using enum enum_finalised_test_set_data_fields_t;
    switch (_a_fid)
    {
    case TOTAL_TESTS_RAN:
    case TOTAL_TESTS_PASSED:
    case TOTAL_TESTS_FAILED:
    case TOTAL_TESTS_FAILED_AND_TERMINATED:
    case TOTAL_TESTS_FAILED_BUT_NOT_TERMINATED:
    case TOTAL_TESTS_DESIGNATED_FAILURE_DUE_TO_EXCEPTION:
    case TOTAL_ASSERTIONS_RAN:
    case TOTAL_ASSERTIONS_PASSED:
    case TOTAL_ASSERTIONS_FAILED:
    case TDG_COLLECTION_STACK_TRIES_MAP:
        return true;
    default:
        throw errors::unaccounted_for_enum_exception(_a_fid);
    }
}

__no_constexpr_imp void
                finalised_test_set_data_list_formatter::get_data(
        const enum_finalised_test_set_data_fields_t&             _a_fid,
        const ds::finalised_test_set_data_t&                     _a_element,
        const print_config_t&                                    _a_pc,
        const abc::utility::io::threated_text_output_reporter_t& _a_ttor,
                    const std::size_t _a_idx
    ) const
{
    using namespace std;
    using enum enum_finalised_test_set_data_fields_t;
    pair<u8string, u8string> _l_pair;
    switch (_a_fid)
    {
    case TOTAL_TESTS_RAN:
        _l_pair = {
            _a_pc.space(_a_pc.colon(_a_pc.str_total_tests_ran())),
            _a_pc.print_integer(_a_element.total_tests_ran())
        };
        break;
    case TOTAL_TESTS_PASSED:
        _l_pair = {
            _a_pc.space(_a_pc.colon(_a_pc.str_total_tests_passed())),
            _a_pc.highlight_pass(
                _a_pc.print_integer(_a_element.total_tests_passed())
            )
        };
        break;
    case TOTAL_TESTS_FAILED:
        _l_pair = {
            _a_pc.space(_a_pc.colon(_a_pc.str_total_tests_failed())),
            _a_pc.highlight_fail(
                _a_pc.print_integer(_a_element.total_tests_failed())
            )
        };
        break;
    case TOTAL_TESTS_FAILED_AND_TERMINATED:
        _l_pair = {
            _a_pc.space(_a_pc.colon(_a_pc.str_total_tests_failed_and_terminated(
            ))),
            _a_pc.highlight_fail(_a_pc.print_integer(
                _a_element.total_tests_failed_and_terminated()
            ))
        };
        break;
    case TOTAL_TESTS_FAILED_BUT_NOT_TERMINATED:
        _l_pair = {
            _a_pc.space(
                _a_pc.colon(_a_pc.str_total_tests_failed_but_not_terminated())
            ),
            _a_pc.highlight_fail(_a_pc.print_integer(
                _a_element.total_tests_failed_but_not_terminated()
            ))
        };
        break;
    case TOTAL_TESTS_DESIGNATED_FAILURE_DUE_TO_EXCEPTION:
        _l_pair = {
            _a_pc.space(_a_pc.colon(
                _a_pc.str_total_tests_designated_failure_due_to_exception()
            )),
            _a_pc.highlight_fail(_a_pc.print_integer(
                _a_element.total_tests_designated_failure_due_to_exception()
            ))
        };
        break;
    case TOTAL_ASSERTIONS_RAN:
        _l_pair = {
            _a_pc.space(_a_pc.colon(_a_pc.str_total_assertions_ran())),
            _a_pc.slight_highlight(
                _a_pc.print_integer(_a_element.total_assertions_ran())
            )
        };
        break;
    case TOTAL_ASSERTIONS_PASSED:
        _l_pair = {
            _a_pc.space(_a_pc.colon(_a_pc.str_total_assertions_passed())),
            _a_pc.highlight_pass(
                _a_pc.print_integer(_a_element.total_assertions_passed())
            )
        };
        break;
    case TOTAL_ASSERTIONS_FAILED:
        _l_pair = {
            _a_pc.space(_a_pc.colon(_a_pc.str_total_assertions_failed())),
            _a_pc.highlight_fail(
                _a_pc.print_integer(_a_element.total_assertions_failed())
            )
        };
        break;
    case RNG_SEED:
        _l_pair = {
            _a_pc.space(_a_pc.colon(_a_pc.str_seed())),
            _a_pc.slight_highlight(_a_pc.print_seed(_a_element.seed()))
        };
        break;
    case TDG_COLLECTION_STACK_TRIES_MAP:
        _l_pair = {
            _a_pc.space(_a_pc.colon(_a_pc.str_tdg_collection_stack_tries_map())
            ),
            _a_pc.slight_highlight(_a_pc.print_tdg_collection_stack_tries_map(
                _a_element.map_ids_to_tdg_collection_stack_tries()
            ))
        };
        break;
    default:
        throw errors::unaccounted_for_enum_exception(_a_fid);
    }
    _a_ttor.write(fmt::format(u8"{0}{1}", _l_pair.first, _l_pair.second));
}

_END_ABC_REPORTERS_NS