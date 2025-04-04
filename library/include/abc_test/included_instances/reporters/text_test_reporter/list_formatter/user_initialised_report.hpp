#pragma once
#include "abc_test/core/test_reports/user_initialised_report.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/user_initialised_report.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/list_formatter.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/print_config.hpp"
#include "abc_test/utility/internal/macros.hpp"
_BEGIN_ABC_REPORTERS_NS

template <bool Single_Source>
struct user_initialised_report_list_formatter_t
    : public list_formattable_t<
          reports::user_initialised_report_t<Single_Source>,
          enum_user_initialised_report_fields_t,
          print_config_t>
{
public:
    __constexpr
    user_initialised_report_list_formatter_t(
        const std::size_t _a_index,
        const std::size_t _a_max_index
    ) noexcept;
    __constexpr virtual bool
        check_data(
            const enum_user_initialised_report_fields_t&             _a_fid,
            const reports::user_initialised_report_t<Single_Source>& _a_element
        ) const;
    __constexpr virtual void
        get_data(
            const enum_user_initialised_report_fields_t&             _a_fid,
            const reports::user_initialised_report_t<Single_Source>& _a_element,
            const print_config_t&                                    _a_pc,
            const utility::io::threated_text_output_reporter_t&      _a_ttor,
            const std::size_t                                        _a_idx
        ) const;
protected:
    std::size_t _m_index;
    std::size_t _m_max_index;
    std::size_t _m_max_str_size;
public:
    __constexpr std::u8string
                prefix(
                    const std::size_t _a_idx
                ) const noexcept
    {
        using namespace std;
        u8string _l_max_str{fmt::format(u8"  {0})  ", _m_max_index)};
        if (_a_idx == 0)
        {
            u8string _l_s1{fmt::format(u8"  {0})", _m_index)};
            return fmt::format(
                u8"{0}{1}",
                _l_s1,
                u8string(_l_max_str.size() - _l_s1.size(), char8_t(' '))
            );
        }
        else
        {
            return fmt::format(
                u8"{0}", u8string(_l_max_str.size(), char8_t(' '))
            );
        }
    }
};

_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
template <bool Single_Source>
__constexpr_imp
    user_initialised_report_list_formatter_t<Single_Source>::
        user_initialised_report_list_formatter_t(
            const std::size_t _a_index,
            const std::size_t _a_max_index
        ) noexcept
    : _m_index(_a_index)
    , _m_max_index(_a_max_index)
    , _m_max_str_size(fmt::format("  {0})  ", _a_max_index).size())
{}

template <bool Single_Source>
__constexpr_imp bool
    user_initialised_report_list_formatter_t<Single_Source>::check_data(
        const enum_user_initialised_report_fields_t&             _a_fid,
        const reports::user_initialised_report_t<Single_Source>& _a_element
    ) const
{
    using namespace errors;
    using enum enum_user_initialised_report_fields_t;
    switch (_a_fid)
    {
    case SOURCE:
        // case STR_REPRESENTATION:
        return true;
    case LOG_INFOS:
        return _a_element.log_infos().size() > 0;
    default:
        throw unaccounted_for_enum_exception(_a_fid);
    }
}

template <bool Single_Source>
__constexpr_imp void
    user_initialised_report_list_formatter_t<Single_Source>::get_data(
        const enum_user_initialised_report_fields_t&             _a_fid,
        const reports::user_initialised_report_t<Single_Source>& _a_element,
        const print_config_t&                                    _a_pc,
        const utility::io::threated_text_output_reporter_t&      _a_ttor,
        const std::size_t                                        _a_idx
    ) const
{
    using namespace std;
    using namespace errors;
    using enum enum_user_initialised_report_fields_t;
    switch (_a_fid)
    {
    // case STR_REPRESENTATION:
    //	return
    //	{
    //		str_representation()
    //	};
    case SOURCE:
        if constexpr (Single_Source)
        {
            _a_ttor.write(fmt::format(
                u8"{0}{1}",
                prefix(_a_idx),
                _a_pc.colon(_a_pc.source_location_str())
            ));
            _a_ttor.write(fmt::format(
                u8"{0}{1}",
                prefix(_a_idx),
                _a_pc.indent(
                    _a_pc.source_location(_a_element.source().source_location())
                )
            ));
            _a_ttor.write(fmt::format(
                u8"{0}{1}", prefix(_a_idx), _a_pc.colon(_a_pc.source_code_str())
            ));
            _a_ttor.write(fmt::format(
                u8"{0}{1}",
                prefix(_a_idx),
                _a_pc.indent(_a_pc.source_representation(
                    convert_string_to_u8string(
                        _a_element.source().source_code_representation()
                    ).value()

                ))
            ));
        }
        else
        {
            _a_ttor.write(fmt::format(
                u8"{0}{1}",
                prefix(_a_idx),
                _a_pc.colon(_a_pc.source_location_pair_begin_str())
            ));
            _a_ttor.write(fmt::format(
                u8"{0}{1}",
                prefix(_a_idx),
                _a_pc.indent(_a_pc.colon(_a_pc.source_location_str()))
            ));
            _a_ttor.write(fmt::format(
                u8"{0}{1}",
                prefix(_a_idx),
                _a_pc.indent(
                    _a_pc.source_location(
                        _a_element.source().begin_source().source_location()
                    ),
                    2
                )
            ));
            _a_ttor.write(fmt::format(
                u8"{0}{1}",
                prefix(_a_idx),
                _a_pc.indent(_a_pc.colon(_a_pc.source_code_str()))
            ));
            _a_ttor.write(fmt::format(
                u8"{0}{1}",
                prefix(_a_idx),
                _a_pc.indent(
                    _a_pc.source_representation(abc::convert_string_to_u8string(
                        _a_element.source()
                            .begin_source()
                            .source_code_representation()
                    ).value()),
                    2
                )
            ));

            if (not _a_element.source().end_source().has_value())
            {
            }
            else
            {
                const ds::single_source_t _l_end{
                    _a_element.source().end_source().value()
                };
                _a_ttor.write(fmt::format(
                    u8"{0}{1}",
                    prefix(_a_idx),
                    _a_pc.indent(_a_pc.colon(_a_pc.source_location_str()))
                ));
                _a_ttor.write(fmt::format(
                    u8"{0}{1}",
                    prefix(_a_idx),
                    _a_pc.indent(
                        _a_pc.source_location(_l_end.source_location()), 2
                    )
                ));
                _a_ttor.write(fmt::format(
                    u8"{0}{1}",
                    prefix(_a_idx),
                    _a_pc.indent(_a_pc.colon(_a_pc.source_code_str()))
                ));
                _a_ttor.write(fmt::format(
                    u8"{0}{1}",
                    prefix(_a_idx),
                    _a_pc.indent(
                        _a_pc.source_representation(
                            abc::convert_string_to_u8string(
                                _l_end.source_code_representation()
                            ).value()
                        ),
                        2
                    )
                ));
            }
        }
        break;
    case LOG_INFOS:
    {
        _a_ttor.write(fmt::format(
            u8"{0}{1}", prefix(_a_idx), _a_pc.colon(_a_pc.log_info_str())
        ));
        for (const u8string_view& _l_str : _a_element.log_infos())
        {
            _a_ttor.write(fmt::format(
                u8"{0}{1}",
                prefix(_a_idx),
                _a_pc.indent(_a_pc.log_info(_l_str
                ))
            ));
        }
    }
    break;
    default:
        throw unaccounted_for_enum_exception(_a_fid);
    }
}

_END_ABC_REPORTERS_NS