#pragma once

#include "abc_test/internal/ds/data_generator_memoization/map_unique_id_to_tdg_collection_stack_trie.hpp"
#include "abc_test/internal/ds/type_synonyms.hpp"
#include "abc_test/internal/global.hpp"
#include "abc_test/internal/reporters/error_reporter_controller.hpp"
#include "abc_test/internal/reporters/test_reporter_controller.hpp"
#include "abc_test/internal/utility/internal/log.hpp"
#include "abc_test/internal/utility/parsers/default_parser.hpp"
#include "abc_test/internal/utility/rng.hpp"

#include <filesystem>
#include <fmt/color.h>
#include <fstream>
#include <map>

_BEGIN_ABC_NS


using rep_data_fields_t    = std::vector<std::pair<std::string, std::string>>;
using rep_data_file_info_t = std::tuple<
    bool,
    rep_data_fields_t,
    std::filesystem::path,
    std::size_t,
    std::size_t>;
/*!
 * @brief Strcutre which holds the basic options for the library.
 *
 * This is designed to be extended by the user.
 */
struct test_options_base_t
{
public:
    /*!
     * @brief Default constructor.
     */
    __no_constexpr
        test_options_base_t() noexcept
        = default;
    std::size_t autofile_size = 1000;
    std::string autofile_name  = "autofile";
    /*!
     * @brief Path delimiter used to separate tests into their components.
     *
     * Given the string "aa::bb::cc", this string would be split into "aa",
     * "bb", "cc".
     */
    _ABC_NS_DS::test_path_delimiter path_delimiter = "::";
    /*!
     * @brief The root folder. It is from this folder all relative paths in the
     * test library are navigated from.
     */
    std::filesystem::path root_path = std::filesystem::current_path();
    /*!
     * @brief The seed used for the random data structures.
     */
    _ABC_NS_UTILITY::global_seed_t global_seed = abc::utility::global_seed_t();
    /*!
     * @brief The number of elements used to seed the random generators.
     *
     * Each random generator used in each test is taken from the global random
     * generator in abc_test. This variable tells the system how many values to
     * take from the global random generator for a seed, when creating a new
     * random genrator.
     *
     * If rng_seed is set to nullopt, then this number of elements are taken
     * from the C random generator to create the seed for the global random
     * generator.
     */
    size_t number_of_integers_used_to_seed_random_generators = 100;
    /*!
     * @brief The file extension used for data written to and from gen_data_t
     * elements. This is specifically for those files where the reader and
     * writer uses fmt::format and scn::scan.
     */
    std::string general_data_extension                       = "gd";
    std::string tertiary_data_file_extension                 = "td";
    /*!
     * @brief The comment line string.
     *
     * In read data files, lines beginning with this string are identified as
     * comments and ignored.
     */
    std::string comment_str                                  = "#";
    /*!
     * @brief Turn on (or off) the ability for the system to write data to
     * files.
     */
    bool write_data_to_files                                 = true;
    /*!
     * @brief The number of threads the system has access to.
     *
     * If this is set to zero, it defaults to
     * std::thread::hardware_concurrency().
     */
    std::size_t threads = std::thread::hardware_concurrency();
    /*!
     * @brief Map from a test's discovery id to a trie containing for loop
     * stacks.
     *
     * These are essentially data used when repeating tests; these tries tell
     * the test which elements of a for loop to re-generate.
     */
    _ABC_NS_DS::map_unique_id_to_tdg_collection_stack_trie_t
        map_of_unique_ids_and_for_loop_stack_tries;
    /*!
     * @brief Denotes whether to attach the global test list to the interanl
     * test_list collection when validating.
     */
    bool use_global_test_list = true;
    bool show_configuration_explanations = false;
    /*!
     * @brief A collection of shared pointers to test_reporter_t elements; each
     * of these is owned by the system. They are deleted when this object is
     * deleted.
     *
     * We use shared ptrs so that this object can be moved around freely by the
     * user.
     */
    std::vector<std::shared_ptr<_ABC_NS_REPORTERS::test_reporter_t>>
        test_reporters;
    /*!
     * @brief A collectin of unique pointers to error_reporter_t elements. Each
     * of these is owned by the system. They are deleted when this object is
     * deleted.
     */
    std::vector<std::shared_ptr<_ABC_NS_REPORTERS::error_reporter_t>>
        error_reporters;
    /*!
     * @brief The collection of test lists.
     */
    std::vector<std::shared_ptr<_ABC_NS_DS::test_list_t>> test_lists;
    /*!
     * @brief If this is set to true ,the following happens:
     * - All tests are ran. If there is no data corresponding to a test in
     * map_of_unique_ids_and_for_loop_stack_tries then the test is ran with no
     * repetition data. If there is repetition data then it is used for that
     * test. If set to false:
     * - If there is any repetition data, then only those tests with repetition
     * data are ran.
     * - If there is no reptition dta, then all tests are ran.
     */
    bool force_run_all_tests = false;
    /*!
     * Test paths to run
     */
    std::vector<std::string> test_paths_to_run;
    std::string              autofile_metadata_string = "metadata";
    /*!
     * @brief Function to validate the input.
     *
     * Any errors are written to the optional string outout.
     *
     * @return Nullopt if there are no errors. Otherwise a string representation
     * of the error.
     */
    __no_constexpr std::optional<std::vector<std::string>>
                   validate_and_pre_process() noexcept;
protected:
    __no_constexpr void virtual validate_and_pre_process_(
        std::vector<std::string>& _a_error_ref
    ) noexcept;
    std::optional<std::filesystem::path> _m_file_to_write_to;
};

namespace
{
template <typename T>
requires requires (const T& _a_ptr) {
    { _a_ptr == nullptr } -> std::same_as<bool>;
}
__constexpr std::vector<std::size_t>
    get_indexes_of_nullptrs(const std::vector<T>& _a_index_of_ptrs) noexcept;
} // namespace

__no_constexpr std::string
               make_test_options_base_member_variables_fmt(
                   const test_options_base_t& _a_opts
               ) noexcept;
_END_ABC_NS

template <>
struct fmt::formatter<abc::test_options_base_t> : formatter<string_view>
{
    // parse is inherited from formatter<string_view>.
    // Can'tbe constexpr due to use of fmt::format
    __no_constexpr auto
        format(abc::test_options_base_t _a_rtd, format_context& _a_ctx) const
        -> format_context::iterator;
};

_BEGIN_ABC_NS
__no_constexpr_imp std::optional<std::vector<std::string>>
                   test_options_base_t::validate_and_pre_process() noexcept
{
    using namespace std;
    vector<string> _l_rv{};
    validate_and_pre_process_(_l_rv);
    if (_l_rv.size() == 0)
    {
        return optional<vector<string>>{};
    }
    else
    {
        return optional<vector<string>>{_l_rv};
    }
}

__no_constexpr_imp void
    test_options_base_t::validate_and_pre_process_(
        std::vector<std::string>& _a_error_ref
    ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_REPORTERS;
    vector<string> _l_rv;
    if (error_reporters.size() == 0)
    {
        _a_error_ref.push_back(fmt::format(
            "Error reporters must have atleast one element. Otherwise errors "
            "cannot be reported. "
            "If no others are available, "
            "included_instances/reporters/text_error_reporter.h contains an "
            "instance which prints errors to the console."
        ));
    }
    else
    {
        vector<size_t> _l_indexes_of_nullptrs{
            get_indexes_of_nullptrs(error_reporters)
        };
        if (_l_indexes_of_nullptrs.size() > 0)
        {
            _a_error_ref.push_back(fmt::format(
                "error_reporters has some elemnets which point to nullptr. "
                "Specifically those elements at the following indexes: {0}",
                _l_indexes_of_nullptrs
            ));
        }
    }
    if (test_reporters.size() == 0)
    {
        _a_error_ref.push_back(fmt::format(
            "test_reporters must have atleast one element. Otherwise tests "
            "cannot be reported. "
            "If no others are available, "
            "included_instances/reporters/text_test_reporter.h contains an "
            "instance which prints tests to the console."
        ));
    }
    else
    {
        vector<size_t> _l_indexes_of_nullptrs{
            get_indexes_of_nullptrs(error_reporters)
        };
        if (_l_indexes_of_nullptrs.size() > 0)
        {
            _a_error_ref.push_back(fmt::format(
                "test_reporters has some elemnets which point to nullptr. "
                "Specifically those elements at the following indexes: {0}",
                _l_indexes_of_nullptrs
            ));
        }
    }
    if (test_lists.size() == 0 && use_global_test_list == false)
    {
        _a_error_ref.push_back(fmt::format(
            "test_lists must have atleast one element or use_global_test_list "
            "must be set to true. use_global_test_list = {0}",
            use_global_test_list
        ));
    }
    else if (test_lists.size() > 0)
    {
        vector<size_t> _l_indexes_of_nullptrs{get_indexes_of_nullptrs(test_lists
        )};
        if (_l_indexes_of_nullptrs.size() > 0)
        {
            _a_error_ref.push_back(fmt::format(
                "test_lists has some elemnets which point to nullptr. "
                "Specifically those elements at the following indexes: {0}",
                _l_indexes_of_nullptrs
            ));
        }
    }

    if (map_of_unique_ids_and_for_loop_stack_tries.size() > 0
        && write_data_to_files)
    {
        _a_error_ref.push_back(fmt::format(
            "map_of_unique_ids_and_for_loop_stack_tries has elements in it - "
            "thus this run of the test suite corresponds to some test "
            "repetitions. However, we have set write_data_to_files to true. "
            "These two options are incompatable, as it will cause already "
            "written test data to be re-written to files."
        ));
    }
    if (threads == 0 || threads > std::thread::hardware_concurrency())
    {
        _a_error_ref.push_back(fmt::format(
            "threads = {0}. This value must be between 1 and the number of "
            "threads available on the system ({1}",
            threads,
            std::thread::hardware_concurrency()
        ));
    }
    if (comment_str == "" || comment_str == "\\n")
    {
        _a_error_ref.push_back(fmt::format(
            "comment_str = \"{0}\". It cannot be empty or equal to \"\\n\"",
            comment_str
        ));
    }
    if (general_data_extension == "" || general_data_extension.contains("\n")
        || general_data_extension.contains("."))
    {
        _a_error_ref.push_back(fmt::format(
            "Invalid general_data_extension ({0}). Cannot be empty or contain "
            "any of the following: {{\\n,'.'",
            general_data_extension
        ));
    }
    if (path_delimiter == "")
    {
        _a_error_ref.push_back(fmt::format(
            "Invalid path_delimiter ({0}). Cannot be empty", path_delimiter
        ));
    }
    if (not filesystem::is_directory(root_path))
    {
        _a_error_ref.push_back(
            fmt::format("Root folder \"{0}\" does not exist", root_path)
        );
    }
    /*if (automatic_file_line_index.has_value()
        && not automatic_repetition_data_folder.has_value())

    {
    }*/
    if (test_paths_to_run.size() == 0)
    {
        using namespace ds;
        test_paths_to_run = vector<string>(1, "");
    }
}

namespace
{
template <typename T>
requires requires (const T& _a_ptr) {
    { _a_ptr == nullptr } -> std::same_as<bool>;
}
__constexpr_imp std::vector<std::size_t>
                get_indexes_of_nullptrs(
                    const std::vector<T>& _a_index_of_ptrs
                ) noexcept
{
    using namespace std;
    vector<size_t> _l_indexes_of_nullptrs;
    for (size_t _l_idx{0}; const T& _l_ptr : _a_index_of_ptrs)
    {
        if (_l_ptr == nullptr)
        {
            _l_indexes_of_nullptrs.push_back(_l_idx);
        }
        ++_l_idx;
    }
    return _l_indexes_of_nullptrs;
}
} // namespace

__no_constexpr_imp std::string
                   make_test_options_base_member_variables_fmt(
                       const test_options_base_t& _a_opts
                   ) noexcept
{
    using namespace std;
    const string _l_rv{fmt::format(
        "{0} = {1}"
        ", {2} = {3}"
        ", {4} = {5}"
        ", {6} = {7}"
        ", {8} = {9}"
        ", {10} = {11}"
        ", {12} = {13}"
        ", {14} = {15}"
        ", {16} = {17}",
        // ", {19} = {20}"
        // ", {21} = {22}"
        // ", {23} = {24}",
        "root_path",
        _a_opts.root_path,
        "global_seed",
        _a_opts.global_seed,
        "number_of_integers_used_to_seed_random_generators",
        _a_opts.number_of_integers_used_to_seed_random_generators,
        "general_data_extension",
        _a_opts.general_data_extension,
        "comment_str",
        _a_opts.comment_str,
        "write_data_to_files",
        _a_opts.write_data_to_files,
        "threads",
        _a_opts.threads,
        "map_of_unique_ids_and_for_loop_stack_tries",
        _a_opts.map_of_unique_ids_and_for_loop_stack_tries,
        "use_global_test_list",
        _a_opts.use_global_test_list
        //"test_reporters", _a_rtd.test_reporters,
        // "error_reporters", _a_rtd.error_reporters,
        // "test_lists", _a_rtd.test_lists
    )};
    return _l_rv;
}

_END_ABC_NS

__no_constexpr_imp auto
    fmt::formatter<abc::test_options_base_t>::format(
        abc::test_options_base_t _a_rtd,
        format_context&          _a_ctx
    ) const -> format_context::iterator
{
    using namespace std;
    const string _l_rv{fmt::format(
        "{0}{{{1}}}",
        typeid(_a_rtd).name(),
        make_test_options_base_member_variables_fmt(_a_rtd)
    )};
    return formatter<string_view>::format(_l_rv, _a_ctx);
}