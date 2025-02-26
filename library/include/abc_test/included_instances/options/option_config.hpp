#pragma once
#include "abc_test/internal/utility/internal/macros.hpp"

#include <variant>

_BEGIN_ABC_NS

namespace detail
{
using option_config_t = std::
    tuple<std::string_view, std::string_view, std::optional<std::string_view>>;
static constexpr const option_config_t _s_show_configuration_explanations
    = {"show_configuration_explanations",
       "Provides an explanation to the user how an option will change the test "
       "configuration",
       {}};
static constexpr const option_config_t _s_global_test_list
    = {"use_global_test_list",
       "Use the global test list as the set of tests",
       {"g"}};
static constexpr option_config_t _s_write_data_to_files
    = {"write_data_to_files", "Denotes that data can be written to files", {"w"}
};
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
static constexpr option_config_t _s_integers_used_for_rng_generation
    = {"n_integers_used_for_rng_generation",
       "Number of integers used to seed each individual random generator from "
       "the global random generator",
       {}};
static constexpr option_config_t _s_repetition_config
    = {"repetition_config", "Map of repetition trees", {}};
static constexpr option_config_t _s_global_seed
    = {"global_seed", "Set the global seed", {}};
static constexpr option_config_t _s_force_run_all_tests
    = {"force_run_all_tests", "Forces all tests to run", {}};
static constexpr option_config_t _c_test_paths_to_run
    = {"test_paths_to_run", "Which test paths are to be ran. ", {}};
} // namespace detail

_END_ABC_NS