#pragma once
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/after_execution_test_report.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/list_formatter.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/print_config.hpp"
_BEGIN_ABC_REPORTERS_NS

struct test_report_list_formatter
    : public list_vertical_formattable_t<
          ds::invoked_test_data_t,
          enum_after_execution_test_report_fields_t,
          print_config_t>
{
public:
    __constexpr virtual bool
        check_data(
            const enum_after_execution_test_report_fields_t& _a_fid,
            const ds::invoked_test_data_t&                   _a_element
        ) const;
    __no_constexpr virtual std::pair<std::u8string, std::vector<std::u8string>>
        get_data(
            const enum_after_execution_test_report_fields_t& _a_fid,
            const ds::invoked_test_data_t&                   _a_element,
            const print_config_t&                            _a_pc
        ) const;
};

_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__constexpr_imp bool
    test_report_list_formatter::check_data(
        const enum_after_execution_test_report_fields_t& _a_fid,
        const ds::invoked_test_data_t&                   _a_element
    ) const
{
    using enum enum_after_execution_test_report_fields_t;
    switch (_a_fid)
    {
    case STR_STATUS:
    case PASSED:
    case NAME:
    case SOURCE_LOCATION:
    case SOURCE_REPRESENTATION:
    case TEST_PATH:
    case SEED_USED:
    case SEED_USED_HEX:
    case SEED_TO_USE_TO_RE_RUN_TEST:
    case SEED_TO_USE_TO_RE_RUN_TEST_IN_HEX:
    case TEST_WARNINGS_RECIEVED:
    case ASSERTIONS_RECIEVED:
    case ASSERTIONS_PASSED:
    case ASSERTIONS_FAILED:
    case COMPACT_ASSERTION_DATA:
    case TERMINATION_STATUS:
    case TIME_TAKEN:
        return true;
    case DESCRIPTION:
        return _a_element.post_setup_test_data()
            .registered_test_data()
            ._m_user_data.description.has_value();
    default:
        throw errors::unaccounted_for_enum_exception(_a_fid);
    }
}

__no_constexpr_imp std::pair<std::u8string, std::vector<std::u8string>>
                   test_report_list_formatter::get_data(
        const enum_after_execution_test_report_fields_t& _a_fid,
        const ds::invoked_test_data_t&                   _a_element,
        const print_config_t&                            _a_pc
    ) const
{
    using namespace std;
    using namespace _ABC_NS_DS;
    using namespace abc::utility::printer;
    using namespace _ABC_NS_UTILITY_STR;
    using enum enum_after_execution_test_report_fields_t;
    pair<string, string> _l_pair;
    switch (_a_fid)
    {
    case TIME_TAKEN:
        return {
            _a_pc.space(_a_pc.colon(_a_pc.time_taken_str())),
            {_a_pc.slight_highlight(
                _a_pc.time_taken(_a_element.time_begin(), _a_element.time_end())
            )}
        };
    case DESCRIPTION:
        return {
            _a_pc.space(_a_pc.colon(_a_pc.test_description_str())),
            {_a_pc.slight_highlight(_a_pc.message_str(
                _a_element.post_setup_test_data()
                        .registered_test_data()
                        ._m_user_data.description.has_value()
                    ? optional<u8string>{}
                    : cast_string_to_u8string(
                          _a_element.post_setup_test_data()
                              .registered_test_data()
                              ._m_user_data.description.value()
                      )
            ))}
        };
    case STR_STATUS:
        return {
            _a_pc.space(_a_pc.colon(_a_pc.str_status_str())),
            {_a_pc.str_status(ds::test_passed(_a_element.test_status()))}
        };
    case PASSED:
        return {
            _a_pc.space(_a_pc.colon(_a_pc.passed_str())),
            {_a_pc.passed(ds::test_passed(_a_element.test_status()))}
        };
    case NAME:
        return {
            _a_pc.space(_a_pc.colon(_a_pc.name_str())),
            {_a_pc.name(cast_string_to_u8string(_a_element
                                                      .post_setup_test_data()
                                                      .registered_test_data()
                                                      ._m_user_data.name))}
        };
    case SOURCE_LOCATION:
    {
        const optional<single_source_t> _l_opt{
            _a_element.post_setup_test_data().registered_test_data()._m_source
        };
        return {
            _a_pc.space(_a_pc.colon(_a_pc.source_location_str())),
            {_a_pc.source_location(
                _l_opt.has_value()
                    ? optional<source_location>{_l_opt.value().source_location()
                      }
                    : optional<source_location>{}
            )}
        };
    }
    case SOURCE_REPRESENTATION:
    {
        const optional<single_source_t> _l_opt{
            _a_element.post_setup_test_data().registered_test_data()._m_source
        };
        return {
            _a_pc.space(_a_pc.colon(_a_pc.source_code_str())),
            {_a_pc.source_representation(
                _l_opt.has_value()
                    ? optional<u8string>{cast_string_to_u8string(
                          _l_opt.value().source_code_representation()
                      )}
                    : optional<u8string>{}
            )}
        };
    }
    break;
    case TEST_PATH:
        return {
            _a_pc.space(_a_pc.colon(_a_pc.test_path_str())),
            {_a_pc.test_path(cast_string_to_u8string(
                _a_element.post_setup_test_data()
                    .registered_test_data()
                    ._m_user_data.path
            ))}
        };
    case SEED_USED:
        return {
            _a_pc.space(_a_pc.colon(_a_pc.seed_used_str())),
            {_a_pc.seed_used(
                _a_element.post_setup_test_data().has_for_loop_stack_trie()
                    ? optional<u8string>(
                          compressed_typless_data_generator_collection_stack_trie_printer_t<
                              false>{}
                              .run_printer(_a_element.post_setup_test_data()
                                               .for_loop_stack_trie())
                      )
                    : optional<u8string>()
            )}
        };
    case SEED_USED_HEX:
        return {
            _a_pc.space(_a_pc.colon(_a_pc.seed_used_in_hex_str())),
            {_a_pc.seed_used_in_hex(
                _a_element.post_setup_test_data().has_for_loop_stack_trie()
                    ? optional<u8string>(
                          compressed_typless_data_generator_collection_stack_trie_printer_t<
                              true>{}
                              .run_printer(_a_element.post_setup_test_data()
                                               .for_loop_stack_trie())
                      )
                    : optional<u8string>()
            )}
        };
    case SEED_TO_USE_TO_RE_RUN_TEST:
        return {
            _a_pc.space(_a_pc.colon(_a_pc.seed_to_use_to_re_run_test_str())),
            {_a_pc.seed_to_use_to_re_run_test(
                compressed_typless_data_generator_collection_stack_trie_printer_t<
                    false>{}
                    .run_printer(_a_element.for_loop_stack_trie())
            )}
        };
    case SEED_TO_USE_TO_RE_RUN_TEST_IN_HEX:
        return {
            _a_pc.space(_a_pc.colon(_a_pc.seed_to_use_to_re_run_test_in_hex_str(
            ))),
            {_a_pc.seed_to_use_to_re_run_test_in_hex(
                compressed_typless_data_generator_collection_stack_trie_printer_t<
                    true>{}
                    .run_printer(_a_element.for_loop_stack_trie())
            )}
        };
    case TEST_WARNINGS_RECIEVED:
        return {
            _a_pc.space(_a_pc.colon(_a_pc.test_warnings_recieved_str())),
            {_a_pc.test_warnings_recieved(_a_element.warnings_recieved())}
        };
    case ASSERTIONS_RECIEVED:
        return {
            _a_pc.space(_a_pc.colon(_a_pc.assertions_recieved_str())),
            {_a_pc.print_integer(_a_element.assertions_recieved())}
        };
    case ASSERTIONS_PASSED:
        return {
            _a_pc.space(_a_pc.colon(_a_pc.assertions_passed_str())),
            {_a_pc.assertions_passed(_a_element.assertions_passed())}
        };
    case ASSERTIONS_FAILED:
        return {
            _a_pc.space(_a_pc.colon(_a_pc.assertions_failed_str())),
            {_a_pc.assertions_failed(_a_element.assertions_failed())}
        };
    case COMPACT_ASSERTION_DATA:
        return {
            _a_pc.space(_a_pc.colon(_a_pc.compat_assertion_data_str())),
            {_a_pc.compat_assertion_data(
                _a_element.assertions_passed(),
                _a_element.assertions_recieved(),
                _a_element.assertions_failed()
            )}
        };
    case TERMINATION_STATUS:
        return {
            _a_pc.space(_a_pc.colon(_a_pc.termination_status_str())),
            {_a_pc.test_status(_a_element.test_status())}
        };
    default:
        throw errors::unaccounted_for_enum_exception(_a_fid);
    }
}

_END_ABC_REPORTERS_NS