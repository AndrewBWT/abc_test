#pragma once
#include "abc_test/utility/cli/cli_option_config.hpp"

_BEGIN_ABC_UTILITY_CLI_NS

namespace detail
{

static constexpr const cli_option_config_t _s_show_configuration_explanations
    = {u8"show_configuration_explanations",
       u8"Provides an explanation to the user how an option will change the "
       u8"test "
       u8"configuration",
       {}};
static constexpr const cli_option_config_t _s_global_test_list
    = {u8"use_global_test_list",
       u8"Use the global test list as the set of tests",
       {'g'}};
static constexpr cli_option_config_t _s_write_data_to_files
    = {u8"write_data_to_files",
       u8"Denotes that data can be written to files",
       {'w'}};
static constexpr cli_option_config_t _s_path_delimiter
    = {u8"path_delimiter", u8"Path delimiter used", {}};
static constexpr cli_option_config_t _s_repetition_file_metadata_string
    = {u8"repetition_file_metadata_string", u8"Path delimiter used", {}};
static constexpr cli_option_config_t _s_root_path
    = {u8"root_path", u8"Root path used", {}};
static constexpr cli_option_config_t _s_threads
    = {u8"threads", u8"Number of threads allocated to the library", {}};
static constexpr cli_option_config_t _s_autofile_name
    = {u8"autofile_name", u8"Auto file name", {}};
static constexpr cli_option_config_t _s_autofile_size
    = {u8"autofile_size", u8"Auto file size", {}};
static constexpr cli_option_config_t _s_comment_str
    = {u8"comment", u8"Comment used", {}};
static constexpr cli_option_config_t _s_general_data_file_extension
    = {u8"general_data_file_extension",
       u8"The extension used for general data files",
       {}};
static constexpr cli_option_config_t _s_tertiary_data_file_extension
    = {u8"tertiary_data_file_extension",
       u8"The extension used for tertiary data files",
       {}};
static constexpr cli_option_config_t _s_retain_passed_assertions
    = {u8"retain_passed_assertions",
       u8"Whether to retain passed assertions in test suite output",
       {}};
static constexpr cli_option_config_t _s_integers_used_for_rng_generation
    = {u8"n_integers_used_for_rng_generation",
       u8"Number of integers used to seed each individual random generator "
       u8"from "
       u8"the global random generator",
       {}};
static constexpr cli_option_config_t _s_repetition_config
    = {u8"repetition_config", u8"Map of repetition trees", {}};
static constexpr cli_option_config_t _s_global_seed
    = {u8"global_seed", u8"Set the global seed", {}};
static constexpr cli_option_config_t _s_force_run_all_tests
    = {u8"force_run_all_tests", u8"Forces all tests to run", {}};
static constexpr cli_option_config_t _c_test_paths_to_run
    = {u8"test_paths_to_run", u8"Which test paths are to be ran. ", {}};
} // namespace detail

_END_ABC_UTILITY_CLI_NS