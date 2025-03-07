#pragma once
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/matcher_based_assertion_block.hpp"
#include "abc_test/internal/test_reports/matcher_based_assertion_block.hpp"

_BEGIN_ABC_REPORTERS_NS

struct assertion_block_matcher_data_list_formatter_t
    : public list_formattable_t<
          _ABC_NS_MATCHER::bba_inner_assertion_type_t,
          enum_assertion_block_matcher_data_fields_t,
          print_config_t>
{
public:
    __constexpr
    assertion_block_matcher_data_list_formatter_t(
        const std::size_t _a_indent_offset = 0
    ) noexcept;
    __constexpr virtual bool
        check_data(
            const enum_assertion_block_matcher_data_fields_t&  _a_fid,
            const _ABC_NS_MATCHER::bba_inner_assertion_type_t& _a_element
        ) const override;
    __constexpr virtual std::vector<std::string>
        get_data(
            const enum_assertion_block_matcher_data_fields_t&  _a_fid,
            const _ABC_NS_MATCHER::bba_inner_assertion_type_t& _a_element,
            const print_config_t&                              _a_pc
        ) const override;
private:
    std::size_t _m_indent_offset;
};

_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__constexpr
assertion_block_matcher_data_list_formatter_t::
    assertion_block_matcher_data_list_formatter_t(
        const std::size_t _a_indent_offset
    ) noexcept
    : _m_indent_offset(_a_indent_offset)
{}

__constexpr bool
    assertion_block_matcher_data_list_formatter_t::check_data(
        const enum_assertion_block_matcher_data_fields_t&  _a_fid,
        const _ABC_NS_MATCHER::bba_inner_assertion_type_t& _a_element
    ) const
{
    using enum enum_assertion_block_matcher_data_fields_t;
    switch (_a_fid)
    {
    case MATCHER_ANNOTATION:
        return _a_element.annotation().has_value();
    case MATCHER_RESULT_STRING:
        return true;
    case MATCHER_SOURCE_MAP:
        return _a_element.source_map().has_elements();
    case MATCHER_MAIN_SOURCE_REP:
        return _a_element.source().has_value();
    case MATCHER_MAIN_SOURCE_LOCATION:
        return _a_element.source().has_value();
    default:
        throw errors::unaccounted_for_enum_exception(_a_fid);
    }
}

__constexpr std::vector<std::string>
            assertion_block_matcher_data_list_formatter_t::get_data(
        const enum_assertion_block_matcher_data_fields_t&  _a_fid,
        const _ABC_NS_MATCHER::bba_inner_assertion_type_t& _a_element,
        const print_config_t&                              _a_pc
    ) const
{
    using enum enum_assertion_block_matcher_data_fields_t;
    using enum _ABC_NS_MATCHER::enum_bba_inner_assertion_type_t;
    switch (_a_fid)
    {
    case MATCHER_ANNOTATION:
        switch (_a_element.assertion_type())
        {
        case MATCHER_BASED_ASSERTION:
        {
            return {
                _a_pc.indent(
                    _a_pc.colon(_a_pc.matcher_annotation()), _m_indent_offset
                ),
                _a_pc.indent(
                    _a_pc.message_str(_a_element.annotation()), _m_indent_offset + 1
                )
            };
        }
        case STATIC_ASSERTION:
        {
            return {
                _a_pc.indent(
                    _a_pc.colon(_a_pc.static_test_annotation_str()), _m_indent_offset
                ),
                _a_pc.indent(
                    _a_pc.message_str(_a_element.annotation()), _m_indent_offset + 1
                )
            };
        }
        default:
            return {};
        }
    case MATCHER_RESULT_STRING:
        switch (_a_element.assertion_type())
        {
        case MATCHER_BASED_ASSERTION:
        {
            const _ABC_NS_MATCHER::matcher_result_t& _l_matcher_result{
                _a_element.matcher_result()
            };
            return {
                _a_pc.indent(
                    _a_pc.matcher_output_str(_l_matcher_result.passed(),
                        _a_element.terminate()),
                    _m_indent_offset
                ),
                _a_pc.indent(
                    _a_pc.matcher_output(_l_matcher_result.str()),
                    _m_indent_offset + 1
                )
            };
        }
        case STATIC_ASSERTION:
        {
            const _ABC_NS_MATCHER::matcher_result_t& _l_matcher_result{
                _a_element.matcher_result()
            };
            return {
                _a_pc.indent(
                    _a_pc.static_assertion_output_str(_l_matcher_result.passed()),
                    _m_indent_offset
                )
            };
        }

        default:
            return {};
        }
    case MATCHER_SOURCE_MAP:
    {
        using namespace std;
        vector<string> _l_rv{_a_pc.indent(
            _a_pc.colon(_a_pc.matcher_source_map_str()), _m_indent_offset
        )};
        for (const pair<std::source_location, vector<string>>& _l_element :
             _a_element.source_map().map())
        {
            _l_rv.push_back(_a_pc.indent(
                _a_pc.colon(_a_pc.source_location_str()), _m_indent_offset + 1
            ));
            _l_rv.push_back(_a_pc.indent(
                _a_pc.source_location(_l_element.first), _m_indent_offset + 2
            ));
            _l_rv.push_back(_a_pc.indent(
                _a_pc.colon(_a_pc.source_code_str()), _m_indent_offset + 1
            ));
            for (const string_view _l_str : _l_element.second)
            {
                _l_rv.push_back(_a_pc.indent(
                    _a_pc.source_representation(_l_str), _m_indent_offset + 2
                ));
            }
        }
        return _l_rv;
    }
    case MATCHER_MAIN_SOURCE_REP:
        return {
            _a_pc.indent(
                _a_pc.colon(_a_pc.source_code_str()), _m_indent_offset
            ),
            _a_pc.indent(
                _a_pc.source_representation(
                    _a_element.source().value().source_code_representation()
                ),
                _m_indent_offset + 1
            )
        };
    case MATCHER_MAIN_SOURCE_LOCATION:
        return {
            _a_pc.indent(
                _a_pc.colon(_a_pc.source_location_str()), _m_indent_offset
            ),
            _a_pc.indent(
                _a_pc.source_location(
                    _a_element.source().value().source_location()
                ),
                _m_indent_offset + 1
            )
        };
    default:
        throw errors::unaccounted_for_enum_exception(_a_fid);
    }
}

_END_ABC_REPORTERS_NS