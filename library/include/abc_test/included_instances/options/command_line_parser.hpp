#pragma once
#include "abc_test/core/options/test_options_base.hpp"
#include "abc_test/included_instances/options/included_instances_test_options.hpp"
#include "abc_test/included_instances/options/option_config.hpp"
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
    included_instances_test_options_t                  _l_iito;
    cli_results_t                                      _l_cli_results;
    result_t<cli_t<included_instances_test_options_t>> _l_cli_res{
        make_cli<included_instances_test_options_t>()
    };
    if (_l_cli_res.has_value())
    {
        cli_t<included_instances_test_options_t>& _l_cli{_l_cli_res.value()};
        _ABC_NS::detail::add_all_options(_l_iito, _l_cli, _l_cli_results);
        _l_cli.parse_arguments(_l_iito, argc, argv, _l_cli_results);
        if (_l_cli_results.has_errors())
        {
            std::cout
                << "Errors encountered when parsing command line parameters. "
                   "These are as follows:"
                << std::endl;
            const u8string _l_errors{_l_cli_results.errors()};
            std::cout << "\t" << string(_l_errors.begin(), _l_errors.end());
            return -1;
        }
        if (_l_cli_results.has_warnings())
        {
        }
        if (_l_cli_results.show_log())
        {
            _l_cli_results.log_msg();
            return -1;
        }
        if (_l_cli_results.has_text_output())
        {
            for (auto& k : _l_cli_results.text_output())
            {
                string _l_str{k.begin(), k.end()};
                std::cout << _l_str << std::endl;
            }
            return -1;
        }
        if (_l_cli_results.can_continue())
        {
            ds::pre_test_run_report_t _l_ptrr(
                _l_cli_results.memoized_data(), _l_iito
            );
            auto _l_validated_test_options{
                validated_test_options_t<included_instances_test_options_t>::
                    validate_test_options(_l_iito)
            };
            if (_l_validated_test_options.has_value())
            {
                test_main_t _l_test_main(
                    _l_validated_test_options.value(), _l_cli
                );
                _l_test_main.run_tests(_l_ptrr);
                return 0;
            }
            else
            {
                std::u8string _l_st;
                for (size_t _l_idx{0};
                     u8string & _l_error : _l_validated_test_options.error())
                {
                    _l_st.append(fmt::format(u8" {0})  ", ++_l_idx));
                    _l_st.append(_l_error);
                    _l_st.append(u8"\n");
                }
                const u8string _l_rv{fmt::format(
                    u8"Error(s) encountered when validating test_options_t. "
                    u8"The following errors were returned from the validation "
                    u8"function:\n{0}\nThe program will now terminate. "
                    u8"included_instances_test_options_t = {1}",
                    _l_st,
                    cast_string_to_u8string(fmt::format("{}", _l_iito))
                )};
                std::cout << string(_l_rv.begin(), _l_rv.end()) << std::endl;
                return -1;
            }
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
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
    _a_cli.add_option(
        _s_show_configuration_explanations,
        static_cast<bool included_instances_test_options_t::*>(
            &test_options_base_t::show_configuration_explanations
        ),
        _a_cli_results
    );
    _a_cli.add_option(
        _s_global_test_list,
        static_cast<bool included_instances_test_options_t::*>(
            &test_options_base_t::use_global_test_list
        ),
        _a_cli_results
    );
    _a_cli.add_option(
        _s_write_data_to_files,
        static_cast<bool included_instances_test_options_t::*>(
            &test_options_base_t::write_data_to_files
        ),
        _a_cli_results
    );
    _a_cli.add_option(
        _s_path_delimiter,
        static_cast<_ABC_NS_DS::test_path_delimiter
                        included_instances_test_options_t::*>(
            &included_instances_test_options_t::path_delimiter
        ),
        _a_cli_results
    );
    _a_cli.add_option(
        _s_root_path,
        static_cast<std::filesystem::path included_instances_test_options_t::*>(
            &included_instances_test_options_t::root_path
        ),
        _a_cli_results
    );
    _a_cli.add_option(
        _s_threads,
        static_cast<std::size_t included_instances_test_options_t::*>(
            &included_instances_test_options_t::threads
        ),
        _a_cli_results
    );
    _a_cli.add_option(
        _s_comment_str,
        static_cast<std::u8string included_instances_test_options_t::*>(
            &included_instances_test_options_t::comment_str
        ),
        _a_cli_results
    );
    _a_cli.add_option(
        _s_general_data_file_extension,
        static_cast<std::u8string included_instances_test_options_t::*>(
            &included_instances_test_options_t::general_data_extension
        ),
        _a_cli_results
    );
    _a_cli.add_option(
        _s_tertiary_data_file_extension,
        static_cast<std::u8string included_instances_test_options_t::*>(
            &included_instances_test_options_t::tertiary_data_file_extension
        ),
        _a_cli_results
    );
    _a_cli.add_option(
        _s_retain_passed_assertions,
        static_cast<bool included_instances_test_options_t::*>(
            &test_options_base_t::retain_passed_assertions
        ),
        _a_cli_results
    );
    _a_cli.add_option(
        _s_integers_used_for_rng_generation,
        static_cast<size_t included_instances_test_options_t::*>(
            &test_options_base_t::
                number_of_integers_used_to_seed_random_generators
        ),
        _a_cli_results
    );
    _a_cli.add_option<
        _ABC_NS_DS::map_unique_id_to_tdg_collection_stack_trie_t,
        u8string>(
        _s_repetition_config,
        static_cast<_ABC_NS_DS::map_unique_id_to_tdg_collection_stack_trie_t
                        included_instances_test_options_t::*>(
            &test_options_base_t::map_of_unique_ids_and_for_loop_stack_tries
        ),
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
        static_cast<_ABC_NS_UTILITY::global_seed_t
                        included_instances_test_options_t::*>(
            &test_options_base_t::global_seed
        ),
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
    _a_cli.add_option(
        _s_force_run_all_tests,
        static_cast<bool included_instances_test_options_t::*>(
            &test_options_base_t::force_run_all_tests
        ),
        _a_cli_results
    );
    _a_cli.add_option(
        _s_autofile_name,
        static_cast<u8string included_instances_test_options_t::*>(
            &test_options_base_t::autofile_name
        ),
        _a_cli_results
    );
    _a_cli.add_option(
        _s_autofile_size,
        static_cast<size_t included_instances_test_options_t::*>(
            &test_options_base_t::autofile_size
        ),
        _a_cli_results
    );
    _a_cli.add_option(
        _s_repetition_file_metadata_string,
        static_cast<u8string included_instances_test_options_t::*>(
            &test_options_base_t::autofile_metadata_string
        ),
        _a_cli_results
    );
    _a_cli.add_option(
        _s_tertiary_data_file_extension,
        static_cast<u8string included_instances_test_options_t::*>(
            &test_options_base_t::tertiary_data_file_extension
        ),
        _a_cli_results
    );
    _a_cli.add_auto_configuration();
    _a_cli.add_option(
        _s_default_cout_text_test_reporter,
        static_cast<console_output_e included_instances_test_options_t::*>(
            &included_instances_test_options_t::use_text_test_reporter_to_cout
        ),
        _a_cli_results
    );
    _a_cli.add_multi_element_option<vector<u8string>, u8string>(
        _c_test_paths_to_run,
        static_cast<vector<u8string> included_instances_test_options_t::*>(
            &test_options_base_t::test_paths_to_run
        ),
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