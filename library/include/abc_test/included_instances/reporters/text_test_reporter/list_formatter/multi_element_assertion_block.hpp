#pragma once
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/multi_element_assertion_block.hpp"
#include "abc_test/internal/test_reports/multi_element_assertion_block.hpp"
_BEGIN_ABC_REPORTERS_NS

template <typename Assertion_Status>
struct multi_element_test_block_list_formatter_t
    : public list_formattable_t<
          reports::multi_element_assertion_block_t<Assertion_Status>,
          combined_enum_multi_element_assertion_block_fields_t,
          print_config_t>
    , public assertion_list_formatter_t<false, Assertion_Status>
    , public assertion_block_matcher_data_list_formatter_t
{
public:
    __constexpr virtual bool
        check_data(
            const combined_enum_multi_element_assertion_block_fields_t& _a_fid,
            const reports::multi_element_assertion_block_t<Assertion_Status>&
                _a_element
        ) const override;
    __constexpr virtual std::vector<std::string>
        get_data(
            const combined_enum_multi_element_assertion_block_fields_t& _a_fid,
            const reports::multi_element_assertion_block_t<Assertion_Status>&
                                  _a_element,
            const print_config_t& _a_pc
        ) const override;
protected:
    __constexpr virtual std::string
        get_str_representation(
            const reports::assertion_t<false, Assertion_Status>& _a_element,
            const print_config_t&                                _a_pc
        ) const override;
};

_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
template <typename Assertion_Status>
__constexpr_imp bool
    multi_element_test_block_list_formatter_t<Assertion_Status>::check_data(
        const combined_enum_multi_element_assertion_block_fields_t& _a_fid,
        const reports::multi_element_assertion_block_t<Assertion_Status>&
            _a_element
    ) const
{
    using namespace std;
    if (auto _l_ptr{get_if<enum_multi_element_assertion_block_fields_t>(&_a_fid)
        };
        _l_ptr != nullptr)
    {
        using enum enum_multi_element_assertion_block_fields_t;
        switch (*_l_ptr)
        {
        case SHOW_ALL_ASSERTIONS:
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
__constexpr_imp std::vector<std::string>
    multi_element_test_block_list_formatter_t<Assertion_Status>::get_data(
        const combined_enum_multi_element_assertion_block_fields_t& _a_fid,
        const reports::multi_element_assertion_block_t<Assertion_Status>&
                              _a_element,
        const print_config_t& _a_pc
    ) const
{
    using namespace std;
    if (auto _l_ptr{get_if<enum_multi_element_assertion_block_fields_t>(&_a_fid)
        };
        _l_ptr != nullptr)
    {
        using enum enum_multi_element_assertion_block_fields_t;
        switch (*_l_ptr)
        {
        case SHOW_ALL_ASSERTIONS:
        {
            vector<string> _l_rv;
            for (auto& _l_matcher : _a_element.get_matcher())
            {
                _l_rv.append_range(get_all_data(
                    _a_pc.matcher_assertion_block_assertion_list_fields(),
                    _l_matcher,
                    _a_pc,
                    assertion_block_matcher_data_list_formatter_t()
                ));
            }
            return _l_rv;
        }
        default:
            throw errors::unaccounted_for_enum_exception(*_l_ptr);
        }
    }
    else if (auto _l_ptr{get_if<combined_enum_assertion_fields_t>(&_a_fid)};
             _l_ptr != nullptr)
    {
        return assertion_list_formatter_t<false, Assertion_Status>::get_data(
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
                multi_element_test_block_list_formatter_t<Assertion_Status>::
        get_str_representation(
            const reports::assertion_t<false, Assertion_Status>& _a_element,
            const print_config_t&                                _a_pc
        ) const
{
    return construct_str_representation(
        _a_element, "Matcher-based assertion block"
    );
}

_END_ABC_REPORTERS_NS