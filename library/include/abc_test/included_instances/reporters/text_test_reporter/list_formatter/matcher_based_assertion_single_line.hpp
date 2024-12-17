#pragma once
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/matcher_based_assertion_single_line.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/list_formatter/assertion.hpp"
#include "abc_test/internal/test_reports/matcher_based_assertion_single_line.hpp"
_BEGIN_ABC_REPORTERS_NS

template <typename Assertion_Status>
struct matcher_based_assertion_single_line_list_formatter_t
    : public list_formattable_t<
          reports::matcher_based_assertion_single_line_t<Assertion_Status>,
          combined_enum_matcher_based_assertion_single_line_fields_t,
          print_config_t>
    , public assertion_list_formatter_t<true, Assertion_Status>
{
public:
    __constexpr virtual bool
        check_data(
            const combined_enum_matcher_based_assertion_single_line_fields_t&
                _a_fid,
            const reports::matcher_based_assertion_single_line_t<
                Assertion_Status>& _a_element
        ) const override;
    __constexpr virtual std::vector<std::string>
        get_data(
            const combined_enum_matcher_based_assertion_single_line_fields_t&
                _a_fid,
            const reports::matcher_based_assertion_single_line_t<
                Assertion_Status>& _a_element,
            const print_config_t&  _a_pc
        ) const override;
protected:
    __constexpr virtual std::string
        get_str_representation(
            const reports::assertion_t<true, Assertion_Status>& _a_element,
            const print_config_t&                               _a_pc
        ) const override;
};

_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
template <typename Assertion_Status>
__constexpr_imp bool
    matcher_based_assertion_single_line_list_formatter_t<Assertion_Status>::
        check_data(
            const combined_enum_matcher_based_assertion_single_line_fields_t&
                _a_fid,
            const reports::matcher_based_assertion_single_line_t<
                Assertion_Status>& _a_element
        ) const
{
    using namespace std;
    if (auto _l_ptr{
            get_if<enum_matcher_based_assertion_single_line_fields_t>(&_a_fid)
        };
        _l_ptr != nullptr)
    {
        using enum enum_matcher_based_assertion_single_line_fields_t;
        switch (*_l_ptr)
        {
        case MATCHER_SOURCE_MAP:
            return _a_element.matcher_info().source_map().has_elements();
        case MATCHER_OUTPUT:
            return true;
        case MATCHER_ANNOTATION:
            return _a_element.matcher_info().annotation().has_value();
        default:
            throw errors::unaccounted_for_enum_exception(*_l_ptr);
        }
    }
    else if (auto _l_ptr{get_if<combined_enum_assertion_fields_t>(&_a_fid)};
             _l_ptr != nullptr)
    {
        return assertion_list_formatter_t<true, Assertion_Status>::check_data(
            *_l_ptr, _a_element
        );
    }
    else
    {
        throw errors::unaccounted_for_variant_exception(_a_fid);
    }
}

template <typename Assertion_Status>
__constexpr_imp std::vector<std::string>
    matcher_based_assertion_single_line_list_formatter_t<Assertion_Status>::
        get_data(
            const combined_enum_matcher_based_assertion_single_line_fields_t&
                _a_fid,
            const reports::matcher_based_assertion_single_line_t<
                Assertion_Status>& _a_element,
            const print_config_t&  _a_pc
        ) const
{
    using namespace std;
    if (auto _l_ptr{
            get_if<enum_matcher_based_assertion_single_line_fields_t>(&_a_fid)
        };
        _l_ptr != nullptr)
    {
        using enum enum_matcher_based_assertion_single_line_fields_t;
        switch (*_l_ptr)
        {
        case MATCHER_SOURCE_MAP:
        {
            vector<string> _l_rv{_a_pc.colon(_a_pc.matcher_source_map_str())};
            for (const pair<std::source_location, vector<string>>& _l_element :
                 _a_element.matcher_info().source_map().map())
            {
                _l_rv.push_back(
                    _a_pc.indent(_a_pc.colon(_a_pc.source_location_str()))
                );
                _l_rv.push_back(
                    _a_pc.indent(_a_pc.source_location(_l_element.first), 2)
                );
                _l_rv.push_back(_a_pc.indent(_a_pc.colon(_a_pc.source_code_str()
                )));
                for (const string_view _l_str : _l_element.second)
                {
                    _l_rv.push_back(
                        _a_pc.indent(_a_pc.source_representation(_l_str), 2)
                    );
                }
            }
            return _l_rv;
        }
        case MATCHER_OUTPUT:
            return {
                _a_pc.matcher_output_str(_a_element.get_pass_status()),
                _a_pc.indent(_a_pc.matcher_output(
                    _a_element.matcher_info().matcher_result().str()
                ))
            };
        case MATCHER_ANNOTATION:
            return {
                _a_pc.colon(_a_pc.matcher_annotation()),
                _a_pc.indent(
                    _a_pc.message_str(_a_element.matcher_info().annotation())
                )
            };
        default:
            throw errors::unaccounted_for_enum_exception(*_l_ptr);
        }
    }
    else if (auto _l_ptr{get_if<combined_enum_assertion_fields_t>(&_a_fid)};
             _l_ptr != nullptr)
    {
        return assertion_list_formatter_t<true, Assertion_Status>::get_data(
            *_l_ptr, _a_element, _a_pc
        );
    }
    else
    {
        throw errors::unaccounted_for_variant_exception(_a_fid);
    }
}

template <typename Assertion_Status>
__constexpr_imp std::string
    matcher_based_assertion_single_line_list_formatter_t<Assertion_Status>::
        get_str_representation(
            const reports::assertion_t<true, Assertion_Status>& _a_element,
            const print_config_t&                               _a_pc
        ) const
{
    return construct_str_representation(_a_element, "Single-line assertion");
}

_END_ABC_REPORTERS_NS