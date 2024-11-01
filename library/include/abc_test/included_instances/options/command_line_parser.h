#pragma once
#include "abc_test/external/CLI11/CLI11.hpp"
#include "abc_test/internal/options/test_options_base.h"
#include "abc_test/included_instances/options/included_instances_test_options.h"

_BEGIN_ABC_DS_NS
__no_constexpr bool
    lexical_cast(
        const std::string&                            _a_input,
        map_unique_id_to_tdg_collection_stack_trie_t& _a_param
    );
_END_ABC_DS_NS
_BEGIN_ABC_UTILITY_NS
__no_constexpr bool
lexical_cast(
    const std::string& _a_input,
    global_seed_t& _a_param
);
_END_ABC_UTILITY_NS
_BEGIN_ABC_NS

__no_constexpr_imp CLI::App*
                   build_command_line_parser(
                       included_instances_test_options_t& _a_opts
                   )
{
    using namespace CLI;
    using namespace abc;
    using namespace std;
    App*   _l_app_ptr = new App();
    App&   _l_app{*_l_app_ptr};
    string _l_repetition_str;
    _a_opts.map_of_unique_ids_and_for_loop_stack_tries;
    _l_app.add_flag(
        "-g,--use_global_test_list",
        _a_opts.use_global_test_list,
        "Use the global test list as the set of tests"
    );
    _l_app.add_flag(
        "-w,--write_data_to_files",
        _a_opts.write_data_to_files,
        "Denotes that data can be written to files"
    );
    _l_app.add_option("--path_delimiter", _a_opts.path_delimiter, "Path delimiter used");
    _l_app.add_option("-r,--root_path", _a_opts.root_path, "root path used");
    _l_app.add_option(
        "-t,--threads",
        _a_opts.threads,
        "Number of threads allocated to the testing library"
    );
    _l_app.add_option(
        "--comment",
        _a_opts.comment_str,
        "The string used at the start of files to signify a comment"
    );
    _l_app.add_option(
        "--general_data_file_extension",
        _a_opts.general_data_extension,
        "The extension used for general data files"
    );
    _l_app.add_option(
        "--tertiary_data_file_extension",
        _a_opts.tertiary_data_file_extension,
        "The extension used for tertiary data files"
    );
    _l_app.add_option(
        "--n_integers_used_for_rng_generation",
        _a_opts.number_of_integers_used_to_seed_random_generators,
        "Number of integers used to seed each individual random generator from "
        "the global random generator"
    );
    _l_app.add_option(
        "--repetition_config",
        _a_opts.map_of_unique_ids_and_for_loop_stack_tries,
        "Map of repetition trees"
    );
    _l_app.add_option(
        "--global_seed",
        _a_opts.global_seed,
        "Set the global seed"
    );
    _l_app.set_config("--config");

    return _l_app_ptr;
}

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
    global_seed_t& _a_param
)
{
    // Annoyingly we can't parse and validate at the same time (or atleast I've
// not been able to find a way to).
    using namespace _ABC_NS_DS;
    std::expected<complete_global_seed_t, std::string> _l_parse_result{ abc::utility::parse_complete_global_string_in_hex(_a_input) };
    if (_l_parse_result.has_value())
    {
        _a_param = _l_parse_result.value();
        return true;
    }
    return true;
}
_END_ABC_UTILITY_NS