#pragma once

#include "abc_test/core/ds/data_generator_memoization/map_unique_id_to_tdg_collection_stack_trie.hpp"
#include "abc_test/core/ds/type_synonyms.hpp"
#include "abc_test/core/global.hpp"
#include "abc_test/core/reporters/error_reporter_controller.hpp"
#include "abc_test/core/reporters/test_reporter_controller.hpp"
#include "abc_test/utility/internal/log.hpp"
#include "abc_test/utility/parsers/default_parser.hpp"
#include "abc_test/utility/rng.hpp"
#include "abc_test/utility/rng/inner_rng_mt19937_64.hpp"

#include <filesystem>
#include <fmt/color.h>
#include <fstream>
#include <map>
#include <vector>

_BEGIN_ABC_UTILITY_CLI_NS
template <typename Option_Class>
class cli_t;
_END_ABC_UTILITY_CLI_NS

_BEGIN_ABC_NS

struct cli_test_options_t
{
    std::size_t   autofile_size                   = 1'000;
    std::u8string autofile_name                   = u8"autofile";
    bool          show_configuration_explanations = false;
    std::u8string              autofile_metadata_string = u8"metadata";
};

struct group_test_options_t
{
    _ABC_NS_DS::test_path_delimiter path_delimiter = u8"::";
    _ABC_NS_UTILITY::global_seed_t  global_seed = abc::utility::global_seed_t();
    size_t      number_of_integers_used_to_seed_random_generators = 100;
    std::size_t threads = std::thread::hardware_concurrency();
    _ABC_NS_DS::map_unique_id_to_tdg_collection_stack_trie_t
        map_of_unique_ids_and_for_loop_stack_tries;
    std::vector<std::shared_ptr<_ABC_NS_REPORTERS::test_reporter_t>>
        test_reporters;
    std::vector<std::shared_ptr<_ABC_NS_REPORTERS::error_reporter_t>>
                                                          error_reporters;
    std::vector<std::u8string>                            test_paths_to_run;
    std::vector<std::shared_ptr<_ABC_NS_DS::test_list_t>> test_lists;
    bool force_run_all_tests = false;
    std::filesystem::path error_root_path = std::filesystem::current_path();
    std::filesystem::path reports_root_path = std::filesystem::current_path();
};

struct individual_test_options_t
{
    bool        retain_passed_assertions             = false;
    std::size_t maximum_individual_alloctable_memory = 2'147; // 483'648;
};

struct individual_io_based_test_options_t
{
    std::filesystem::path root_path = std::filesystem::current_path();
    std::u8string         general_data_extension       = u8"gd";
    std::u8string         tertiary_data_file_extension = u8"td";
    std::u8string         comment_str                  = u8"#";
    bool                  write_data_to_files          = true;
};

struct glot_aware_test_options_t
{
    bool use_global_test_list = true;
};

struct test_options_base_t
{
    cli_test_options_t                 cli_test_options;
    group_test_options_t               group_test_options;
    individual_test_options_t          individual_test_options;
    individual_io_based_test_options_t individual_io_based_test_options;
    glot_aware_test_options_t          glot_aware_test_options;
    __no_constexpr                     std::optional<std::vector<std::u8string>>
                                       validate() const noexcept;
    __no_constexpr virtual void
                       pre_validation_process() noexcept;
    __no_constexpr virtual void post_validation_process() noexcept;
    __no_constexpr_imp utility::rng_t
                       make_rng() const noexcept
    {
        return utility::rng_t::make_rng<utility::inner_rng_mt19937_64_t>(
            global::get_global_seed(),
            group_test_options.number_of_integers_used_to_seed_random_generators
        );
    }

    template <typename T>
    __constexpr std::size_t
                maximum_individual_allocatable_memory() const
    {
        if constexpr (sizeof(T) == 0)
        {
            __STATIC_ASSERT(T, "Cannot allocate to T, as T has sizeof 0");
            return 0;
        }
        else if (individual_test_options.maximum_individual_alloctable_memory
                 == 0)
        {
            throw abc::errors::test_library_exception_t(
                u8"Cannot determine allocation as size == 0"
            );
        }
        else
        {
            return individual_test_options.maximum_individual_alloctable_memory
                   / sizeof(T);
        }
    }
protected:
    __no_constexpr void virtual validate_(
        std::vector<std::u8string>& _a_error_ref
    ) const noexcept;
    std::optional<std::filesystem::path> _m_file_to_write_to;
};

#if 0
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
    std::size_t   autofile_size                    = 1'000;
    std::u8string autofile_name                    = u8"autofile";
    /*!
     * @brief Path delimiter used to separate tests into their components.
     *
     * Given the string "aa::bb::cc", this string would be split into "aa",
     * "bb", "cc".
     */
    _ABC_NS_DS::test_path_delimiter path_delimiter = u8"::";
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
    std::u8string general_data_extension                     = u8"gd";
    std::u8string tertiary_data_file_extension               = u8"td";
    /*!
     * @brief The comment line string.
     *
     * In read data files, lines beginning with this string are identified as
     * comments and ignored.
     */
    std::u8string comment_str                                = u8"#";
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
    bool use_global_test_list            = true;
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
    std::vector<std::u8string> test_paths_to_run;
    std::u8string              autofile_metadata_string = u8"metadata";
    std::size_t maximum_individual_alloctable_memory    = 2'147; // 483'648;
    bool        retain_passed_assertions                = false;
    //std::shared_ptr<_ABC_NS_CLI::cli_t< test_options_base_t>> _m_cli;
    /*!
     * @brief Function to validate the input.
     *
     * Any errors are written to the optional string outout.
     *
     * @return Nullopt if there are no errors. Otherwise a string representation
     * of the error.
     */
    __no_constexpr     std::optional<std::vector<std::u8string>>
                       validate() const noexcept;
    __no_constexpr virtual void pre_process() noexcept;
    __no_constexpr_imp utility::rng_t
                       make_rng() const noexcept
    {
        return utility::rng_t::make_rng<utility::inner_rng_mt19937_64_t>(
            global::get_global_seed(),
            number_of_integers_used_to_seed_random_generators
        );
    }

    template <typename T>
    __constexpr std::size_t
                maximum_individual_allocatable_memory() const
    {
        if constexpr (sizeof(T) == 0)
        {
            __STATIC_ASSERT(T, "Cannot allocate to T, as T has sizeof 0");
            return 0;
        }
        else if (maximum_individual_alloctable_memory == 0)
        {
            throw abc::errors::test_library_exception_t(
                u8"Cannot determine allocation as size == 0"
            );
        }
        else
        {
            return maximum_individual_alloctable_memory / sizeof(T);
        }
    }
protected:
    __no_constexpr void virtual validate_(
        std::vector<std::u8string>& _a_error_ref
    ) const noexcept;
    std::optional<std::filesystem::path> _m_file_to_write_to;
};
#endif

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

_BEGIN_ABC_NS
__no_constexpr_imp std::optional<std::vector<std::u8string>>
                   test_options_base_t::validate() const noexcept
{
    using namespace std;
    vector<u8string> _l_rv{};
    validate_(_l_rv);
    if (_l_rv.size() == 0)
    {
        return optional<vector<u8string>>{};
    }
    else
    {
        return optional<vector<u8string>>{_l_rv};
    }
}

__no_constexpr_imp void
    test_options_base_t::validate_(
        std::vector<std::u8string>& _a_error_ref
    ) const noexcept
{
    using namespace std;
    using namespace _ABC_NS_REPORTERS;
    vector<string> _l_rv;
    if (group_test_options.error_reporters.size() == 0)
    {
        _a_error_ref.push_back(fmt::format(
            u8"Error reporters must have atleast one element. Otherwise errors "
            u8"cannot be reported. "
            u8"If no others are available, "
            u8"included_instances/reporters/text_error_reporter.h contains an "
            u8"instance which prints errors to the console."
        ));
    }
    else
    {
        vector<size_t> _l_indexes_of_nullptrs{
            get_indexes_of_nullptrs(group_test_options.error_reporters)
        };
        if (_l_indexes_of_nullptrs.size() > 0)
        {
            _a_error_ref.push_back(fmt::format(
                u8"error_reporters has some elemnets which point to nullptr. "
                u8"Specifically those elements at the following indexes: {0}",
                _l_indexes_of_nullptrs
            ));
        }
    }
    if (group_test_options.test_reporters.size() == 0)
    {
        _a_error_ref.push_back(fmt::format(
            u8"test_reporters must have atleast one element. Otherwise tests "
            u8"cannot be reported. "
            u8"If no others are available, "
            u8"included_instances/reporters/text_test_reporter.hpp contains an "
            u8"instance which prints tests to the console."
        ));
    }
    else
    {
        vector<size_t> _l_indexes_of_nullptrs{
            get_indexes_of_nullptrs(group_test_options.test_reporters)
        };
        if (_l_indexes_of_nullptrs.size() > 0)
        {
            _a_error_ref.push_back(fmt::format(
                u8"test_reporters has some elemnets which point to nullptr. "
                u8"Specifically those elements at the following indexes: {0}",
                _l_indexes_of_nullptrs
            ));
        }
    }
    if (group_test_options.test_lists.size() == 0
        && glot_aware_test_options.use_global_test_list == false)
    {
        _a_error_ref.push_back(fmt::format(
            u8"test_lists must have atleast one element or "
            u8"use_global_test_list "
            u8"must be set to true. use_global_test_list = {0}",
            glot_aware_test_options.use_global_test_list
        ));
    }
    else if (group_test_options.test_lists.size() > 0)
    {
        vector<size_t> _l_indexes_of_nullptrs{
            get_indexes_of_nullptrs(group_test_options.test_lists)
        };
        if (_l_indexes_of_nullptrs.size() > 0)
        {
            _a_error_ref.push_back(fmt::format(
                u8"test_lists has some elemnets which point to nullptr. "
                u8"Specifically those elements at the following indexes: {0}",
                _l_indexes_of_nullptrs
            ));
        }
    }

    if (group_test_options.map_of_unique_ids_and_for_loop_stack_tries.size() > 0
        && individual_io_based_test_options.write_data_to_files)
    {
        _a_error_ref.push_back(fmt::format(
            u8"map_of_unique_ids_and_for_loop_stack_tries has elements in it - "
            u8"thus this run of the test suite corresponds to some test "
            u8"repetitions. However, we have set write_data_to_files to true. "
            u8"These two options are incompatable, as it will cause already "
            u8"written test data to be re-written to files."
        ));
    }
    if (group_test_options.threads == 0
        || group_test_options.threads > std::thread::hardware_concurrency())
    {
        _a_error_ref.push_back(fmt::format(
            u8"threads = {0}. This value must be between 1 and the number of "
            u8"threads available on the system ({1}",
            group_test_options.threads,
            std::thread::hardware_concurrency()
        ));
    }
    if (individual_io_based_test_options.comment_str.empty()
        || individual_io_based_test_options.comment_str == u8"\\n")
    {
        _a_error_ref.push_back(fmt::format(
            u8"comment_str = \"{0}\". It cannot be empty or equal to \"\\n\"",
            individual_io_based_test_options.comment_str
        ));
    }
    if (individual_io_based_test_options.general_data_extension.empty()
        || individual_io_based_test_options.general_data_extension.contains(
            u8"\n"
        )
        || individual_io_based_test_options.general_data_extension.contains(
            u8"."
        ))
    {
        _a_error_ref.push_back(fmt::format(
            u8"Invalid general_data_extension ({0}). Cannot be empty or "
            u8"contain "
            u8"any of the following: {{\\n,'.'",
            individual_io_based_test_options.general_data_extension
        ));
    }
    if (group_test_options.path_delimiter.empty())
    {
        _a_error_ref.push_back(fmt::format(
            u8"Invalid path_delimiter ({0}). Cannot be empty",
            group_test_options.path_delimiter
        ));
    }
    if (not filesystem::is_directory(individual_io_based_test_options.root_path
        ))
    {
        _a_error_ref.push_back(fmt::format(
            u8"Root folder \"{0}\" does not exist",
            individual_io_based_test_options.root_path
        ));
    }
}

__no_constexpr_imp void
test_options_base_t::pre_validation_process() noexcept
{

}

__no_constexpr_imp void
    test_options_base_t::post_validation_process() noexcept
{
    using namespace std;
    if (group_test_options.test_paths_to_run.size() == 0)
    {
        using namespace ds;
        group_test_options.test_paths_to_run = vector<u8string>(1, u8string{});
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
