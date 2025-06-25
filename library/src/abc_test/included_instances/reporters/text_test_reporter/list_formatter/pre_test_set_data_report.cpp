#include "abc_test/included_instances/reporters/text_test_reporter/list_formatter/pre_test_set_data_report.hpp"

#include "abc_test/included_instances/options/included_instances_test_options.hpp"
#include "abc_test/included_instances/options/option_config.hpp"
#include "abc_test/utility/io/threaded_text_output_reporter.hpp"
#include "abc_test/utility/printers/default_printer.hpp"
#include "abc_test/utility/rng.hpp"

_BEGIN_ABC_REPORTERS_NS

__no_constexpr_or_inline_imp std::optional<std::u8string>
                             pre_test_set_data_list_formatter::get_cli_info(
        const enum_pre_test_set_data_fields_t& _a_fid,
        const ds::pre_test_run_report_t&       _a_element,
        const print_config_t&                  _a_pc
    ) const
{
    using namespace std;
    using enum enum_pre_test_set_data_fields_t;
    if (_a_element.get_options().show_configuration_explanations)
    {
        switch (_a_fid)
        {
        case USE_GLOBAL_TEST_LIST:
            return _a_pc.indent(_a_pc.slight_highlight(
                _a_pc.make_source_info(_a_element.get_option_data(
                    _ABC_NS_UTILITY_CLI::detail::_s_global_test_list
                ))
            ));
        case WRITE_DATA_TO_FILES:
            return _a_pc.indent(_a_pc.slight_highlight(
                _a_pc.make_source_info(_a_element.get_option_data(
                    _ABC_NS_UTILITY_CLI::detail::_s_write_data_to_files
                ))
            ));
        case PATH_DELIMITER:
            return _a_pc.indent(_a_pc.slight_highlight(
                _a_pc.make_source_info(_a_element.get_option_data(
                    _ABC_NS_UTILITY_CLI::detail::_s_path_delimiter
                ))
            ));
        case ROOT_PATH:
            return _a_pc.indent(_a_pc.slight_highlight(
                _a_pc.make_source_info(_a_element.get_option_data(
                    _ABC_NS_UTILITY_CLI::detail::_s_root_path
                ))
            ));
        case THREADS:
            return _a_pc.indent(_a_pc.slight_highlight(
                _a_pc.make_source_info(_a_element.get_option_data(
                    _ABC_NS_UTILITY_CLI::detail::_s_threads
                ))
            ));
        case COMMENT_STR:
            return _a_pc.indent(_a_pc.slight_highlight(
                _a_pc.make_source_info(_a_element.get_option_data(
                    _ABC_NS_UTILITY_CLI::detail::_s_comment_str
                ))
            ));
        case GENERAL_DATA_FILE_EXTENSION:
            return _a_pc.indent(_a_pc.slight_highlight(
                _a_pc.make_source_info(_a_element.get_option_data(
                    _ABC_NS_UTILITY_CLI::detail::_s_general_data_file_extension
                ))
            ));
        case N_INTEGERS_USED_FOR_CREATION_OF_RNG:
            return _a_pc.indent(_a_pc.slight_highlight(
                _a_pc.make_source_info(_a_element.get_option_data(
                    _ABC_NS_UTILITY_CLI::detail::
                        _s_integers_used_for_rng_generation
                ))
            ));
        case REPETITION_CONFIG_USED:
            return _a_pc.indent(_a_pc.slight_highlight(
                _a_pc.make_source_info(_a_element.get_option_data(
                    _ABC_NS_UTILITY_CLI::detail::_s_repetition_config
                ))
            ));
        case GLOBAL_SEED:

            return _a_pc.indent(_a_pc.slight_highlight(
                _a_pc.make_source_info(_a_element.get_option_data(
                    _ABC_NS_UTILITY_CLI::detail::_s_global_seed
                ))
            ));
        case FORCE_RUN_ALL_TESTS:
            return _a_pc.indent(_a_pc.slight_highlight(
                _a_pc.make_source_info(_a_element.get_option_data(
                    _ABC_NS_UTILITY_CLI::detail::_s_force_run_all_tests
                ))
            ));
        case TEST_PATHS_TO_RUN:
            return _a_pc.indent(_a_pc.slight_highlight(
                _a_pc.make_source_info(_a_element.get_option_data(
                    _ABC_NS_UTILITY_CLI::detail::_c_test_paths_to_run
                ))
            ));

        default:
            throw errors::unaccounted_for_enum_exception(_a_fid);
        }
    }
    else
    {
        return nullopt;
    }
}

__no_constexpr_or_inline_imp std::optional<std::u8string>
    pre_test_set_data_list_formatter::get_explanation_data(
        const enum_pre_test_set_data_fields_t& _a_fid,
        const ds::pre_test_run_report_t&       _a_element,
        const print_config_t&                  _a_pc
    ) const
{
    using namespace std;
    using enum enum_pre_test_set_data_fields_t;
    if (_a_element.get_options().show_configuration_explanations)
    {
        switch (_a_fid)
        {
        case USE_GLOBAL_TEST_LIST:
            return _a_pc.highlight(_a_pc.use_global_test_list_info(
                _a_element.get_options().use_global_test_list
            ));
        case WRITE_DATA_TO_FILES:
            return _a_pc.highlight(_a_pc.write_data_to_files_info(
                _a_element.get_options().write_data_to_files
            ));
        case PATH_DELIMITER:
            return _a_pc.highlight(_a_pc.path_delimiter_used_info(
                _a_element.get_options().path_delimiter
            ));
        case ROOT_PATH:
            return _a_pc.highlight(
                _a_pc.root_path_info(_a_element.get_options().root_path)
            );
        case THREADS:
            return _a_pc.highlight(
                _a_pc.threads_used_info(_a_element.get_options().threads)
            );
        case COMMENT_STR:
            return _a_pc.highlight(
                _a_pc.comment_str_info(_a_element.get_options().comment_str)
            );
        case GENERAL_DATA_FILE_EXTENSION:
            return _a_pc.highlight(_a_pc.general_data_file_extension_info(
                _a_element.get_options().general_data_extension
            ));
        case N_INTEGERS_USED_FOR_CREATION_OF_RNG:
            return _a_pc.highlight(
                _a_pc.n_integers_used_for_creation_of_rng_info(
                    _a_element.get_options()
                        .number_of_integers_used_to_seed_random_generators
                )
            );
        case REPETITION_CONFIG_USED:
            return _a_pc.highlight(_a_pc.repetition_config_used_info());
        case GLOBAL_SEED:

            return _a_pc.highlight(
                _a_pc.global_seed_info(_a_element.get_options().global_seed)
            );
        case FORCE_RUN_ALL_TESTS:
            return _a_pc.highlight(_a_pc.force_run_all_tests_info(
                _a_element.get_options().force_run_all_tests
            ));
        case TEST_PATHS_TO_RUN:
            return _a_pc.highlight(_a_pc.test_paths_to_run_info(
                _a_element.get_options().test_paths_to_run
            ));
        default:
            throw errors::unaccounted_for_enum_exception(_a_fid);
        }
    }
    else
    {
        return nullopt;
    }
}

__no_constexpr_or_inline_imp std::pair<std::u8string, std::u8string>
                             pre_test_set_data_list_formatter::get_output(
        const enum_pre_test_set_data_fields_t& _a_fid,
        const ds::pre_test_run_report_t&       _a_element,
        const print_config_t&                  _a_pc
    ) const
{
    using namespace std;
    using enum enum_pre_test_set_data_fields_t;
    switch (_a_fid)
    {
    case USE_GLOBAL_TEST_LIST:
        return {
            _a_pc.space(_a_pc.question(_a_pc.global_test_list_used())),
            _a_pc.passed(_a_element.get_options().use_global_test_list)
        };
    case WRITE_DATA_TO_FILES:
        return {
            _a_pc.space(_a_pc.colon(_a_pc.write_data_to_files())),
            _a_pc.passed(_a_element.get_options().write_data_to_files)
        };
    case PATH_DELIMITER:
        return {
            _a_pc.space(_a_pc.colon(_a_pc.path_delimiter_used())),
            _a_pc.quote(_a_element.get_options().path_delimiter)
        };
    case ROOT_PATH:
        return {
            _a_pc.space(_a_pc.colon(_a_pc.root_path_used())),
            _a_pc.show_path(_a_element.get_options().root_path)
        };
    case THREADS:
        return {
            _a_pc.space(_a_pc.colon(_a_pc.threads_used())),
            _a_pc.print_integer(_a_element.get_options().threads)
        };
    case COMMENT_STR:
        return {
            _a_pc.space(_a_pc.colon(_a_pc.comment_str_used())),
            _a_pc.quote(_a_element.get_options().comment_str)
        };
    case GENERAL_DATA_FILE_EXTENSION:
        return {
            _a_pc.space(_a_pc.colon(_a_pc.general_data_file_extension_used())),
            _a_pc.quote(_a_element.get_options().general_data_extension)
        };
    case N_INTEGERS_USED_FOR_CREATION_OF_RNG:
        return {
            _a_pc.space(
                _a_pc.colon(_a_pc.n_integers_used_for_creation_of_rng_used())
            ),
            _a_pc.print_integer(
                _a_element.get_options()
                    .number_of_integers_used_to_seed_random_generators
            )
        };
    case REPETITION_CONFIG_USED:
        return {
            _a_pc.space(_a_pc.colon(_a_pc.repetition_config_used())),
            _a_pc.show_repetition_config(
                _a_element.get_options()
                    .map_of_unique_ids_and_for_loop_stack_tries
            )
        };
    case GLOBAL_SEED:

        return {
            _a_pc.space(_a_pc.colon(_a_pc.global_seed_used())),
            _a_pc.print_global_seed(
                _a_element.get_options().global_seed, global::get_global_seed()
            )
        };
    case FORCE_RUN_ALL_TESTS:
        return {
            _a_pc.space(_a_pc.question(_a_pc.force_run_all_tests_used())),
            _a_pc.passed(_a_element.get_options().force_run_all_tests)
        };
    case TEST_PATHS_TO_RUN:
        return {
            _a_pc.space(_a_pc.colon(_a_pc.test_paths_to_run_used())),
            _a_pc.show_test_paths_to_run(
                _a_element.get_options().test_paths_to_run
            )
        };
    default:
        throw errors::unaccounted_for_enum_exception(_a_fid);
    }
}

__no_constexpr_or_inline_imp
    std::pair<std::u8string, std::vector<std::u8string>>
    pre_test_set_data_list_formatter::get_data(
        const enum_pre_test_set_data_fields_t& _a_fid,
        const ds::pre_test_run_report_t&       _a_element,
        const print_config_t&                  _a_pc
    ) const
{
    using namespace std;
    using enum enum_pre_test_set_data_fields_t;

    const pair<u8string, u8string> _l_pair{get_output(_a_fid, _a_element, _a_pc)
    };
    pair<u8string, vector<u8string>> _l_rv{
        _l_pair.first, vector<u8string>(1, _l_pair.second)
    };
    const optional<u8string> _l_explanation_data{
        get_explanation_data(_a_fid, _a_element, _a_pc)
    };
    if (_l_explanation_data.has_value())
    {
        _l_rv.second.push_back(_l_explanation_data.value());
    }
    const optional<u8string> _l_cli_info{get_cli_info(_a_fid, _a_element, _a_pc)
    };
    if (_l_cli_info.has_value())
    {
        _l_rv.second.push_back(_l_cli_info.value());
    }
    return _l_rv;
}

_END_ABC_REPORTERS_NS