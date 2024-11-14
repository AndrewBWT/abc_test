#pragma once
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/matcher_based_assertion.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/list_formatter/assertion.hpp"
#include "abc_test/internal/test_reports/assertion_status/pass_or_terminate.hpp"
#include "abc_test/internal/test_reports/matcher_based_assertion.hpp"
// #include
// "abc_test/included_instances/reporters/text_test_reporter/enum_fields/generic_matcher_based_assertion.hpp"
_BEGIN_ABC_REPORTERS_NS

template <bool Single_Source, typename Assertion_Status>
struct matcher_based_assertion_list_formatter_t
    : public list_formattable_t<
          reports::matcher_based_assertion_t<Single_Source, Assertion_Status>,
          combined_enum_matcher_based_assertion_fields_t,
          print_config_t>
    , public assertion_list_formatter_t<Single_Source, Assertion_Status>
{
public:
    __constexpr virtual bool
        check_data(
            const combined_enum_matcher_based_assertion_fields_t& _a_fid,
            const reports::matcher_based_assertion_t<
                Single_Source,
                Assertion_Status>& _a_element
        ) const;
    __constexpr virtual std::vector<std::string>
        get_data(
            const combined_enum_matcher_based_assertion_fields_t& _a_fid,
            const reports::matcher_based_assertion_t<
                Single_Source,
                Assertion_Status>& _a_element,
            const print_config_t&  _a_pc
        ) const;
    /*__constexpr
        virtual
        std::string
        get_str_representation(
            const reports::generic_assertion_t<true, Assertion_Status>&
       _a_element, const print_config_t& _a_pc ) const override;*/
};

_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
template <bool Single_Source, typename Assertion_Status>
__constexpr_imp bool
    matcher_based_assertion_list_formatter_t<Single_Source, Assertion_Status>::
        check_data(
            const combined_enum_matcher_based_assertion_fields_t& _a_fid,
            const reports::matcher_based_assertion_t<
                Single_Source,
                Assertion_Status>& _a_element
        ) const
{
    using namespace std;
    if (auto _l_ptr{get_if<combined_enum_assertion_fields_t>(&_a_fid)};
        _l_ptr != nullptr)
    {
        return assertion_list_formatter_t<Single_Source, Assertion_Status>::
            check_data(*_l_ptr, _a_element);
    }
    else if (auto _l_ptr{get_if<enum_matcher_based_assertion_fields_t>(&_a_fid)
             };
             _l_ptr != nullptr)
    {
        using enum enum_matcher_based_assertion_fields_t;
        switch (*_l_ptr)
        {
        /*case MATCHER_OUTPUT:
            return true;
        case MATCHER_SOURCE_MAP:
            if constexpr (Single_Source)
            {
                return get<2>(_a_element.matcher_result()).has_elements();
            }
            else
            {
                return true;
            }
        case MATCHER_ANNOTATION:
            if constexpr (Single_Source)
            {
                return get<1>(_a_element.matcher_result()).has_value();
            }
            else
            {
                return true;
            }*/
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
__constexpr_imp std::vector<std::string>
    matcher_based_assertion_list_formatter_t<Single_Source, Assertion_Status>::
        get_data(
            const combined_enum_matcher_based_assertion_fields_t& _a_fid,
            const reports::matcher_based_assertion_t<
                Single_Source,
                Assertion_Status>& _a_element,
            const print_config_t&  _a_pc
        ) const
{
    using namespace std;
    if (auto _l_ptr{get_if<combined_enum_assertion_fields_t>(&_a_fid)};
        _l_ptr != nullptr)
    {
        return assertion_list_formatter_t<Single_Source, Assertion_Status>::
            get_data(*_l_ptr, _a_element, _a_pc);
    }
    else if (auto _l_ptr{get_if<enum_matcher_based_assertion_fields_t>(&_a_fid)
             };
             _l_ptr != nullptr)
    {
        using enum enum_matcher_based_assertion_fields_t;
        switch (*_l_ptr)
        {
        /*case MATCHER_OUTPUT:
            if constexpr (Single_Source)
            {
                return {
                    _a_pc.colon(_a_pc.matcher_output_str()),
                    _a_pc.indent(_a_pc.matcher_output(
                        get<0>(_a_element.matcher_result()).str()
                    ))
                };
            }
            else
            {
                return {};
            }
        case MATCHER_SOURCE_MAP:
        {
            if constexpr (Single_Source)
            {
                vector<string> _l_rv{ _a_pc.colon(_a_pc.matcher_source_map_str()) };
                for (const pair<std::source_location, vector<string>>& _l_element :
                    get<2>(_a_element.matcher_result()).map())
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
            else
            {
                return {};
            }
        }
        case MATCHER_ANNOTATION:
            if constexpr (Single_Source)
            {
                return {
                    _a_pc.colon(_a_pc.matcher_annotation()),
                    _a_pc.indent(
                        _a_pc.message_str(get<1>(_a_element.matcher_result()))
                    )
                };
            }
            else
            {
                return {};
            }*/
        default:
            throw errors::unaccounted_for_enum_exception(*_l_ptr);
        }
    }
    else
    {
        throw errors::unaccounted_for_variant_exception(_a_fid);
    }
}

/*template<
    typename Assertion_Status
>
__constexpr_imp
std::string
assertion_list_formatter_t<Assertion_Status>::get_str_representation(
    const reports::generic_assertion_t<true, Assertion_Status>& _a_element,
    const print_config_t& _a_pc
) const
{
    using namespace std;
    using namespace reports;
    return fmt::format("Matcher-based assertion {0}.{1}",
        _a_element.get_pass_status() ? "passed" : "failed",
        (not _a_element.get_pass_status() &&
            same_as<Assertion_Status, pass_or_terminate_t>) ?
        " Assertion terminated function." : "");
}*/
_END_ABC_REPORTERS_NS