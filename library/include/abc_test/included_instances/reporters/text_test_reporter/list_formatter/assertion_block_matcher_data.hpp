#pragma once
#include "abc_test/core/test_reports/matcher_based_assertion_block.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/matcher_based_assertion_block.hpp"

_BEGIN_ABC_REPORTERS_NS

struct assertion_block_matcher_data_list_formatter_t
    : public list_formattable_t<
          _ABC_NS_MATCHER::bba_inner_assertion_type_t,
          enum_assertion_block_matcher_data_fields_t,
          print_config_t>
{
public:
    __constexpr
    assertion_block_matcher_data_list_formatter_t() noexcept
    {}

    __constexpr
    assertion_block_matcher_data_list_formatter_t(
        const std::size_t _a_index,
        const std::size_t _a_max_index,
        const std::size_t _a_global_indent,
        const bool        _a_use_indexes,
        const std::size_t _a_indent_offset = 0
    ) noexcept;
    __constexpr virtual bool
        check_data(
            const enum_assertion_block_matcher_data_fields_t&  _a_fid,
            const _ABC_NS_MATCHER::bba_inner_assertion_type_t& _a_element
        ) const override;
    __no_constexpr virtual void
        get_data(
            const enum_assertion_block_matcher_data_fields_t&   _a_fid,
            const _ABC_NS_MATCHER::bba_inner_assertion_type_t&  _a_element,
            const print_config_t&                               _a_pc,
            const utility::io::threated_text_output_reporter_t& _a_ttor,
            const std::size_t                                   _a_idx
        ) const override;
protected:
    std::size_t        _m_indent_offset;
    std::size_t        _m_index;
    std::size_t        _m_global_indent;
    std::size_t        _m_max_index;
    bool               _m_use_indexes;

    __no_constexpr_imp std::u8string
                       prefix(
                           const std::size_t _a_idx
                       ) const noexcept
    {
        using namespace std;
        u8string _l_max_str{fmt::format(u8"  {0})  ", _m_max_index)};
        if (_m_use_indexes)
        {
            if (_a_idx == 0)
            {
                u8string _l_s1{fmt::format(u8"  {0})", _m_index)};
                return fmt::format(
                    u8"{0}{1}{2}",
                    u8string(_m_global_indent, char8_t(' ')),
                    _l_s1,
                    u8string(_l_max_str.size() - _l_s1.size(), ' ')
                );
            }
            else
            {
                return fmt::format(
                    u8"{0}{1}",
                    u8string(_m_global_indent, char8_t(' ')),
                    u8string(_l_max_str.size(), char8_t(' '))
                );
            }
        }
        else
        {
            return fmt::format(u8"{0}", u8string(_m_global_indent, char8_t(' ')));
        }
    }
};

_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__constexpr
assertion_block_matcher_data_list_formatter_t::
    assertion_block_matcher_data_list_formatter_t(
        const std::size_t _a_index,
        const std::size_t _a_max_index,
        const std::size_t _a_global_indent,
        const bool        _a_use_indexes,
        const std::size_t _a_indent_offset
    ) noexcept
    : _m_indent_offset(_a_indent_offset)
    , _m_index(_a_index)
    , _m_max_index(_a_max_index)
    , _m_global_indent(_a_global_indent)
    , _m_use_indexes(_a_use_indexes)
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
    case LOG_INFOS:
        return _a_element.log_infos().size() > 0;
    default:
        throw errors::unaccounted_for_enum_exception(_a_fid);
    }
}

__no_constexpr_imp void
    assertion_block_matcher_data_list_formatter_t::get_data(
        const enum_assertion_block_matcher_data_fields_t&   _a_fid,
        const _ABC_NS_MATCHER::bba_inner_assertion_type_t&  _a_element,
        const print_config_t&                               _a_pc,
        const utility::io::threated_text_output_reporter_t& _a_ttor,
        const std::size_t                                   _a_idx
    ) const
{
    using enum enum_assertion_block_matcher_data_fields_t;
    using enum _ABC_NS_MATCHER::enum_bba_inner_assertion_type_t;
    using namespace std;
    pair<u8string, u8string> _l_pair;
    switch (_a_fid)
    {
    case MATCHER_ANNOTATION:
        switch (_a_element.assertion_type())
        {
        case MATCHER_BASED_ASSERTION:
        {
            _l_pair
                = {_a_pc.indent(
                       _a_pc.colon(_a_pc.matcher_annotation()), _m_indent_offset
                   ),
                   _a_pc.indent(
                       _a_pc.message_str(
                           _a_element.annotation()
                       ),
                       _m_indent_offset + 1
                   )};
        }
        break;
        case STATIC_ASSERTION:
        {
            _l_pair
                = {_a_pc.indent(
                       _a_pc.colon(_a_pc.static_test_annotation_str()),
                       _m_indent_offset
                   ),
                   _a_pc.indent(
                       _a_pc.message_str(
                           _a_element.annotation()
                       ),
                       _m_indent_offset + 1
                   )};
        }
        break;
        default:
            _l_pair = {};
        }
        break;
    case MATCHER_RESULT_STRING:
        switch (_a_element.assertion_type())
        {
        case MATCHER_BASED_ASSERTION:
        {
            const _ABC_NS_MATCHER::matcher_result_t& _l_matcher_result{
                _a_element.matcher_result()
            };
            _l_pair
                = {_a_pc.indent(
                       _a_pc.matcher_output_str(
                           _l_matcher_result.passed(), _a_element.terminate()
                       ),
                       _m_indent_offset
                   ),
                   _a_pc.indent(
                       _a_pc.matcher_output(
                           _l_matcher_result.str()
                       ),
                       _m_indent_offset + 1
                   )};
        }
        break;
        case STATIC_ASSERTION:
        {
            const _ABC_NS_MATCHER::matcher_result_t& _l_matcher_result{
                _a_element.matcher_result()
            };
            _l_pair.first = {_a_pc.indent(
                _a_pc.static_assertion_output_str(_l_matcher_result.passed()),
                _m_indent_offset
            )};
        }
        break;
        default:
            _l_pair = {};
        }
        break;
    case MATCHER_SOURCE_MAP:
    {
        using namespace std;
        _a_ttor.write(fmt::format(
            u8"{0}{1}",
            prefix(_a_idx),
            _a_pc.indent(
                _a_pc.colon(_a_pc.matcher_source_map_str()), _m_indent_offset
            )
        ));
        for (const pair<std::source_location, vector<string>>& _l_element :
             _a_element.source_map().map())
        {
            _a_ttor.write(fmt::format(
                u8"{0}{1}",
                prefix(_a_idx + 1),
                _a_pc.indent(
                    _a_pc.colon(_a_pc.source_location_str()),
                    _m_indent_offset + 1
                )
            ));
            _a_ttor.write(fmt::format(
                u8"{0}{1}",
                prefix(_a_idx + 1),
                _a_pc.indent(
                    _a_pc.source_location(_l_element.first),
                    _m_indent_offset + 1
                )
            ));
            _a_ttor.write(fmt::format(
                u8"{0}{1}",
                prefix(_a_idx + 1),
                _a_pc.indent(
                    _a_pc.colon(_a_pc.source_code_str()), _m_indent_offset + 1
                )
            ));
            for (const string_view _l_str : _l_element.second)
            {
                _a_ttor.write(fmt::format(
                    u8"{0}{1}",
                    prefix(_a_idx + 1),
                    _a_pc.indent(
                        _a_pc.source_representation(
                            checkless_convert_ascii_to_unicode_string<std::u8string>(_l_str)
                        ),
                        _m_indent_offset + 2
                    )
                ));
            }
        }
    }
        return;
    case MATCHER_MAIN_SOURCE_REP:
        _l_pair
            = {_a_pc.indent(
                   _a_pc.colon(_a_pc.source_code_str()), _m_indent_offset
               ),
               _a_pc.indent(
                   _a_pc.source_representation(checkless_convert_ascii_to_unicode_string<std::u8string>(
                       _a_element.source().value().source_code_representation()
                   )),
                   _m_indent_offset + 1
               )};
        break;
    case LOG_INFOS:
        _a_ttor.write(fmt::format(
            u8"{0}{1}", prefix(_a_idx), _a_pc.colon(_a_pc.log_info_str())
        ));
        for (const u8string_view& _l_str : _a_element.log_infos())
        {
            _a_ttor.write(fmt::format(
                u8"{0}{1}",
                prefix(_a_idx),
                _a_pc.indent(_a_pc.log_info(_l_str))
            ));
        }
        return;
    case MATCHER_MAIN_SOURCE_LOCATION:
        _l_pair
            = {_a_pc.indent(
                   _a_pc.colon(_a_pc.source_location_str()), _m_indent_offset
               ),
               _a_pc.indent(
                   _a_pc.source_location(
                       _a_element.source().value().source_location()
                   ),
                   _m_indent_offset + 1
               )};
        break;
    default:
        throw errors::unaccounted_for_enum_exception(_a_fid);
    }
    _a_ttor.write(fmt::format(
        u8"{0}{1}\n{3}{2}",
        prefix(_a_idx),
        _l_pair.first,
        _l_pair.second,
        prefix(_a_idx + 1)
    ));
}

_END_ABC_REPORTERS_NS