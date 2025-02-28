#pragma once
#include "abc_test/included_instances/options/included_instances_test_options.hpp"
#include "abc_test/included_instances/options/option_config.hpp"
#include "abc_test/internal/options/test_options_base.hpp"
#include "abc_test/internal/utility/cli.hpp"
#include "abc_test/internal/utility/printers/specializations/fundamental_types.hpp"
#include "abc_test/internal/utility/printers/specializations/stl_11.hpp"
#include "abc_test/internal/utility/printers/specializations/stl_98.hpp"

_BEGIN_ABC_NS
__no_constexpr int
    run_test_suite_using_command_line_args(int argc, char** argv);

namespace detail
{
__no_constexpr void
    add_all_options(
        included_instances_test_options_t&        _a_opts,
        cli_t<included_instances_test_options_t>& _a_cli,
        cli_results_t&                            _a_cli_results
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
    included_instances_test_options_t        _l_iito;
    cli_results_t                            _l_cli_results;
    cli_t<included_instances_test_options_t> _l_cli;
    detail::add_all_options(_l_iito, _l_cli, _l_cli_results);
    _l_cli.parse_arguments(_l_iito, argc, argv, _l_cli_results);
    if (_l_cli_results.has_errors())
    {
        std::cout << "Errors encountered when parsing command line parameters. "
                     "These are as follows:"
                  << std::endl;
        std::cout << "\t" << _l_cli_results.errors();
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
            std::cout << k << std::endl;
        }
        return -1;
    }
    if (_l_cli_results.can_continue())
    {
        ds::pre_test_run_report_t _l_ptrr(
            _l_cli_results.memoized_data(), _l_iito
        );
        auto _l_validated_test_options{validated_test_options_t<
            included_instances_test_options_t>::validate_test_options(_l_iito)};
        if (_l_validated_test_options.has_value())
        {
            test_main_t _l_test_main(_l_validated_test_options.value(), _l_cli);
            _l_test_main.run_tests(_l_ptrr);
            return 0;
        }
        else
        {
            str::string_table_t _l_st({1});
            for (size_t _l_idx{0};
                 string & _l_error : _l_validated_test_options.error())
            {
                _l_st.push_back(fmt::format(" {0})  ", ++_l_idx));
                _l_st.push_back(_l_error);
                _l_st.new_line();
            }
            std::cout << fmt::format(
                "Error(s) encountered when validating test_options_t. "
                "The following errors were returned from the validation "
                "function:\n{0}\nThe program will now terminate. "
                "included_instances_test_options_t = {1}",
                _l_st(),
                _l_iito
            );
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
        included_instances_test_options_t&        _a_opts,
        cli_t<included_instances_test_options_t>& _a_cli,
        cli_results_t&                            _a_cli_results
    ) noexcept
{
    using namespace std;
    _a_cli.add_help_flag();
    _a_cli.add_file_config_flag();
    _a_cli.add_option(
        _s_show_configuration_explanations,
        &included_instances_test_options_t::show_configuration_explanations,
        _a_cli_results
    );
    _a_cli.add_option(
        _s_global_test_list,
        &included_instances_test_options_t::use_global_test_list,
        _a_cli_results
    );
    _a_cli.add_option(
        _s_write_data_to_files,
        &included_instances_test_options_t::write_data_to_files,
        _a_cli_results
    );
    _a_cli.add_option(
        _s_path_delimiter,
        &included_instances_test_options_t::path_delimiter,
        _a_cli_results
    );
    _a_cli.add_option(
        _s_root_path,
        &included_instances_test_options_t::root_path,
        _a_cli_results
    );
    _a_cli.add_option(
        _s_threads,
        &included_instances_test_options_t::threads,
        _a_cli_results
    );
    _a_cli.add_option(
        _s_comment_str,
        &included_instances_test_options_t::comment_str,
        _a_cli_results
    );
    _a_cli.add_option(
        _s_general_data_file_extension,
        &included_instances_test_options_t::general_data_extension,
        _a_cli_results
    );
    _a_cli.add_option(
        _s_tertiary_data_file_extension,
        &included_instances_test_options_t::tertiary_data_file_extension,
        _a_cli_results
    );
    _a_cli.add_option(
        _s_integers_used_for_rng_generation,
        &included_instances_test_options_t::
            number_of_integers_used_to_seed_random_generators,
        _a_cli_results
    );
    _a_cli.add_option<
        _ABC_NS_DS::map_unique_id_to_tdg_collection_stack_trie_t,
        string>(
        _s_repetition_config,
        &included_instances_test_options_t::
            map_of_unique_ids_and_for_loop_stack_tries,
        _a_cli_results,
        detail::make_parser_func<string>(),
        [](ds::map_unique_id_to_tdg_collection_stack_trie_t& _a_ref,
           const string_view                                 _a_str)
        {
            auto _l_res = ds::
                parse_compressed_map_of_unique_ids_to_tdg_collection_stack_tries(
                    _a_str
                );
            if (_l_res.has_value())
            {
                _a_ref = _l_res.value();
                return true;
            }
            else
            {
                return false;
            }
        },
        [](const ds::map_unique_id_to_tdg_collection_stack_trie_t& _a_element)
        {
            return ds::
                print_compressed_map_of_unique_ids_to_tdg_collection_stack_tries(
                    _a_element
                );
        }
    );
    _a_cli.add_option<utility::global_seed_t, utility::global_seed_t>(
        _s_global_seed,
        &included_instances_test_options_t::global_seed,
        _a_cli_results,
        detail::make_parser_func<utility::global_seed_t>(),
        detail::process_value<utility::global_seed_t, utility::global_seed_t>(),
        [](const utility::global_seed_t& _a_global_seed)
        {
            return _a_global_seed.complete_global_seed()
                           .value()
                           .integer()
                           .has_value()
                       ? fmt::format(
                             "{0}",
                             _a_global_seed.complete_global_seed()
                                 .value()
                                 .integer()
                                 .value()
                         )
                       : fmt::format(
                             "{0}",
                             _a_global_seed.complete_global_seed()
                                 .value()
                                 .vector_of_integers()
                                 .value()
                         );
        }
    );
    _a_cli.add_option(
        _s_force_run_all_tests,
        &included_instances_test_options_t::force_run_all_tests,
        _a_cli_results
    );
    _a_cli.add_option(
        _s_autofile_name,
        &included_instances_test_options_t::autofile_name,
        _a_cli_results
    );
    _a_cli.add_option(
        _s_autofile_size,
        &included_instances_test_options_t::autofile_size,
        _a_cli_results
    );
    _a_cli.add_option(
        _s_repetition_file_metadata_string,
        &included_instances_test_options_t::autofile_metadata_string,
        _a_cli_results
    );
    _a_cli.add_option(
        _s_tertiary_data_file_extension,
        &included_instances_test_options_t::tertiary_data_file_extension,
        _a_cli_results
    );
    _a_cli.add_auto_configuration();
    _a_cli.add_multi_element_option<vector<string>, string>(
        _c_test_paths_to_run,
        &included_instances_test_options_t::test_paths_to_run,
        _a_cli_results,
        detail::make_parser_func<string>(),
        detail::process_value<vector<string>, string>(),
        [](const vector<string>& _a_strs)
        {
            string _l_rv;
            for (size_t _l_idx{0}; const string_view _l_str : _a_strs)
            {
                if (_l_str == "")
                {
                    continue;
                }
                _l_rv.append(_l_str);
                if (_l_idx + 1 < _a_strs.size())
                {
                    _l_rv.append(" ");
                }
                ++_l_idx;
            }
            return _l_rv;
        }
    );
}
} // namespace detail

_END_ABC_NS