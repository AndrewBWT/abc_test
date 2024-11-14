#pragma once
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/matcher_based_assertion_block.hpp"
#include "abc_test/internal/test_reports/matcher_based_assertion_block.hpp"
_BEGIN_ABC_REPORTERS_NS

struct matcher_based_assertion_block_assertion_list_list_formatter_t
    : public list_formattable_t<
          _ABC_NS_MATCHER::matcher_res_info_t,
          enum_matcher_assertion_block_assertion_fields_t,
          print_config_t>
{
public:
    __constexpr virtual bool
        check_data(
            const enum_matcher_assertion_block_assertion_fields_t& _a_fid,
            const _ABC_NS_MATCHER::matcher_res_info_t&             _a_element
        ) const override;
    __constexpr virtual std::vector<std::string>
        get_data(
            const enum_matcher_assertion_block_assertion_fields_t& _a_fid,
            const _ABC_NS_MATCHER::matcher_res_info_t&             _a_element,
            const print_config_t&                                  _a_pc
        ) const override;
};

template <typename Assertion_Status>
struct matcher_based_assertion_block_list_formatter_t
    : public list_formattable_t<
          reports::matcher_based_assertion_block_t<Assertion_Status>,
          combined_enum_matcher_based_assertion_block_fields_t,
          print_config_t>
    , public assertion_list_formatter_t<false, Assertion_Status>
{
public:
    __constexpr virtual bool
        check_data(
            const combined_enum_matcher_based_assertion_block_fields_t& _a_fid,
            const reports::matcher_based_assertion_block_t<Assertion_Status>&
                _a_element
        ) const override;
    __constexpr virtual std::vector<std::string>
        get_data(
            const combined_enum_matcher_based_assertion_block_fields_t& _a_fid,
            const reports::matcher_based_assertion_block_t<Assertion_Status>&
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
    matcher_based_assertion_block_list_formatter_t<Assertion_Status>::
        check_data(
            const combined_enum_matcher_based_assertion_block_fields_t& _a_fid,
            const reports::matcher_based_assertion_block_t<Assertion_Status>&
                _a_element
        ) const
{
    using namespace std;
    if (auto _l_ptr{get_if<enum_matcher_based_assertion_block_fields_t>(&_a_fid)
        };
        _l_ptr != nullptr)
    {
        using enum enum_matcher_based_assertion_block_fields_t;
        switch (*_l_ptr)
        {
        case SHOW_ALL_ASSERTIONS:
            return (_a_element.get_matchers()).size() > 0;
        default:
            throw errors::unaccounted_for_enum_exception(*_l_ptr);
        }
    }
    else if (auto _l_ptr{
                 get_if<combined_enum_assertion_fields_t>(&_a_fid)
             };
             _l_ptr != nullptr)
    {
        return assertion_list_formatter_t<
            false,
            Assertion_Status>::check_data(*_l_ptr, _a_element);
    }
    else
    {
        throw errors::unaccounted_for_variant_exception(_a_fid);
    }
}

template <typename Assertion_Status>
__constexpr_imp std::vector<std::string>
    matcher_based_assertion_block_list_formatter_t<Assertion_Status>::get_data(
        const combined_enum_matcher_based_assertion_block_fields_t& _a_fid,
        const reports::matcher_based_assertion_block_t<Assertion_Status>&
                              _a_element,
        const print_config_t& _a_pc
    ) const
{
    using namespace std;
    if (auto _l_ptr{get_if<enum_matcher_based_assertion_block_fields_t>(&_a_fid)
        };
        _l_ptr != nullptr)
    {
        using enum enum_matcher_based_assertion_block_fields_t;
        switch (*_l_ptr)
        {
        case SHOW_ALL_ASSERTIONS:
        {
            using namespace _ABC_NS_MATCHER;
            vector<string> _l_rv;
            for (const matcher_res_info_t& _l_matcher_info : _a_element.get_matchers())
            {
                _l_rv.append_range(get_all_data(
                    _a_pc._m_matcher_assertion_block_assertion_list_fields,
                    _l_matcher_info,
                    _a_pc,
                    matcher_based_assertion_block_assertion_list_list_formatter_t{
                    }
                ));
            }
            return _l_rv;
        }
        default:
            throw errors::unaccounted_for_enum_exception(*_l_ptr);
        }
    }
    else if (auto _l_ptr{
                 get_if<combined_enum_assertion_fields_t>(&_a_fid)
             };
             _l_ptr != nullptr)
    {
        return assertion_list_formatter_t<
            false,
            Assertion_Status>::get_data(*_l_ptr, _a_element, _a_pc);
    }
    else
    {
        throw errors::unaccounted_for_variant_exception(_a_fid);
    }
}

template <typename Assertion_Status>
__constexpr_imp std::string
    matcher_based_assertion_block_list_formatter_t<Assertion_Status>::
        get_str_representation(
            const reports::assertion_t<false, Assertion_Status>& _a_element,
            const print_config_t&                                _a_pc
        ) const
{
    return construct_str_representation(
        _a_element, "Matcher-based assertion block"
    );
}

__constexpr bool
    matcher_based_assertion_block_assertion_list_list_formatter_t::check_data(
        const enum_matcher_assertion_block_assertion_fields_t& _a_fid,
        const _ABC_NS_MATCHER::matcher_res_info_t&             _a_element
    ) const
{
    using enum enum_matcher_assertion_block_assertion_fields_t;
    switch (_a_fid)
    {
    case MATCHER_ANNOTATION:
        return get<1>(_a_element).has_value();
    case MATCHER_RESULT_STRING:
        return true;
    case MATCHER_SOURCE_MAP:
        return get<2>(_a_element).has_elements();
    default:
        throw errors::unaccounted_for_enum_exception(_a_fid);
    }
}

__constexpr std::vector<std::string>
    matcher_based_assertion_block_assertion_list_list_formatter_t::get_data(
        const enum_matcher_assertion_block_assertion_fields_t& _a_fid,
        const _ABC_NS_MATCHER::matcher_res_info_t&             _a_element,
        const print_config_t&                                  _a_pc
    ) const
{
    using enum enum_matcher_assertion_block_assertion_fields_t;
    switch (_a_fid)
    {
    case MATCHER_ANNOTATION:
        return {
            _a_pc.indent(_a_pc.matcher_annotation()),
            _a_pc.indent(_a_pc.message_str(get<1>(_a_element)))
        };
    case MATCHER_RESULT_STRING:
        return {
            _a_pc.colon(_a_pc.matcher_output_str()),
            _a_pc.indent(_a_pc.matcher_output(get<0>(_a_element).str()))
        };
    case MATCHER_SOURCE_MAP:
    {
        using namespace std;
        vector<string> _l_rv{_a_pc.colon(_a_pc.matcher_source_map_str())};
        for (const pair<std::source_location, vector<string>>& _l_element :
             get<2>(_a_element).map())
        {
            _l_rv.push_back(_a_pc.indent(_a_pc.colon(_a_pc.source_location_str()
            )));
            _l_rv.push_back(
                _a_pc.indent(_a_pc.source_location(_l_element.first), 2)
            );
            _l_rv.push_back(_a_pc.indent(_a_pc.colon(_a_pc.source_code_str())));
            for (const string_view _l_str : _l_element.second)
            {
                _l_rv.push_back(
                    _a_pc.indent(_a_pc.source_representation(_l_str), 2)
                );
            }
        }
        return _l_rv;
    }
    default:
        throw errors::unaccounted_for_enum_exception(_a_fid);
    }
}

_END_ABC_REPORTERS_NS