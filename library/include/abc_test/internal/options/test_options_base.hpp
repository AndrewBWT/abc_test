#pragma once

#include "abc_test/external/CLI11/CLI11.hpp"
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
    std::size_t size_of_repetition_files = 1'000;
    /*!
     * @brief Function to validate the input.
     *
     * Any errors are written to the optional string outout.
     *
     * @return Nullopt if there are no errors. Otherwise a string representation
     * of the error.
     */
    __no_constexpr std::optional<std::vector<std::string>>
                   validate_and_pre_process(
                   ) noexcept;
protected:
    __no_constexpr void virtual validate_and_pre_process_(
        std::vector<std::string>&                     _a_error_ref
    ) noexcept;
    std::optional<std::filesystem::path> _m_file_to_write_to;
    __constexpr void
        parse_and_write_data(const rep_data_fields_t& _a_strs) noexcept;
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
__no_constexpr CLI::App*
    build_auto_repetition_file_parser(test_options_base_t& _a_opts) noexcept;
__no_constexpr std::expected<rep_data_file_info_t, std::string>
    get_most_recent_test_data(const std::string_view _a_folder) noexcept;
__no_constexpr std::expected<rep_data_file_info_t, std::string>
               get_specific_test_data(
                   const std::string_view _a_folder,
                   const std::size_t      _a_repetition_index
               ) noexcept;
__no_constexpr std::filesystem::path
               create_file_to_write_to(
                   const std::string_view _a_folder,
                   const std::size_t      _a_begin_index,
                   const std::size_t      _a_end_index
               ) noexcept;
__constexpr
    std::expected<std::tuple<std::size_t, bool, rep_data_fields_t>, std::string>
    navigate_to_repetition_index(
        const std::filesystem::path&      _a_path,
        const std::optional<std::size_t>& _a_opt_index
        = std::optional<std::size_t>{}
    ) noexcept;
__constexpr std::optional<std::pair<std::string, std::string>>
    parse_equals_separated_line(const std::string_view _a_line) noexcept;
__constexpr std::optional<bool>
            parse_test_passed(const std::string_view _a_line) noexcept;
__constexpr std::optional<std::size_t>
            parse_test_index(const std::string_view _a_line) noexcept;
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
                   test_options_base_t::validate_and_pre_process(
    ) noexcept
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
    /*if (automatic_file_line_index.has_value()
        && not automatic_repetition_data_folder.has_value())
    {
        _a_error_ref.push_back(fmt::format(
            "Automatic repetition data file folder does not have value, while "
            "an automatic test re-run index has been provided."
        ));
    }
    if (automatic_repeating_of_previous_test_if_failure
        && not automatic_repetition_data_folder.has_value())
    {
        _a_error_ref.push_back(fmt::format(
            "Automatic repetition data file folder does not have value, while "
            "automatic_repeating_of_previous_test_if_failure is set to true. "
            "For tests to be automatically re-run, a folder must be provied"
        ));
    }
    if (automatic_repetition_data_folder.has_value())
    {
        if (automatic_file_line_index.has_value())
        {
            // Automatic repetition with a specific index.
            const auto _l_file_found_result{get_specific_test_data(
                automatic_repetition_data_folder.value(),
                automatic_file_line_index.value()
            )};
            if (_l_file_found_result.has_value())
            {
                const auto
                    [_l_test_passed,
                     _l_data_to_re_write,
                     _l_file_name,
                     _l_max_idx_of_file,
                     _l_idx_of_test]{_l_file_found_result.value()};
                _a_file_process_function(_l_data_to_re_write, _a_error_ref);
            }
            else
            {
                _a_error_ref.push_back(fmt::format(
                    "Automatic repetition data file error {0}",
                    _l_file_found_result.error()
                ));
            }
        }
        else if (automatic_repeating_of_previous_test_if_failure)
        {
            // Automatic repetition with a specific index.
            const auto _l_file_found_result{get_most_recent_test_data(
                automatic_repetition_data_folder.value()
            )};
            if (_l_file_found_result.has_value())
            {
                // automatic repetition, last test.
                const auto
                    [_l_test_passed,
                     _l_data_to_re_write,
                     _l_file_name,
                     _l_max_idx_of_file,
                     _l_idx_of_test]{_l_file_found_result.value()};

                if (not _l_test_passed)
                {
                    _a_file_process_function(_l_data_to_re_write, _a_error_ref);
                }
                else
                {
                    if (_l_idx_of_test + 1 > _l_max_idx_of_file)
                    {
                        _m_file_to_write_to = create_file_to_write_to(
                            automatic_repetition_data_folder.value(),
                            _l_idx_of_test + 1,
                            _l_idx_of_test + size_of_repetition_files
                        );
                    }
                    else
                    {
                        _m_file_to_write_to = _l_file_name;
                    }
                }
                // Get the previous test and re-run it if failure.
                // If it
            }
            else
            {
                _a_error_ref.push_back(fmt::format(
                    "Automatic repetition data file error {0}",
                    _l_file_found_result.error()
                ));
            }
        }
        else
        {
        }
    }*/
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
        test_paths_to_run
            = vector<string>(1, "");
    }
}

__constexpr_imp void
    test_options_base_t::parse_and_write_data(
        const std::vector<std::pair<std::string, std::string>>& _a_strs
    ) noexcept
{
    using namespace std;
    for (const auto& [_l_field_name, _l_field_data] : _a_strs)
    {
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

__no_constexpr_imp std::expected<rep_data_file_info_t, std::string>
                   get_most_recent_test_data(
                       const std::string_view _a_folder
                   ) noexcept
{
    using namespace std;
    const string           auto_repetition_file_name = "autofile";
    const filesystem::path _l_folder{_a_folder};
    // Create the directory if it doesn't exist.
    if (not filesystem::exists(_l_folder)
        || not filesystem::is_directory(_l_folder))
    {
        auto f1 = _l_folder;
        if (not filesystem::exists(_l_folder.parent_path()))
        {
            return std::unexpected(fmt::format(
                "Parent folder {0} does not exist", _l_folder.parent_path()
            ));
        }
        filesystem::create_directory(_l_folder);
    }
    // Read the files and either find the correct one, or create a new one.
    optional<tuple<filesystem::path, size_t, size_t>> _l_correct_file_opt;
    optional<size_t>                                  _l_previous_greatest;
    for (const filesystem::path& _l_file :
         filesystem::directory_iterator(_l_folder))
    {
        const string _l_file_name{_l_file.filename().string()};
        if (auto _l_pos{_l_file_name.find(auto_repetition_file_name)};
            _l_pos != string::npos || _l_pos == 0)
        {
            // The filename begins with the correct string.
            const size_t _l_first_underscore{
                _l_file_name.find_first_of("_", _l_pos)
            };
            if (_l_first_underscore == string::npos)
            {
                continue;
            }
            const size_t _l_second_underscore{
                _l_file_name.find_first_of("_", _l_first_underscore + 1)
            };
            if (_l_second_underscore == string::npos)
            {
                continue;
            }
            // Get the information out of the file name.
            size_t _l_begin_index{0};
            size_t _l_end_index{0};
            try
            {
                _l_begin_index = stoull(_l_file_name.substr(
                    _l_first_underscore + 1,
                    _l_second_underscore - (_l_first_underscore + 1)
                ));
                _l_end_index
                    = stoull(_l_file_name.substr(_l_second_underscore + 1));
            }
            catch (...)
            {
                continue;
            }
            // If we have a pre-determined repetition to try, find out if
            // this file has it in.
            if (not _l_previous_greatest.has_value())
            {
                _l_previous_greatest = _l_end_index;
                _l_correct_file_opt
                    = make_tuple(_l_file, _l_begin_index, _l_end_index);
            }
            else
            {
                if (_l_previous_greatest.value() < _l_begin_index)
                {
                    _l_previous_greatest = _l_end_index;
                    _l_correct_file_opt
                        = make_tuple(_l_file, _l_begin_index, _l_end_index);
                }
            }
        }
    }
    optional<tuple<size_t, _ABC_NS_UTILITY::complete_global_seed_t, string>>
        _l_repetition_data;
    if (not _l_correct_file_opt.has_value())
    {
        return tuple<bool, rep_data_fields_t, filesystem::path, size_t, size_t>{
        };
    }
    else
    {
        tuple<bool, rep_data_fields_t, filesystem::path, size_t, size_t> _l_rv;
        const auto& [_l_file, _l_begin_idx, _l_end_idx]{
            _l_correct_file_opt.value()
        };
        get<3>(_l_rv) = _l_end_idx;
        get<2>(_l_rv) = _l_file;
        ifstream _l_ifstream(_l_file);
        const expected<tuple<size_t, bool, rep_data_fields_t>, string> _l_res{
            navigate_to_repetition_index(_l_file)
        };
        if (not _l_res.has_value())
        {
            return std::unexpected(_l_res.error());
        }
        get<0>(_l_rv) = get<1>(_l_res.value());
        get<1>(_l_rv) = get<2>(_l_res.value());
        get<4>(_l_rv) = get<0>(_l_res.value());
        return _l_rv;
    }
}

__no_constexpr_imp std::expected<rep_data_file_info_t, std::string>
                   get_specific_test_data(
                       const std::string_view _a_folder,
                       const std::size_t      _a_repetition_index
                   ) noexcept
{
    using namespace std;
    const string           auto_repetition_file_name = "autofile";
    const filesystem::path _l_folder{_a_folder};
    // Create the directory if it doesn't exist.
    if (not filesystem::exists(_l_folder)
        || not filesystem::is_directory(_l_folder))
    {
        filesystem::create_directory(_l_folder);
    }
    // Read the files and either find the correct one, or create a new one.
    optional<tuple<filesystem::path, size_t, size_t>> _l_correct_file_opt;
    optional<size_t>                                  _l_previous_greatest;
    for (const filesystem::path& _l_file :
         filesystem::directory_iterator(_l_folder))
    {
        const string _l_file_name{_l_file.filename().string()};
        if (auto _l_pos{_l_file_name.find(auto_repetition_file_name)};
            _l_pos != string::npos || _l_pos == 0)
        {
            // The filename begins with the correct string.
            const size_t _l_first_underscore{
                _l_file_name.find_first_of("_", _l_pos)
            };
            if (_l_first_underscore == string::npos)
            {
                continue;
            }
            const size_t _l_second_underscore{
                _l_file_name.find_first_of("_", _l_first_underscore + 1)
            };
            if (_l_second_underscore == string::npos)
            {
                continue;
            }
            // Get the information out of the file name.
            size_t _l_begin_index{0};
            size_t _l_end_index{0};
            try
            {
                _l_begin_index = stoull(_l_file_name.substr(
                    _l_first_underscore + 1,
                    _l_second_underscore - (_l_first_underscore + 1)
                ));
                _l_end_index
                    = stoull(_l_file_name.substr(_l_second_underscore + 1));
            }
            catch (...)
            {
                continue;
            }
            // If we have a pre-determined repetition to try, find out if
            // this file has it in.
            if (_a_repetition_index >= _l_begin_index
                && _a_repetition_index <= _l_end_index)
            {
                _l_correct_file_opt
                    = make_tuple(_l_file, _l_begin_index, _l_end_index);
                break;
            }
        }
    }
    optional<tuple<size_t, _ABC_NS_UTILITY::complete_global_seed_t, string>>
        _l_repetition_data;
    if (not _l_correct_file_opt.has_value())
    {
        return tuple<bool, rep_data_fields_t, filesystem::path, size_t, size_t>{
        };
    }
    else
    {
        tuple<bool, rep_data_fields_t, filesystem::path, size_t, size_t> _l_rv;
        const auto& [_l_file, _l_begin_idx, _l_end_idx]{
            _l_correct_file_opt.value()
        };
        get<3>(_l_rv) = _l_end_idx;
        get<2>(_l_rv) = _l_file;
        ifstream _l_ifstream(_l_file);
        const expected<tuple<size_t, bool, rep_data_fields_t>, string> _l_res{
            navigate_to_repetition_index(_l_file, _a_repetition_index)
        };
        if (not _l_res.has_value())
        {
            return std::unexpected("Couldn't parse");
        }
        get<0>(_l_rv) = get<1>(_l_res.value());
        get<1>(_l_rv) = get<2>(_l_res.value());
        get<4>(_l_rv) = get<0>(_l_res.value());
        return _l_rv;
    }
}

__no_constexpr_imp std::filesystem::path
                   create_file_to_write_to(
                       const std::string_view _a_folder,
                       const std::size_t      _a_begin_index,
                       const std::size_t      _a_end_index
                   ) noexcept
{
    using namespace std;
    const string _a_file{
        fmt::format("autofile_{0}_{1}", _a_begin_index, _a_end_index)
    };
    const filesystem::path _l_file_to_create(
        filesystem::path(_a_folder) / _a_file
    );
    ofstream _l_if(_l_file_to_create);
    return _l_file_to_create;
}

__constexpr_imp
    std::expected<std::tuple<std::size_t, bool, rep_data_fields_t>, std::string>
    navigate_to_repetition_index(
        const std::filesystem::path&      _a_path,
        const std::optional<std::size_t>& _a_opt_index
    ) noexcept
{
    using namespace std;
    tuple<size_t, bool, rep_data_fields_t> _l_rv;
    ifstream                               _l_ifs(_a_path);
    string                                 _l_line;
    if (_a_opt_index.has_value())
    {
        optional<size_t> _l_index{};
        while (true)
        {
            if (std::getline(_l_ifs, _l_line))
            {
                optional<size_t> _l_test_index{parse_test_index(_l_line)};
                if (_l_test_index.has_value()
                    && _l_test_index.value() == _a_opt_index.value())
                {
                    get<0>(_l_rv) = _l_test_index.value();
                    if (std::getline(_l_ifs, _l_line))
                    {
                        optional<bool> _l_test_passed{parse_test_passed(_l_line)
                        };
                        if (_l_test_passed.has_value())
                        {
                            get<1>(_l_rv) = _l_test_passed.value();
                            while (true)
                            {
                                if (std::getline(_l_ifs, _l_line))
                                {
                                    if (parse_test_index(_l_line).has_value())
                                    {
                                        return _l_rv;
                                    }
                                    optional<pair<string, string>>
                                        _l_equals_separated_line{
                                            parse_equals_separated_line(_l_line)
                                        };
                                    if (_l_equals_separated_line.has_value())
                                    {
                                        get<2>(_l_rv).push_back(
                                            _l_equals_separated_line.value()
                                        );
                                    }
                                    else
                                    {
                                        return std::unexpected(
                                            "Expected an equals separated "
                                            "line. did not get"
                                        );
                                    }
                                }
                            }
                        }
                        else
                        {
                            return std::unexpected(fmt::format(
                                "Could not parse bool denoting test success or "
                                "failure after finding test index {0} in file "
                                "{1}. String was \"{2}\"",
                                _l_test_index.value(),
                                _a_path,
                                _l_line
                            ));
                        }
                    }
                    else
                    {
                        return std::unexpected(
                            fmt::format("Line expected but none found")
                        );
                    }
                }
            }
            else
            {
                return std::unexpected(fmt::format(
                    "Could not find test index {0} in file {1}",
                    _a_opt_index.value(),
                    _a_path
                ));
            }
        }
    }
    else
    {
        optional<tuple<size_t, bool, rep_data_fields_t>> _l_last_rv;
        while (true)
        {
            if (std::getline(_l_ifs, _l_line))
            {
                optional<size_t> _l_test_index{parse_test_index(_l_line)};
                if (_l_test_index.has_value())
                {
                    get<0>(_l_rv) = _l_test_index.value();
                    if (std::getline(_l_ifs, _l_line))
                    {
                        optional<bool> _l_test_passed{parse_test_passed(_l_line)
                        };
                        if (_l_test_passed.has_value())
                        {
                            get<1>(_l_rv) = _l_test_passed.value();
                            while (true)
                            {
                                streampos oldpos
                                    = _l_ifs.tellg(); // stores the position
                                if (std::getline(_l_ifs, _l_line))
                                {
                                    if (parse_test_index(_l_line).has_value())
                                    {
                                        _l_ifs.seekg(oldpos);
                                        _l_last_rv = _l_rv;
                                        _l_rv      = tuple<
                                                 size_t,
                                                 bool,
                                                 rep_data_fields_t>{};
                                        break;
                                    }
                                    optional<pair<string, string>>
                                        _l_equals_separated_line{
                                            parse_equals_separated_line(_l_line)
                                        };
                                    if (_l_equals_separated_line.has_value())
                                    {
                                        get<2>(_l_rv).push_back(
                                            _l_equals_separated_line.value()
                                        );
                                    }
                                    else
                                    {
                                        return std::unexpected(
                                            "Expected an equals separated "
                                            "line. did not get"
                                        );
                                    }
                                }
                                else
                                {
                                    return _l_rv;
                                }
                            }
                        }
                        else
                        {
                            return std::unexpected(fmt::format(
                                "Could not parse bool denoting test success or "
                                "failure after finding test index {0} in file "
                                "{1}. String was \"{2}\"",
                                _l_test_index.value(),
                                _a_path,
                                _l_line
                            ));
                        }
                    }
                    else
                    {
                        return std::unexpected(
                            fmt::format("Line expected but none found")
                        );
                    }
                }
            }
            else
            {
                return std::unexpected(fmt::format(
                    "Could not find any previously ran tests in file \"{0}\"",
                    _a_path
                ));
            }
        }
    }
    return std::tuple<std::size_t, bool, rep_data_fields_t>{};
}

__constexpr_imp std::optional<std::pair<std::string, std::string>>
                parse_equals_separated_line(
                    const std::string_view _a_line
                ) noexcept
{
    using namespace std;
    const size_t _l_pos_first_equals{_a_line.find_first_of("=")};
    if (_l_pos_first_equals == string::npos)
    {
        return optional<pair<string, string>>{};
    }
    else
    {
        if (_l_pos_first_equals > 0 && _a_line.size() > _l_pos_first_equals + 1
            && _a_line[_l_pos_first_equals - 1] == ' '
            && _a_line[_l_pos_first_equals + 1] == ' ')
        {
            return make_pair(
                string(_a_line.substr(0, _l_pos_first_equals - 1)),
                string(_a_line.substr(_l_pos_first_equals + 2))
            );
        }
        else
        {
            return optional<pair<string, string>>{};
        }
    }
}

__constexpr_imp std::optional<bool>
                parse_test_passed(
                    const std::string_view _a_line
                ) noexcept
{
    using namespace std;
    optional<pair<string, string>> _l_equals_parsed{
        parse_equals_separated_line(_a_line)
    };
    if (_l_equals_parsed.has_value())
    {
        if (_l_equals_parsed.value().first == "passed")
        {
            if (_l_equals_parsed.value().second == "true")
            {
                return true;
            }
            else if (_l_equals_parsed.value().second == "false")
            {
                return false;
            }
            else
            {
                return optional<bool>{};
            }
        }
        else
        {
            return optional<bool>{};
        }
    }
    else
    {
        return optional<bool>{};
    }
}

__constexpr_imp std::optional<std::size_t>
                parse_test_index(
                    const std::string_view _a_line
                ) noexcept
{
    using namespace std;
    optional<pair<string, string>> _l_equals_parsed{
        parse_equals_separated_line(_a_line)
    };
    if (_l_equals_parsed.has_value())
    {
        if (_l_equals_parsed.value().first == "test_config_index")
        {
            size_t _l_test_index{0};
            try
            {
                _l_test_index = stoull(_l_equals_parsed.value().second);
                return _l_test_index;
            }
            catch (...)
            {
                return optional<size_t>{};
            }
        }
        else
        {
            return optional<size_t>{};
        }
    }
    else
    {
        return optional<size_t>{};
    }
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