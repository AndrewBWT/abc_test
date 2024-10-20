#pragma once

#include "abc_test/internal/ds/data_generator_memoization/typeless_data_generator_collection_stack_trie.h"
#include "abc_test/internal/ds/type_synonyms.h"
#include "abc_test/internal/global.h"
#include "abc_test/internal/reporters/error_reporter_controller.h"
#include "abc_test/internal/reporters/test_reporter_controller.h"
#include "abc_test/internal/utility/internal/log.h"
#include "abc_test/internal/utility/rng.h"
#include "fmt/color.h"

#include <filesystem>
#include <map>

_BEGIN_ABC_NS

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
    /*!
     * @brief Path delimiter used to separate tests into their components.
     *
     * Given the string "aa::bb::cc", this string would be split into "aa",
     * "bb", "cc".
     */
    _ABC_NS_DS::test_path_delimiter path_delimiter  = "::";
    /*!
     * @brief The root folder. It is from this folder all relative paths in the
     * test library are navigated from.
     */
    std::filesystem::path root_path         = std::filesystem::current_path();
    /*!
     * @brief The seed used for the random data structures.
     */
    std::optional<_ABC_NS_UTILITY::seed_t> rng_seed = {};
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
    std::string general_data_extension                       = "dg";
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
    std::map<size_t, _ABC_NS_DS::tdg_collection_stack_trie_t>
        map_of_unique_ids_and_for_loop_stack_tries;
    /*!
     * @brief Denotes whether to attach the global test list to the interanl
     * test_list collection when validating.
     */
    bool use_global_test_list = true;
    /*!
     * @brief A collection of shared pointers to test_reporter_t elements; each
     * of these is owned by the system. They are deleted when this object is
     * deleted.
     *
     * We use shared ptrs so that this object can be moved around freely by the
     * user.
     */
    std::vector<std::shared_ptr<_ABC_NS_REPORTERS::test_reporter_t>> test_reporters;
    /*!
     * @brief A collectin of unique pointers to error_reporter_t elements. Each
     * of these is owned by the system. They are deleted when this object is
     * deleted.
     */
    std::vector<std::shared_ptr<_ABC_NS_REPORTERS::error_reporter_t>> error_reporters;
    /*!
     * @brief The collection of test lists.
     */
    std::vector<std::shared_ptr<_ABC_NS_DS::test_list_t>> test_lists;
    /*!
     * @brief Function to validate the input.
     *
     * Any errors are written to the optional string outout.
     *
     * @return Nullopt if there are no errors. Otherwise a string representation
     * of the error.
     */
    __no_constexpr virtual std::optional<std::vector<std::string>>
        validate() const noexcept;
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
__no_constexpr_imp

    std::optional<std::vector<std::string>>
    test_options_base_t::validate() const noexcept
{
    using namespace std;
    using namespace _ABC_NS_REPORTERS;
    vector<string> _l_rv;
    if (error_reporters.size() == 0)
    {
        _l_rv.push_back(fmt::format(
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
            _l_rv.push_back(fmt::format(
                "error_reporters has some elemnets which point to nullptr. "
                "Specifically those elements at the following indexes: {0}",
                _l_indexes_of_nullptrs
            ));
        }
    }
    if (test_reporters.size() == 0)
    {
        _l_rv.push_back(fmt::format(
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
            _l_rv.push_back(fmt::format(
                "test_reporters has some elemnets which point to nullptr. "
                "Specifically those elements at the following indexes: {0}",
                _l_indexes_of_nullptrs
            ));
        }
    }
    if (test_lists.size() == 0 && use_global_test_list == false)
    {
        _l_rv.push_back(fmt::format(
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
            _l_rv.push_back(fmt::format(
                "test_lists has some elemnets which point to nullptr. "
                "Specifically those elements at the following indexes: {0}",
                _l_indexes_of_nullptrs
            ));
        }
    }

    if (map_of_unique_ids_and_for_loop_stack_tries.size() > 0
        && write_data_to_files)
    {
        _l_rv.push_back(fmt::format(
            "map_of_unique_ids_and_for_loop_stack_tries has elements in it - "
            "thus this run of the test suite corresponds to some test "
            "repetitions. However, we have set write_data_to_files to true. "
            "These two options are incompatable."
        ));
    }
    if (threads == 0 || threads > std::thread::hardware_concurrency())
    {
        _l_rv.push_back(fmt::format(
            "threads = {0}. This value must be between 1 and the number of "
            "threads available on the system ({1}",
            threads,
            std::thread::hardware_concurrency()
        ));
    }
    if (comment_str == "" || comment_str == "\\n")
    {
        _l_rv.push_back(fmt::format(
            "comment_str = \"{0}\". It cannot be empty or equal to \"\\n\"",
            comment_str
        ));
    }
    if (general_data_extension == "" || general_data_extension.contains("\n")
        || general_data_extension.contains("."))
    {
        _l_rv.push_back(fmt::format(
            "Invalid general_data_extension ({0}). Cannot be empty or contain "
            "any of the following: {{\\n,'.'",
            general_data_extension
        ));
    }
    if (path_delimiter == "")
    {
        _l_rv.push_back(fmt::format(
            "Invalid path_delimiter ({0}). Cannot be empty", path_delimiter
        ));
    }

    if (not filesystem::is_directory(root_path))
    {
        _l_rv.push_back(
            fmt::format("Root folder \"{0}\" does not exist", root_path)
        );
    }

    if (_l_rv.size() == 0)
    {
        return optional<vector<string>>{};
    }
    else
    {
        return optional<vector<string>>{_l_rv};
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

_END_ABC_NS

__no_constexpr_imp auto
    fmt::formatter<abc::test_options_base_t>::format(
        abc::test_options_base_t _a_rtd,
        format_context&          _a_ctx
    ) const -> format_context::iterator
{
    using namespace std;
    const string _l_rv{fmt::format(
        "{0} "
        "{{{1} = {2}"
        ", {3} = {4}"
        ", {5} = {6}"
        ", {7} = {8}"
        ", {9} = {10}"
        ", {11} = {12}"
        ", {13} = {14}"
        ", {15} = {16}"
        ", {17} = {18}"
        // ", {19} = {20}"
        // ", {21} = {22}"
        // ", {23} = {24}"
        "}}",
        typeid(_a_rtd).name(),
        "root_path",
        _a_rtd.root_path,
        "rng_seed",
        _a_rtd.rng_seed,
        "number_of_integers_used_to_seed_random_generators",
        _a_rtd.number_of_integers_used_to_seed_random_generators,
        "general_data_extension",
        _a_rtd.general_data_extension,
        "comment_str",
        _a_rtd.comment_str,
        "write_data_to_files",
        _a_rtd.write_data_to_files,
        "threads",
        _a_rtd.threads,
        "map_of_unique_ids_and_for_loop_stack_tries",
        _a_rtd.map_of_unique_ids_and_for_loop_stack_tries,
        "use_global_test_list",
        _a_rtd.use_global_test_list
        //"test_reporters", _a_rtd.test_reporters,
        // "error_reporters", _a_rtd.error_reporters,
        // "test_lists", _a_rtd.test_lists
    )};
    return formatter<string_view>::format(_l_rv, _a_ctx);
}