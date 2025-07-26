#pragma once
#include "abc_test/core/options/test_options_base.hpp"
#include "abc_test/included_instances/options/included_instances_test_options.hpp"
#include "abc_test/included_instances/options/option_config.hpp"
#include "abc_test/included_instances/test_harness/simple_console_reporter.hpp"
#include "abc_test/utility/cli.hpp"

_BEGIN_ABC_NS
__no_constexpr int
    run_test_suite_using_command_line_args(int argc, char** argv);

namespace detail
{
__no_constexpr void
    add_all_options(
        included_instances_test_options_t&                             _a_opts,
        _ABC_NS_UTILITY_CLI::cli_t<included_instances_test_options_t>& _a_cli,
        _ABC_NS_UTILITY_CLI::cli_results_t& _a_cli_results
    ) noexcept;
} // namespace detail

_END_ABC_NS

_BEGIN_ABC_NS
__no_constexpr_imp int
    run_test_suite_using_command_line_args(
        int    argc,
        char** argv
    )
{
    using namespace std;
    using namespace _ABC_NS;
    using namespace _ABC_NS_REPORTERS;
    using namespace _ABC_NS_DS;
    using namespace _ABC_NS_UTILITY;
    using namespace _ABC_NS_UTILITY_CLI;
    using namespace _ABC_NS_UTILITY_STR;
    simple_console_reporter_t                          _l_sco;
    included_instances_test_options_t                  _l_iito;
    cli_results_t                                      _l_cli_results;
    result_t<cli_t<included_instances_test_options_t>> _l_cli_res{
        make_cli<included_instances_test_options_t>()
    };
    if (not _l_cli_res.has_value())
    {
        _l_sco.write_line(u8"Error encountered when setting up command line "
                          u8"interface object. Error is as follows:");
        _l_sco.write_line(_l_cli_res.error());
    }
    else
    {
        using enum cli_results_status_t;
        cli_t<included_instances_test_options_t>& _l_cli{_l_cli_res.value()};
        _ABC_NS::detail::add_all_options(_l_iito, _l_cli, _l_cli_results);
        _l_cli.parse_arguments(_l_iito, argc, argv, _l_cli_results);
        switch (_l_cli_results.status())
        {
        case has_errors:
            _l_sco.write_error_line(
                u8"Errors encountered when parsing command line parameters. "
                "These are as follows:"
            );
            _l_sco.write_error_line(_l_cli_results.errors());
            break;
        case has_text_output:
            _l_sco.write_lines(_l_cli_results.text_output());
            break;
        default:
            return run_tests<included_instances_test_options_t, true>(
                _l_iito, _l_sco, _l_cli, _l_cli_results.memoized_data()
            );
        }
    }
    return -1;
}

namespace detail
{
__no_constexpr_imp void
    add_all_options(
        included_instances_test_options_t&                             _a_opts,
        _ABC_NS_UTILITY_CLI::cli_t<included_instances_test_options_t>& _a_cli,
        _ABC_NS_UTILITY_CLI::cli_results_t& _a_cli_results
    ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_UTILITY_CLI;
    using namespace _ABC_NS_UTILITY_CLI::detail;
    _a_cli.add_help_flag();
    _a_cli.add_file_config_flag();
    _a_cli.add_option<bool>(
        _s_show_configuration_explanations,
        [](included_instances_test_options_t& _a_to) -> bool&
        {
            return _a_to.cli_test_options.show_configuration_explanations;
        },
        _a_cli_results
    );
    _a_cli.add_option<bool>(
        _s_global_test_list,
        [](included_instances_test_options_t& _a_to) -> bool&
        {
            return _a_to.glot_aware_test_options.use_global_test_list;
        },
        _a_cli_results
    );
    _a_cli.add_option<bool>(
        _s_write_data_to_files,
        [](included_instances_test_options_t& _a_to) -> bool&
        {
            return _a_to.individual_io_based_test_options.write_data_to_files;
        },
        _a_cli_results
    );
    _a_cli.add_option<u8string>(
        _s_path_delimiter,
        [](included_instances_test_options_t& _a_to) -> u8string&
        {
            return _a_to.group_test_options.path_delimiter;
        },
        _a_cli_results
    );
    _a_cli.add_option<filesystem::path>(
        _s_root_path,
        [](included_instances_test_options_t& _a_to) -> filesystem::path&
        {
            return _a_to.individual_io_based_test_options.root_path;
        },
        _a_cli_results
    );
    _a_cli.add_option<size_t>(
        _s_threads,
        [](included_instances_test_options_t& _a_to) -> size_t&
        {
            return _a_to.group_test_options.threads;
        },
        _a_cli_results
    );
    _a_cli.add_option<u8string>(
        _s_comment_str,
        [](included_instances_test_options_t& _a_to) -> u8string&
        {
            return _a_to.individual_io_based_test_options.comment_str;
        },
        _a_cli_results
    );
    _a_cli.add_option<u8string>(
        _s_general_data_file_extension,
        [](included_instances_test_options_t& _a_to) -> u8string&
        {
            return _a_to.individual_io_based_test_options
                .general_data_extension;
        },
        _a_cli_results
    );
    _a_cli.add_option<u8string>(
        _s_tertiary_data_file_extension,
        [](included_instances_test_options_t& _a_to) -> u8string&
        {
            return _a_to.individual_io_based_test_options
                .tertiary_data_file_extension;
        },
        _a_cli_results
    );
    _a_cli.add_option<bool>(
        _s_retain_passed_assertions,
        [](included_instances_test_options_t& _a_to) -> bool&
        {
            return _a_to.individual_test_options.retain_passed_assertions;
        },
        _a_cli_results
    );
    _a_cli.add_option<size_t>(
        _s_integers_used_for_rng_generation,
        [](included_instances_test_options_t& _a_to) -> size_t&
        {
            return _a_to.group_test_options
                .number_of_integers_used_to_seed_random_generators;
        },
        _a_cli_results
    );
    _a_cli.add_option<
        _ABC_NS_DS::map_unique_id_to_tdg_collection_stack_trie_t,
        u8string>(
        _s_repetition_config,
        [](included_instances_test_options_t& _a_to
        ) -> _ABC_NS_DS::map_unique_id_to_tdg_collection_stack_trie_t&
        {
            return _a_to.group_test_options
                .map_of_unique_ids_and_for_loop_stack_tries;
        },
        _a_cli_results,
        cli_argument_processing_info_t<
            _ABC_NS_DS::map_unique_id_to_tdg_collection_stack_trie_t,
            std::u8string>{
            .process_parsed_value_func =
                [](ds::map_unique_id_to_tdg_collection_stack_trie_t& _a_ref,
                   const u8string&                                   _a_str)
            {
                ds::parse_map_unique_id_to_tdg_collection_stack_trie_result_t
                    _l_res
                    = ds::
                        parse_compressed_map_of_unique_ids_to_tdg_collection_stack_tries(
                            _a_str
                        );
                if (_l_res.has_value())
                {
                    _a_ref = _l_res.value();
                    return optional<u8string>{};
                }
                else
                {
                    return make_optional(_l_res.error());
                }
            },
            .print_func =
                [](const ds::map_unique_id_to_tdg_collection_stack_trie_t&
                       _a_element)
            {
                return ds::
                    print_compressed_map_of_unique_ids_to_tdg_collection_stack_tries(
                        _a_element
                    );
            }
        }
    );
    using namespace abc::utility;
    _a_cli.add_option<utility::global_seed_t, utility::global_seed_t>(
        _s_global_seed,
        [](included_instances_test_options_t& _a_to) -> global_seed_t&
        {
            return _a_to.group_test_options.global_seed;
        },
        _a_cli_results,
        cli_argument_processing_info_t<
            utility::global_seed_t,
            utility::global_seed_t>{
            .parser_func = [](utility::parser::parser_input_t& _a_input
                           ) -> result_t<global_seed_t>
            {
                using namespace utility;
                using namespace abc::utility::parser;
                return default_parser_t<variant<unsigned int, seed_t>>{
                    variant_print_parse_e::no_indexes
                }
                    .run_parser(_a_input)
                    .transform(
                        [](const variant<unsigned int, seed_t>& _a_varint)
                        {
                            return global_seed_t(_a_varint);
                        }
                    );
            },
            .print_func =
                [](const utility::global_seed_t& _a_global_seed)
            {
                using namespace utility;
                using namespace abc::utility::printer;
                return _a_global_seed.has_value()
                           ? fmt::format(
                                 u8"{}",
                                 default_printer_t<complete_global_seed_t>{
                                     variant_print_parse_e::no_indexes
                                 }
                                     .run_printer(_a_global_seed.value())
                             )
                           : fmt::format(
                                 u8"Global seed not set to appropriate value"
                             );
            }
        }
    );
    _a_cli.add_option<bool>(
        _s_force_run_all_tests,
        [](included_instances_test_options_t& _a_to) -> bool&
        {
            return _a_to.group_test_options.force_run_all_tests;
        },
        _a_cli_results
    );
    _a_cli.add_option<u8string>(
        _s_autofile_name,
        [](included_instances_test_options_t& _a_to) -> u8string&
        {
            return _a_to.cli_test_options.autofile_name;
        },
        _a_cli_results
    );
    _a_cli.add_option<size_t>(
        _s_autofile_size,
        [](included_instances_test_options_t& _a_to) -> size_t&
        {
            return _a_to.cli_test_options.autofile_size;
        },
        _a_cli_results
    );
    _a_cli.add_option<u8string>(
        _s_repetition_file_metadata_string,
        [](included_instances_test_options_t& _a_to) -> u8string&
        {
            return _a_to.cli_test_options.autofile_metadata_string;
        },
        _a_cli_results
    );
    _a_cli.add_option<u8string>(
        _s_tertiary_data_file_extension,
        [](included_instances_test_options_t& _a_to) -> u8string&
        {
            return _a_to.individual_io_based_test_options
                .tertiary_data_file_extension;
        },
        _a_cli_results
    );
    _a_cli.add_auto_configuration();
    _a_cli.add_option<console_output_e>(
        _s_default_cout_text_test_reporter,
        [](included_instances_test_options_t& _a_to) -> console_output_e&
        {
            return _a_to.use_text_test_reporter_to_cout;
        },
        _a_cli_results
    );
    _a_cli.add_multi_element_option<vector<u8string>, u8string>(
        _c_test_paths_to_run,
        [](included_instances_test_options_t& _a_to) -> vector<u8string>&
        {
            return _a_to.group_test_options.test_paths_to_run;
        },
        _a_cli_results,
        cli_argument_processing_info_t<vector<u8string>, u8string>{
            .print_func =
                [](const vector<u8string>& _a_strs)
            {
                u8string _l_rv;
                for (size_t _l_idx{0}; const u8string_view _l_str : _a_strs)
                {
                    if (_l_str.empty())
                    {
                        continue;
                    }
                    _l_rv.append(_l_str);
                    if (_l_idx + 1 < _a_strs.size())
                    {
                        _l_rv.append(u8" ");
                    }
                    ++_l_idx;
                }
                return _l_rv;
            }
        }
    );
}
} // namespace detail

_END_ABC_NS