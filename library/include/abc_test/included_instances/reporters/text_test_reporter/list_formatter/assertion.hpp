#pragma once
#include "abc_test/core/test_reports/assertion.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/assertion.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/list_formatter/user_initialised_report.hpp"
#include "abc_test/utility/types.hpp"
_BEGIN_ABC_REPORTERS_NS

template <bool Single_Source, typename Assertion_Status>
struct assertion_list_formatter_t
    : public list_formattable_t<
          reports::assertion_t<Single_Source, Assertion_Status>,
          combined_enum_assertion_fields_t,
          print_config_t>
    , public user_initialised_report_list_formatter_t<Single_Source>
{
public:
    using user_initialised_report_list_formatter_t<
        Single_Source>::user_initialised_report_list_formatter_t;
    __constexpr virtual bool
        check_data(
            const combined_enum_assertion_fields_t& _a_fid,
            const reports::assertion_t<Single_Source, Assertion_Status>&
                _a_element
        ) const;
    __constexpr virtual void
        get_data(
            const combined_enum_assertion_fields_t& _a_fid,
            const reports::assertion_t<Single_Source, Assertion_Status>&
                                                                _a_element,
            const print_config_t&                               _a_pc,
            const utility::io::threated_text_output_reporter_t& _a_ttor,
            const std::size_t                                   _a_idx
        ) const;
    __constexpr virtual std::u8string
        get_str_representation(
            const reports::assertion_t<Single_Source, Assertion_Status>&
                                  _a_element,
            const print_config_t& _a_pc
        ) const
        = 0;
};

template <bool Single_Source, typename Assertion_Status>
__constexpr std::u8string
            construct_str_representation(
                const reports::assertion_t<Single_Source, Assertion_Status>& _a_element,
                const std::u8string_view                                     _a_str,
                const std::u8string_view _a_suffix_str = u8""
            ) noexcept;
_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS

template <bool Single_Source, typename Assertion_Status>
__constexpr_imp bool
    assertion_list_formatter_t<Single_Source, Assertion_Status>::check_data(
        const combined_enum_assertion_fields_t&                      _a_fid,
        const reports::assertion_t<Single_Source, Assertion_Status>& _a_element
    ) const
{
    using namespace std;
    if (auto _l_ptr{get_if<enum_user_initialised_report_fields_t>(&_a_fid)};
        _l_ptr != nullptr)
    {
        return user_initialised_report_list_formatter_t<
            Single_Source>::check_data(*_l_ptr, _a_element);
    }
    else if (auto _l_ptr{get_if<enum_assertion_fields_t>(&_a_fid)};
             _l_ptr != nullptr)
    {
        using enum enum_assertion_fields_t;
        switch (*_l_ptr)
        {
        case STATUS:
        case STR_REPRESENTATION:
            return true;
        case TEST_DESCRIPTION:
            return true; // _a_element.test_description().has_value();
        default:
            throw errors::unaccounted_for_enum_exception(*_l_ptr);
        }
    }
    else
    {
        throw errors::unaccounted_for_variant_exception(_a_fid);
    }
}

template <bool Single_Source, typename Assertion_Status>
__constexpr_imp void
    assertion_list_formatter_t<Single_Source, Assertion_Status>::get_data(
        const combined_enum_assertion_fields_t&                      _a_fid,
        const reports::assertion_t<Single_Source, Assertion_Status>& _a_element,
        const print_config_t&                                        _a_pc,
        const utility::io::threated_text_output_reporter_t&          _a_ttor,
        const std::size_t                                            _a_idx
    ) const
{
    using namespace std;
    pair<u8string, u8string> _l_pair;
    if (auto _l_ptr{get_if<enum_user_initialised_report_fields_t>(&_a_fid)};
        _l_ptr != nullptr)
    {
        user_initialised_report_list_formatter_t<Single_Source>::get_data(
            *_l_ptr, _a_element, _a_pc, _a_ttor, _a_idx
        );
    }
    else if (auto _l_ptr{get_if<enum_assertion_fields_t>(&_a_fid)};
             _l_ptr != nullptr)
    {
        using enum enum_assertion_fields_t;
        using namespace std;
        switch (*_l_ptr)
        {
        case STATUS:
            if constexpr (std::derived_from<
                              Assertion_Status,
                              abc::reports::dynamic_status_t>)
            {
                _l_pair
                    = {_a_pc.colon(_a_pc.status_str()),
                       _a_pc.indent(
                           _a_pc.status<Assertion_Status>(_a_element.status())
                       )};
            }
            else
            {
                _l_pair
                    = {_a_pc.colon(_a_pc.status_str()),
                       _a_pc.indent(_a_pc.status<Assertion_Status>())};
            }
            break;
        case TEST_DESCRIPTION:
            if constexpr (not Single_Source)
            {
                _l_pair
                    = {_a_pc.colon(_a_pc.test_description_str()),
                       _a_pc.indent(_a_pc.test_description(abc::unpack_string_to_u8string(
                           _a_element.test_description()
                       )))};
            }
            else
            {
                _l_pair = {u8"", u8""};
            }
            break;
        case STR_REPRESENTATION:
            _l_pair.first = _a_pc.highlight_pass_or_fail(
                _a_element.get_pass_status(),
                get_str_representation(_a_element, _a_pc)
            );
            break;
        default:
            throw errors::unaccounted_for_enum_exception(*_l_ptr);
        }
    }
    else
    {
        throw errors::unaccounted_for_variant_exception(_a_fid);
    }
    if (not _l_pair.first.empty() || not _l_pair.second.empty())
    {
        _a_ttor.write_line(fmt::format(
            u8"{0}{1}{2}", this->prefix(_a_idx), _l_pair.first, _l_pair.second
        ));
    }
    else
    {
        int i = 3;
    }
}

template <bool Single_Source, typename Assertion_Status>
__constexpr_imp std::u8string
                construct_str_representation(
                    const reports::assertion_t<Single_Source, Assertion_Status>& _a_element,
                    const std::u8string_view                                     _a_str,
                    const std::u8string_view _a_suffix_str
                ) noexcept
{
    using namespace std;
    using namespace reports;
    const bool   _l_passed{_a_element.get_pass_status()};
    const u8string _l_terminate_function_str{
        (not _l_passed && same_as<Assertion_Status, pass_or_terminate_t>
         || same_as<Assertion_Status, terminate_t>)
            ? u8" Assertion terminated function."
            : u8""
    };
    const u8string _l_status_str{_l_passed ? u8"passed" : u8"failed"};
    return fmt::format(
        u8"{0} {1}.{2}{3}",
        _a_str,
        _l_status_str,
        _a_suffix_str,
        _l_terminate_function_str
    );
}

_END_ABC_REPORTERS_NS