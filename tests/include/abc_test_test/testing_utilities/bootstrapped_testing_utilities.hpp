#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/included_instances/reporters/memoized_error_reporter.hpp"
#include "abc_test/included_instances/reporters/memoized_test_reporter.hpp"
#include "abc_test/utility/internal/macros.hpp"

#include <functional>
#include <string>
#include <vector>

namespace abc_test::utility
{
struct test_result_t
{
    abc::reporters::memoized_error_reporter_t memoized_error_repoter;
    // abc::reporters::memoized_test_reporter_t memoized_test_reporter;
};

__no_constexpr_imp test_result_t
    abc_test_tests_internal_test_runner(
        const std::size_t                           _a_n_threads,
        const bool                                  _a_write_data_to_files,
        const std::vector<std::tuple<
            std::function<void()>,
            std::string,
            std::string,
            abc::ds::tdg_collection_stack_trie_t>>& _a_functions_to_run
    )
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    using namespace abc::utility::io;
    using namespace _ABC_NS_UTILITY_STR;
    using namespace _ABC_NS_DS;
    shared_ptr<_ABC_NS_DS::test_list_t> _l_tests_to_run
        = make_shared<_ABC_NS_DS::test_list_t>();
    test_options_base_t _l_running_options;
    _l_running_options.group_test_options.threads = _a_n_threads;
    _l_running_options.group_test_options.test_lists.push_back(_l_tests_to_run);
    _l_running_options.group_test_options.error_reporters.push_back(
        make_shared<reporters::memoized_error_reporter_t>()
    );
    _l_running_options.group_test_options.test_reporters.push_back(
        make_shared<reporters::memoized_test_reporter_t>()
    );
    _l_running_options.individual_io_based_test_options.write_data_to_files
        = _a_write_data_to_files;

    for (auto&& [_l_function, _l_name, _l_path, _l_repetition_info] :
         _a_functions_to_run)
    {
        test_function_t ki = std::make_shared<
            test_function_callable_object_t<decltype(_l_function)>>(_l_function
        );
        abc::ds::registered_test_data_t _l_rtd(
            ki,
            abc::ds::checked_user_defined_test_data_t::test_data(
                abc::ds::user_defined_test_data_t{
                    .name = _l_name, .path = _l_path
                }
            )
        );
        _l_tests_to_run->push_back(_l_rtd);
        u8string _l_uuid;

        _l_uuid.append(cast_string_to_u8string(_l_path))
            .append(_l_running_options.group_test_options.path_delimiter)
            .append(cast_string_to_u8string(_l_name));
        if (not _a_write_data_to_files)
        {
            _l_running_options.group_test_options
                .map_of_unique_ids_and_for_loop_stack_tries.insert(
                    _l_uuid, _l_repetition_info
                );
        }
    }
    simple_console_reporter_t _l_sco;
    abc::run_tests<test_options_base_t>(_l_running_options, _l_sco);
    return test_result_t{
        .memoized_error_repoter
        = *(static_cast<reporters::memoized_error_reporter_t*>(
            _l_running_options.group_test_options.error_reporters.at(0).get()
        ))
    };
}

using file_name_basic_info_t = std::variant<
    std::string,
    std::pair<std::string, std::string>,
    std::filesystem::path>;

template <typename T>
using file_name_info_t =
std::variant<
    // gdf
    file_name_basic_info_t,
    // tdf
    file_name_basic_info_t,
    // gdf with rw info
    std::pair<file_name_basic_info_t, abc::utility::str::rw_info_t<T>>>;

__constexpr abc::utility::io::general_data_t
            make_gdf(
                const file_name_basic_info_t& _a_gdf
            )
{
    using namespace abc::utility::io;
    using namespace abc;
    switch (_a_gdf.index())
    {
    case 0:
        return gdf(get<0>(_a_gdf));
    case 1:
        return gdf(get<1>(_a_gdf).first, get<1>(_a_gdf).second);
    case 2:
        return gdf_from_path(get<2>(_a_gdf));
    default:
        return gdf("");
    }
}

inline abc::utility::io::tertiary_data_t
    make_tdf(
        const file_name_basic_info_t& _a_tdf
    )
{
    using namespace abc::utility::io;
    using namespace abc;
    switch (_a_tdf.index())
    {
    case 0:
        return tdf(get<0>(_a_tdf));
    case 1:
        return tdf(get<1>(_a_tdf).first, get<1>(_a_tdf).second);
    case 2:
        return tdf_from_path(get<2>(_a_tdf));
    default:
        return tdf("");
    }
}

template <typename T>
inline _ABC_NS_UTILITY_IO::general_data_with_rw_info_t<T>
    make_gdf_with_rw_info(
        const std::pair<
            file_name_basic_info_t,
            _ABC_NS_UTILITY_STR::rw_info_t<T>>& _a_rw_info
    ) noexcept
{
    using namespace _ABC_NS_UTILITY_IO;
    return general_data_with_rw_info_t(
        make_gdf(_a_rw_info.first), _a_rw_info.second
    );
}

template <typename T>
inline _ABC_NS_UTILITY_IO::file_name_t<T>
    make_file_name(
        const file_name_info_t<T>& _a_gdf
    ) noexcept
{
    switch (_a_gdf.index())
    {
    case 0:
        return make_gdf(get<0>(_a_gdf));
    case 1:
        return make_tdf(get<1>(_a_gdf));
    case 2:
        return make_gdf_with_rw_info(get<2>(_a_gdf));
    }
}

inline std::filesystem::path
    make_path(
        const file_name_basic_info_t& _a_gdf,
        const std::u8string_view      _a_extension,
        const std::filesystem::path&  _a_root_path
    ) noexcept
{
    using namespace abc::utility::io;
    using namespace abc;
    using namespace std;
    switch (_a_gdf.index())
    {
    case 0:
        return filesystem::path(_a_root_path / std::get<0>(_a_gdf))
            .concat(_a_extension);
    case 1:
        return filesystem::path(
                   _a_root_path / std::get<1>(_a_gdf).first
                   / std::get<1>(_a_gdf).second
        )
            .concat(_a_extension);
    case 2:
        return filesystem::path(get<2>(_a_gdf));
    default:
        return std::filesystem::path{};
    }
}

template <typename T>
inline std::filesystem::path
    make_path(
        const file_name_info_t<T>&   _a_gdf,
        const std::u8string_view     _a_extension_gd,
        const std::u8string_view     _a_extension_td,
        const std::filesystem::path& _a_root_path
    ) noexcept
{
    using namespace abc::utility::io;
    using namespace abc;
    using namespace std;
    switch (_a_gdf.index())
    {
    case 0:
        return make_path(get<0>(_a_gdf), _a_extension_gd, _a_root_path);
    case 1:
        return make_path(get<1>(_a_gdf), _a_extension_td, _a_root_path);
    case 2:
        return make_path(get<2>(_a_gdf).first, _a_extension_gd, _a_root_path);
    default:
        return std::filesystem::path{};
    }
}

inline std::vector<std::string>
    get_file_contents(
        const std::filesystem::path& _a_path
    ) noexcept
{
    using namespace std;
    vector<string> _l_file_contents;
    string         _l_line;
    ifstream       _l_input_file(_a_path);
    while (std::getline(_l_input_file, _l_line))
    {
        _l_file_contents.push_back(_l_line);
    }
    return _l_file_contents;
}

template <typename T, typename F, std::size_t... Is>
auto transform_array_impl(const std::array<T, sizeof...(Is)>& input, F&& f, std::index_sequence<Is...>)
{
    return std::array<decltype(f(input[0])), sizeof...(Is)>{{f(input[Is])...}};
}

template <typename T, std::size_t K, typename F>
auto
    transform_array(
        const std::array<T, K>& input,
        F&&                     f
    )
{
    return transform_array_impl(
        input, std::forward<F>(f), std::make_index_sequence<K>{}
    );
}
} // namespace abc_test::utility