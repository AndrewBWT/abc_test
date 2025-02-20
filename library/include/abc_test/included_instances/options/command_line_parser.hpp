#pragma once
#include "abc_test/external/CLI11/CLI11.hpp"
#include "abc_test/included_instances/options/included_instances_test_options.hpp"
#include "abc_test/internal/options/test_options_base.hpp"
#include "abc_test/internal/utility/cli.hpp"

_BEGIN_ABC_DS_NS
__no_constexpr bool
    lexical_cast(
        const std::string&                            _a_input,
        map_unique_id_to_tdg_collection_stack_trie_t& _a_param
    );
_END_ABC_DS_NS
_BEGIN_ABC_UTILITY_NS
__no_constexpr bool
    lexical_cast(const std::string& _a_input, global_seed_t& _a_param);
_END_ABC_UTILITY_NS
_BEGIN_ABC_NS
__no_constexpr int
    run_test_suite_using_command_line_args(int argc, char** argv);

class abc_test_clp_app
{
public:
    using option_config_t = std::tuple<
        std::string_view,
        std::string_view,
        std::optional<std::string_view>>;
    __constexpr
    abc_test_clp_app() noexcept
        = delete;
    __no_constexpr
        abc_test_clp_app(
            included_instances_test_options_t& _a_opts,
            cli_t&                             _a_cli
        ) noexcept;
    __constexpr CLI::App&
                get_ref() noexcept;
    __no_constexpr void
        run_file_reader(
            const std::vector<std::pair<std::string, std::string>>& _a_strs,
            std::vector<std::string>&                               _a_errors
        ) noexcept;
    static constexpr const option_config_t _s_global_test_list
        = {"use_global_test_list",
           "Use the global test list as the set of tests",
           {"g"}};
    static constexpr option_config_t _s_write_data_to_files
        = {"write_data_to_files",
           "Denotes that data can be written to files",
           {"w"}};
    static constexpr option_config_t _s_path_delimiter
        = {"path_delimiter", "Path delimiter used", {}};
    static constexpr option_config_t _s_root_path
        = {"root_path", "Root path used", {}};
    static constexpr option_config_t _s_threads
        = {"threads", "Number of threads allocated to the library", {}};
    static constexpr option_config_t _s_comment_str
        = {"comment", "Comment used", {}};
    static constexpr option_config_t _s_general_data_file_extension
        = {"general_data_file_extension",
           "The extension used for general data files",
           {}};
    static constexpr option_config_t _s_tertiary_data_file_extension
        = {"tertiary_data_file_extension",
           "The extension used for tertiary data files",
           {}};
    static constexpr option_config_t _s_integers_used_for_rng_generation = {
        "n_integers_used_for_rng_generation",
        "Number of integers used to seed each individual random generator from "
        "the global random generator",
        {}
    };
    static constexpr option_config_t _s_repetition_config
        = {"repetition_config", "Map of repetition trees", {}};
    static constexpr option_config_t _s_global_seed
        = {"global_seed", "Set the global seed", {}};
    static constexpr option_config_t _s_force_run_all_tests
        = {"force_run_all_tests", "Forces all tests to run", {}};
    static constexpr option_config_t _s_automatic_repetition_data_folder = {
        "automatic_repetition_data_folder",
        "Denotes this run as automatically configured. Requires exactly "
        "three arguments; the first must be a folder location. The second "
        "is either a positive integer or the strings latest or "
        "latest_if_failure. The third is any of the strings always_write, "
        "write_if_different_or_new_run_that_failed or do_not_write. Using this "
        "option will "
        "read "
        "memoized configurations from the folder given. These are "
        "configurations indexed by integer, with an additional bool "
        "denoting whether the test was a success or failure. If the second "
        "argument is an integer, that configuration is found and loaded (if "
        "it exists). If it is the string \"latest\", it will run the most "
        "recent configuration. If it is the string \"latest_if_failure\", it "
        "will only load the configuration if the previous test was a failure. "
        "Otherwise it will will not load any configuration. The third argument "
        "denotes whether to write data after the test is run to the folder. If "
        "the string is \"always_write\", the data is always written. If the "
        "string is \"\", the data is only written if it was loaded and "
        "overridden by the user, or no configuration was automatically loaded "
        "and instead a clean run was performd, which failed. If the string is "
        "\"do_not_write\", the configuration is never written to any files. ",
        {}
    };
    static constexpr option_config_t _s_override_automatic_configuration
        = {"override_automatic_configuration",
           "Allows an automatic configuration to be overriden by user-provided "
           "values. ",
           {}};
    static constexpr option_config_t _c_automatically_rerun_last_test_if_failure
        = {"automaticly_rerun_last_test_if_failure",
           "Automatically runs previously ran configurations. ",
           {}};
    static constexpr option_config_t _c_test_paths_to_run
        = {"test_paths_to_run", "Which test paths are to be ran. ", {}};
    static constexpr std::string_view _s_automatic_config_option = "--config";
private:
    std::unique_ptr<CLI::App>     _m_internal_ptr;
    std::reference_wrapper<cli_t> _m_cli;
    std::map<
        std::string,
        std::function<std::optional<std::string>(const std::string_view)>>
        _m_map_strs_to_parsing_funcs;
    std::reference_wrapper<included_instances_test_options_t> _m_iito_wrapper;
    // Static Options
    std::map<std::string, CLI::Option*> _m_option_str_to_option_ptr;
    template <typename T, typename U = T>
    __constexpr void
        insert_option(const option_config_t& _a_option_config, T& _a_value)
            noexcept;
    template <typename T>
    __constexpr void
        insert_continuous_file_option(
            const option_config_t& _a_option_config,
            T&                     _a_value
        ) noexcept;
};

namespace
{
__constexpr std::string
    make_cml_string(const abc_test_clp_app::option_config_t& _a_option_config_t
    ) noexcept;
} // namespace

_END_ABC_NS

_BEGIN_ABC_DS_NS
__no_constexpr_imp bool
    lexical_cast(
        const std::string&                            _a_input,
        map_unique_id_to_tdg_collection_stack_trie_t& _a_param
    )
{
    // Annoyingly we can't parse and validate at the same time (or atleast I've
    // not been able to find a way to).
    using namespace _ABC_NS_DS;
    const parse_map_unique_id_to_tdg_collection_stack_trie_result_t _l_result{
        parse_compressed_map_of_unique_ids_to_tdg_collection_stack_tries(
            _a_input
        )
    };
    if (_l_result.has_value())
    {
        _a_param = _l_result.value();
    }
    return _l_result.has_value();
}

_END_ABC_DS_NS

_BEGIN_ABC_UTILITY_NS
__no_constexpr_imp bool
    lexical_cast(
        const std::string& _a_input,
        global_seed_t&     _a_param
    )
{
    // Annoyingly we can't parse and validate at the same time (or atleast I've
    // not been able to find a way to).
    using namespace _ABC_NS_DS;
    std::expected<complete_global_seed_t, std::string> _l_parse_result{
        abc::utility::parse_complete_global_string_in_hex(_a_input)
    };
    if (_l_parse_result.has_value())
    {
        _a_param = _l_parse_result.value();
        return true;
    }
    return true;
}

_END_ABC_UTILITY_NS

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
    std::cout << "abc_test run started." << std::endl;
    included_instances_test_options_t _l_iito;
    cli_output_t                      _l_cli_output;
    cli_t                             _l_cli(_l_cli_output);
    abc_test_clp_app                  _l_clp_app(_l_iito, _l_cli);
    std::cout << "Parsing command line parameters." << std::endl;
    _l_cli.parse_arguments(argc, argv);
    if (_l_cli_output.has_errors())
    {
        std::cout << "Errors encountered when parsing command line parameters. "
                     "These are as follows:"
                  << std::endl;
        std::cout << "\t" << _l_cli_output.errors();
    }
    if (_l_cli_output.has_warnings())
    {
    }
    if (_l_cli_output.show_log())
    {
    }
    if (_l_cli_output.has_text_output())
    {
        for (auto& k : _l_cli_output.text_output())
        {
            std::cout << k << std::endl;
        }
    }
    if (_l_cli_output.can_continue())
    {
        auto _l_validated_test_options{validated_test_options_t<
            included_instances_test_options_t>::validate_test_options(_l_iito)};
        if (_l_validated_test_options.has_value())
        {
            test_main_t _l_test_main(_l_validated_test_options.value());
            _l_test_main.run_tests();
            return 0;
        }
        else
        {
            str::string_table_t _l_st({1});
            for (size_t _l_idx{0};
                 string & _l_error : _l_validated_test_options.error())
            {
                _l_st.push_back(fmt::format(" {0})  ", _l_idx++));
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

__no_constexpr_imp
    abc_test_clp_app::abc_test_clp_app(
        included_instances_test_options_t& _a_opts,
        cli_t&                             _a_cli
    ) noexcept
    : _m_internal_ptr(std::make_unique<CLI::App>())
    , _m_iito_wrapper(_a_opts)
    , _m_cli(std::reference_wrapper(_a_cli))
{
    using namespace std;
    _m_cli.get().add_help_flag();
    _m_cli.get().add_file_config_flag();
    insert_option(_s_global_test_list, _a_opts.use_global_test_list);
    insert_option(_s_write_data_to_files, _a_opts.write_data_to_files);
    insert_option(_s_path_delimiter, _a_opts.path_delimiter);
    insert_option(_s_root_path, _a_opts.root_path);
    insert_option(_s_threads, _a_opts.threads);
    insert_option(_s_comment_str, _a_opts.comment_str);
    insert_option(
        _s_general_data_file_extension, _a_opts.general_data_extension
    );
    insert_option(
        _s_tertiary_data_file_extension, _a_opts.tertiary_data_file_extension
    );
    insert_option(
        _s_integers_used_for_rng_generation,
        _a_opts.number_of_integers_used_to_seed_random_generators
    );
    insert_option(
        _s_repetition_config, _a_opts.map_of_unique_ids_and_for_loop_stack_tries
    );
    insert_option(_s_global_seed, _a_opts.global_seed);

    insert_option(_s_force_run_all_tests, _a_opts.force_run_all_tests);
    _m_cli.get().add_auto_configuration();
    /*insert_option<std::optional<std::string>, std::string>(
        _s_automatic_repetition_data_folder,
        _a_opts.automatic_repetition_data_folder
    );
    insert_option(
        _s_override_automatic_configuration,
        _a_opts.override_automatic_configuration
    );
    insert_option(
        _c_automatically_rerun_last_test_if_failure,
        _a_opts.automatic_repeating_of_previous_test_if_failure
    );*/

    // insert_option<vector<std::string>, std::string>(
    // _c_test_paths_to_run, _a_opts.test_paths_to_run
    // );
    _m_cli.get().add_multi_element_option<vector<string>, string>(
        get<0>(_c_test_paths_to_run),
        get<1>(_c_test_paths_to_run),
        _a_opts.test_paths_to_run,
        get<2>(_c_test_paths_to_run).has_value()
            ? optional<char>(get<2>(_c_test_paths_to_run).value()[0])
            : optional<char>{}
    );

    _m_option_str_to_option_ptr.insert(
        {string(_s_automatic_config_option),
         _m_internal_ptr->set_config(string(_s_automatic_config_option))}
    );
    _m_internal_ptr->allow_config_extras(false);
    insert_continuous_file_option(
        _s_global_test_list, _a_opts.use_global_test_list
    );
    insert_continuous_file_option(_s_path_delimiter, _a_opts.path_delimiter);
    insert_continuous_file_option(_s_root_path, _a_opts.root_path);
    insert_continuous_file_option(_s_threads, _a_opts.threads);
    insert_continuous_file_option(_s_comment_str, _a_opts.comment_str);
    insert_continuous_file_option(
        _s_general_data_file_extension, _a_opts.general_data_extension
    );
    insert_continuous_file_option(
        _s_tertiary_data_file_extension, _a_opts.tertiary_data_file_extension
    );
    insert_continuous_file_option(
        _s_integers_used_for_rng_generation,
        _a_opts.number_of_integers_used_to_seed_random_generators
    );
    insert_continuous_file_option(
        _s_repetition_config, _a_opts.map_of_unique_ids_and_for_loop_stack_tries
    );
    insert_continuous_file_option(_s_global_seed, _a_opts.global_seed);

    insert_continuous_file_option(
        _s_force_run_all_tests, _a_opts.force_run_all_tests
    );
}

__constexpr_imp CLI::App&
                abc_test_clp_app::get_ref() noexcept
{
    return *_m_internal_ptr;
}

__no_constexpr_imp void
    abc_test_clp_app::run_file_reader(
        const std::vector<std::pair<std::string, std::string>>& _a_strs,
        std::vector<std::string>&                               _a_errors
    ) noexcept
{
    using namespace std;
    char** _l_args = new char*[_a_strs.size() * 2 + 1];


    for (const auto& [_l_field_name, _l_field_data] : _a_strs)
    {
        if (auto finder = _m_map_strs_to_parsing_funcs.find(_l_field_name);
            finder != _m_map_strs_to_parsing_funcs.end())
        {
            finder->second(_l_field_data);
        }
        else
        {
            _a_errors.push_back(fmt::format(
                "Could not find field \"{0}\" when parsing automatically "
                "generated configuration file.",
                _l_field_name
            ));
        }
    }
}

template <typename T, typename U>
__constexpr void
    abc_test_clp_app::insert_option(
        const abc_test_clp_app::option_config_t& _a_option_config,
        T&                                       _a_value
    ) noexcept
{
    using namespace std;
    _m_cli.get().add_option<T, U>(
        get<0>(_a_option_config),
        get<1>(_a_option_config),
        _a_value,
        get<2>(_a_option_config).has_value()
            ? optional<char>(get<2>(_a_option_config).value()[0])
            : optional<char>{}
    );
}

template <typename T>
__constexpr void
    abc_test_clp_app::insert_continuous_file_option(
        const abc_test_clp_app::option_config_t& _a_option_config,
        T&                                       _a_value
    ) noexcept
{
    /*using namespace std;
    _m_map_strs_to_parsing_funcs.insert(
        {string(get<0>(_a_option_config)),
         [&](const std::string_view _a_str) -> std::optional<std::string>
         {
             if (not _m_iito_wrapper.get().override_automatic_configuration
                 && _m_option_str_to_option_ptr
                            .at(string(get<0>(_a_option_config)))
                            ->count()
                        > 0)
             {
                 return fmt::format(
                     "Cannot override field member {0}, as it has already been "
                     "written by the user",
                     1
                 );
             }
             else
             {
                 auto _l_res = abc::utility::parser::parse<T>(_a_str);
                 if (_l_res.has_value())
                 {
                     _a_value = _l_res.value();
                     return optional<string>{};
                 }
                 else
                 {
                     return _l_res.error();
                 }
             }
         }}
    );*/
}

namespace
{
__constexpr_imp std::string
                make_cml_string(
                    const abc_test_clp_app::option_config_t& _a_option_config
                ) noexcept
{
    return (get<2>(_a_option_config).has_value())
               ? fmt::format(
                     "-{0},--{1}",
                     get<2>(_a_option_config).value(),
                     get<0>(_a_option_config)
                 )
               : fmt::format("--{0}", get<0>(_a_option_config));
}
} // namespace

_END_ABC_NS