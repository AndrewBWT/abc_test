#pragma once

#include "abc_test/core/options/test_options_base.hpp"
#include "abc_test/included_instances/reporters/text_error_reporter.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter.hpp"

_BEGIN_ABC_NS

enum class console_output_e
{
    no_output,
    coloured_output,
    monochrome_output
};
template <>
inline auto
utility::get_enum_list() noexcept -> utility::enum_list_t<
    console_output_e>
{
    using enum console_output_e;
    return {
        _ENUM_LIST_ENTRY(no_output),
        _ENUM_LIST_ENTRY(coloured_output),
        _ENUM_LIST_ENTRY(monochrome_output)
    };
}

struct included_instances_test_options_t : public test_options_base_t
{
public:
    console_output_e use_text_test_reporter_to_cout
        = console_output_e::coloured_output;
    console_output_e use_text_error_reporter_to_cout
        = console_output_e::coloured_output;
    std::vector<std::pair<std::filesystem::path, bool>>
        text_test_reporter_file_names;
    std::vector<std::pair<std::filesystem::path, bool>>
        text_error_reporter_file_names;
protected:
    __no_constexpr virtual void
        validate_and_pre_process_(std::vector<std::u8string>& _a_error_ref
        ) noexcept;
};

namespace
{
__no_constexpr std::vector<std::filesystem::path>
               check_files(
                   std::vector<std::u8string>& _a_error_ref,
                   const std::vector<std::pair<std::filesystem::path, bool>>&
                                                _a_potential_files,
                   const std::filesystem::path& _a_root_path,
                   const std::string_view       _a_identifier
               ) noexcept;
} // namespace

_END_ABC_NS

template <>
struct fmt::formatter<abc::included_instances_test_options_t>
    : formatter<string_view>
{
    // parse is inherited from formatter<string_view>.
    // Can'tbe constexpr due to use of fmt::format
    __no_constexpr auto
        format(
            abc::included_instances_test_options_t _a_rtd,
            format_context&                        _a_ctx
        ) const -> format_context::iterator;
};

_BEGIN_ABC_NS
__no_constexpr_imp void
    included_instances_test_options_t::validate_and_pre_process_(
        std::vector<std::u8string>& _a_error_ref
    ) noexcept
{
    using namespace _ABC_NS_REPORTERS;
    using namespace std;
    using namespace std::filesystem;
    using enum console_output_e;
    switch (use_text_error_reporter_to_cout)
    {
    case no_output:
        break;
    case coloured_output:
        this->error_reporters.push_back(make_shared<text_error_reporter_t>(true));
        break;
    case monochrome_output:
        this->error_reporters.push_back(make_shared<text_error_reporter_t>(false));
        break;
    }
    switch (use_text_test_reporter_to_cout)
    {
    case no_output:
        break;
    case coloured_output:
        this->test_reporters.push_back(make_shared<text_test_reporter_t>(true));
        break;
    case monochrome_output:
        this->test_reporters.push_back(make_shared<text_test_reporter_t>(false));
        break;
    }
    vector<path> _l_error_files{check_files(
        _a_error_ref,
        text_error_reporter_file_names,
        root_path,
        "text_error_reporter_t"
    )};
    vector<path> _l_test_files{check_files(
        _a_error_ref,
        text_test_reporter_file_names,
        root_path,
        "text_test_reporter_t"
    )};
    // No errors in the system. We can continue.
    if (_a_error_ref.size() == 0)
    {
        for (const path& _l_path : _l_error_files)
        {
            this->error_reporters.push_back(
                make_shared<text_error_reporter_t>(_l_path)
            );
        }
    }
    if (_a_error_ref.size() == 0)
    {
        for (const path& _l_path : _l_test_files)
        {
            this->test_reporters.push_back(
                make_shared<text_test_reporter_t>(_l_path)
            );
        }
    }
    test_options_base_t::validate_and_pre_process_(_a_error_ref);
}

namespace
{
__no_constexpr_imp std::vector<std::filesystem::path>
                   check_files(
                       std::vector<std::u8string>& _a_error_ref,
                       const std::vector<std::pair<std::filesystem::path, bool>>&
                                                    _a_potential_files,
                       const std::filesystem::path& _a_root_path,
                       const std::string_view       _a_identifier
                   ) noexcept
{
    using namespace std::filesystem;
    using namespace std;
    using namespace _ABC_NS_UTILITY_STR;
    vector<path> _l_rv;
    for (const auto& [_l_path, _l_duplicates_allowed] : _a_potential_files)
    {
        const path _l_file_to_be_made{_a_root_path / _l_path};
        const path _l_parent_folder{_l_file_to_be_made.parent_path()};
        if (not exists(_l_parent_folder))
        {
            _a_error_ref.push_back(fmt::format(
                u8"File for {0} \"{1}\" unable to be created because parent "
                u8"path {2} does not exist",
                cast_string_to_u8string(_a_identifier),
                _l_file_to_be_made,
                _l_parent_folder
            ));
        }
        else if (_l_duplicates_allowed == false && exists(_l_file_to_be_made))
        {
            _a_error_ref.push_back(fmt::format(
                u8"File for {0} \"{1}\" unable to be created because it "
                u8"already "
                u8"exists",
                u8"hello",
                _l_file_to_be_made
            ));
        }
        else
        {
            if (_l_duplicates_allowed)
            {
                if (exists(_l_file_to_be_made))
                {
                    size_t _l_int{0};
                    path   _l_file_to_be_made_with_int{
                        _l_file_to_be_made
                        / string("_").append(to_string(_l_int))
                    };
                    while (exists(_l_file_to_be_made_with_int))
                    {
                        ++_l_int;
                        path _l_file_to_be_made_with_int{
                            _l_file_to_be_made
                            / string("_").append(to_string(_l_int))
                        };
                    }
                    _l_rv.push_back(_l_file_to_be_made_with_int);
                }
                else
                {
                    _l_rv.push_back(_l_file_to_be_made);
                }
            }
            else
            {
                _l_rv.push_back(_l_file_to_be_made);
            }
        }
    }
    return _l_rv;
}
} // namespace

_END_ABC_NS

__no_constexpr_imp auto
    fmt::formatter<abc::included_instances_test_options_t>::format(
        abc::included_instances_test_options_t _a_rtd,
        format_context&                        _a_ctx
    ) const -> format_context::iterator
{
    using namespace std;
    const string _l_rv{fmt::format(
        "{0}{{{1}"
        ", {2} = {3}"
        ", {4} = {5}"
        ", {6} = {7}"
        ", {8} = {9}"
        "}}",
        typeid(_a_rtd).name(),
        make_test_options_base_member_variables_fmt(_a_rtd),
        "use_text_test_reporter_to_cout",
        "hi",
       // _a_rtd.use_text_test_reporter_to_cout,
        "use_text_error_reporter_to_cout",
        "hi",
        //_a_rtd.use_text_error_reporter_to_cout,
        "text_test_reporter_file_names",
        _a_rtd.text_test_reporter_file_names,
        "text_error_reporter_file_names",
        _a_rtd.text_error_reporter_file_names
    )};
    return formatter<string_view>::format(_l_rv, _a_ctx);
}

_BEGIN_ABC_UTILITY_PRINTER_NS
/*__no_constexpr_imp std::string
    default_printer_t<abc::ds::map_unique_id_to_tdg_collection_stack_trie_t>::
        run_printer(
            const abc::ds::map_unique_id_to_tdg_collection_stack_trie_t&
                _a_parse_input
        ) const
{
    using namespace abc::ds;
    return fmt::format("{}", _a_parse_input.map());
}*/

_END_ABC_UTILITY_PRINTER_NS