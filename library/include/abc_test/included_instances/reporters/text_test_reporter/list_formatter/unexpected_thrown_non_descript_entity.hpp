#pragma once
#include "abc_test/core/test_reports/unexpected_thrown_non_descript_entity.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/unexpected_thrown_non_descript_entity.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/list_formatter/unexpected_report.hpp"

_BEGIN_ABC_REPORTERS_NS

struct unexpected_thrown_non_descript_entity_list_formatter_t
    : public list_formattable_t<
          reports::unexpected_thrown_not_derived_from_std_exception_t,
          combined_enum_unexpected_thrown_non_descript_entity_fields_t,
          print_config_t>
    , public unexpected_report_list_formatter_t<true>
{
public:
    __constexpr virtual bool
        check_data(
            const combined_enum_unexpected_thrown_non_descript_entity_fields_t&
                _a_fid,
            const reports::unexpected_thrown_not_derived_from_std_exception_t&
                _a_element
        ) const;
    __constexpr virtual void
        get_data(
            const combined_enum_unexpected_thrown_non_descript_entity_fields_t&
                _a_fid,
            const reports::unexpected_thrown_not_derived_from_std_exception_t&
                                                                _a_element,
            const print_config_t&                               _a_pc,
            const utility::io::threated_text_output_reporter_t& _a_ttor,
            const std::size_t _a_idx
        ) const;
protected:
    __constexpr virtual std::string
        get_str_representation(
            const reports::unexpected_report_t<true>& _a_element,
            const print_config_t&                     _a_pc
        ) const;
};

_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__constexpr_imp bool
    unexpected_thrown_non_descript_entity_list_formatter_t::check_data(
        const combined_enum_unexpected_thrown_non_descript_entity_fields_t&
            _a_fid,
        const reports::unexpected_thrown_not_derived_from_std_exception_t&
            _a_element
    ) const
{
    using namespace std;
    if (auto _l_ptr{get_if<enum_unexpected_report_fields_t>(&_a_fid)};
        _l_ptr != nullptr)
    {
        return unexpected_report_list_formatter_t<true>::check_data(
            *_l_ptr, _a_element
        );
    }
    else if (auto _l_ptr{
                 get_if<enum_unexpected_thrown_non_descript_entity_fields_t>(
                     &_a_fid
                 )
             };
             _l_ptr != nullptr)
    {
        using enum enum_unexpected_thrown_non_descript_entity_fields_t;
        switch (*_l_ptr)
        {
        default:
            throw errors::unaccounted_for_enum_exception(*_l_ptr);
        }
    }
    else
    {
        throw errors::unaccounted_for_variant_exception(_a_fid);
    }
}

__constexpr_imp void
    unexpected_thrown_non_descript_entity_list_formatter_t::get_data(
        const combined_enum_unexpected_thrown_non_descript_entity_fields_t&
            _a_fid,
        const reports::unexpected_thrown_not_derived_from_std_exception_t&
                                                            _a_element,
        const print_config_t&                               _a_pc,
        const utility::io::threated_text_output_reporter_t& _a_ttor,
        const std::size_t _a_idx
    ) const
{
    using namespace std;
    if (auto _l_ptr{get_if<enum_unexpected_report_fields_t>(&_a_fid)};
        _l_ptr != nullptr)
    {
        return unexpected_report_list_formatter_t<true>::get_data(
            *_l_ptr, _a_element, _a_pc, _a_ttor, _a_idx
        );
    }
    else if (auto _l_ptr{
                 get_if<enum_unexpected_thrown_non_descript_entity_fields_t>(
                     &_a_fid
                 )
             };
             _l_ptr != nullptr)
    {
        using enum enum_assertion_fields_t;
        switch (*_l_ptr)
        {
        default:
            throw errors::unaccounted_for_enum_exception(*_l_ptr);
        }
    }
    else
    {
        throw errors::unaccounted_for_variant_exception(_a_fid);
    }
}

__constexpr_imp std::string
                unexpected_thrown_non_descript_entity_list_formatter_t::
        get_str_representation(
            const reports::unexpected_report_t<true>& _a_element,
            const print_config_t&                     _a_pc
        ) const
{
    return "Unexpected thrown entity encountered (not an object derived from "
           "std::exception). Terminating function.";
}

_END_ABC_REPORTERS_NS