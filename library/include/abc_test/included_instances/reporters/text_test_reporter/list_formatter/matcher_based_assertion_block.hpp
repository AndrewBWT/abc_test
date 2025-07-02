#pragma once
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/matcher_based_assertion_block.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/list_formatter/assertion_block_matcher_data.hpp"
#include "abc_test/core/test_reports/matcher_based_assertion_block.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/list_formatter/assertion.hpp"
_BEGIN_ABC_REPORTERS_NS

template <typename Assertion_Status>
struct matcher_based_assertion_block_list_formatter_t
    : public list_formattable_t<
          reports::matcher_based_assertion_block_t<Assertion_Status>,
          combined_enum_matcher_based_assertion_block_fields_t,
          print_config_t>
    , public assertion_list_formatter_t<false, Assertion_Status>
    , public assertion_block_matcher_data_list_formatter_t
{
public:
    using assertion_list_formatter_t<false, Assertion_Status>::assertion_list_formatter_t;
    using list_formattable_t<
        reports::matcher_based_assertion_block_t<Assertion_Status>,
        combined_enum_matcher_based_assertion_block_fields_t,
        print_config_t>::check_data;
    using assertion_list_formatter_t<false, Assertion_Status>::check_data;
    using assertion_block_matcher_data_list_formatter_t::check_data;
    __constexpr virtual bool
        check_data(
            const combined_enum_matcher_based_assertion_block_fields_t& _a_fid,
            const reports::matcher_based_assertion_block_t<Assertion_Status>&
                _a_element
        ) const override;
    using list_formattable_t<
        reports::matcher_based_assertion_block_t<Assertion_Status>,
        combined_enum_matcher_based_assertion_block_fields_t,
        print_config_t>::get_data;
    using assertion_list_formatter_t<false, Assertion_Status>::get_data;
    using assertion_block_matcher_data_list_formatter_t::get_data;
    __constexpr virtual void
        get_data(
            const combined_enum_matcher_based_assertion_block_fields_t& _a_fid,
            const reports::matcher_based_assertion_block_t<Assertion_Status>&
                                  _a_element,
            const print_config_t& _a_pc,
            const utility::io::threated_text_output_reporter_t& _a_ttor,
            const std::size_t _a_idx
        ) const override;
protected:
    __constexpr virtual std::u8string
        get_str_representation(
            const reports::assertion_t<false, Assertion_Status>& _a_element,
            const print_config_t&                                _a_pc
        ) const override;
};

_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
template <typename Assertion_Status>
__constexpr_imp bool
    matcher_based_assertion_block_list_formatter_t<Assertion_Status>::
        check_data(
            const combined_enum_matcher_based_assertion_block_fields_t& _a_fid,
            const reports::matcher_based_assertion_block_t<Assertion_Status>&
                _a_element
        ) const
{
    using namespace std;
    if (auto _l_ptr{get_if<enum_single_element_assertion_block_fields_t>(&_a_fid
        )};
        _l_ptr != nullptr)
    {
        using enum enum_single_element_assertion_block_fields_t;
        switch (*_l_ptr)
        {
        case SHOW_ASSERTION_DATA:
            return true;
        default:
            throw errors::unaccounted_for_enum_exception(*_l_ptr);
        }
    }
    else if (auto _l_ptr{get_if<combined_enum_assertion_fields_t>(&_a_fid)};
             _l_ptr != nullptr)
    {
        return assertion_list_formatter_t<false, Assertion_Status>::check_data(
            *_l_ptr, _a_element
        );
    }
    else
    {
        throw errors::unaccounted_for_variant_exception(_a_fid);
    }
}

template <typename Assertion_Status>
__constexpr_imp void
    matcher_based_assertion_block_list_formatter_t<Assertion_Status>::get_data(
        const combined_enum_matcher_based_assertion_block_fields_t& _a_fid,
        const reports::matcher_based_assertion_block_t<Assertion_Status>&
                              _a_element,
        const print_config_t& _a_pc,
        const utility::io::threated_text_output_reporter_t& _a_ttor,
        const std::size_t _a_idx
    ) const
{
    using namespace std;
    if (auto _l_ptr{get_if<enum_single_element_assertion_block_fields_t>(&_a_fid
        )};
        _l_ptr != nullptr)
    {
        using enum enum_single_element_assertion_block_fields_t;
        switch (*_l_ptr)
        {
        case SHOW_ASSERTION_DATA:
        {
            vector<string> _l_rv;
            _l_rv.push_back("Matcher's data:");
            assertion_block_matcher_data_list_formatter_t(1,1,1,true).process_all_data(
                _a_pc.matcher_assertion_single_block_assertion_list_fields(),
                _a_element.get_matcher(),
                _a_pc,
                _a_ttor
            );
        }
        default:
            throw errors::unaccounted_for_enum_exception(*_l_ptr);
        }
    }
    else if (auto _l_ptr{get_if<combined_enum_assertion_fields_t>(&_a_fid)};
             _l_ptr != nullptr)
    {
        return assertion_list_formatter_t<false, Assertion_Status>::get_data(
            *_l_ptr, _a_element, _a_pc, _a_ttor, _a_idx
        );
    }
    else
    {
        throw errors::unaccounted_for_variant_exception(_a_fid);
    }
}

template <typename Assertion_Status>
__constexpr_imp std::u8string
    matcher_based_assertion_block_list_formatter_t<Assertion_Status>::
        get_str_representation(
            const reports::assertion_t<false, Assertion_Status>& _a_element,
            const print_config_t&                                _a_pc
        ) const
{
    return construct_str_representation(
        _a_element, u8"Single-element block-based assertion"
    );
}

_END_ABC_REPORTERS_NS