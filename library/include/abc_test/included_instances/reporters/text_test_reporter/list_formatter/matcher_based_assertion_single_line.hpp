#pragma once
#include "abc_test/core/test_reports/matcher_based_assertion_single_line.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/matcher_based_assertion_single_line.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/list_formatter/assertion.hpp"
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
    using assertion_list_formatter_t<true, Assertion_Status>::
        assertion_list_formatter_t;
    __constexpr virtual bool
        check_data(
            const combined_enum_matcher_based_assertion_single_line_fields_t&
                _a_fid,
            const reports::matcher_based_assertion_single_line_t<
                Assertion_Status>& _a_element
        ) const override;
    __constexpr virtual void
        get_data(
            const combined_enum_matcher_based_assertion_single_line_fields_t&
                _a_fid,
            const reports::matcher_based_assertion_single_line_t<
                Assertion_Status>&                              _a_element,
            const print_config_t&                               _a_pc,
            const utility::io::threated_text_output_reporter_t& _a_ttor,
            const std::size_t                                   _a_idx
        ) const override;
protected:
    __constexpr virtual std::u8string
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
__constexpr_imp void
    matcher_based_assertion_single_line_list_formatter_t<Assertion_Status>::
        get_data(
            const combined_enum_matcher_based_assertion_single_line_fields_t&
                _a_fid,
            const reports::matcher_based_assertion_single_line_t<
                Assertion_Status>&                              _a_element,
            const print_config_t&                               _a_pc,
            const utility::io::threated_text_output_reporter_t& _a_ttor,
            const std::size_t                                   _a_idx
        ) const
{
    using namespace std;
    using namespace _ABC_NS_UTILITY_STR;
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
            _a_ttor.write(
                this->prefix(_a_idx)
                + _a_pc.colon(_a_pc.matcher_source_map_str())
            );
            for (const pair<std::source_location, vector<string>>& _l_element :
                 _a_element.matcher_info().source_map().map())
            {
                _a_ttor.write(
                    this->prefix(_a_idx + 1)
                    + _a_pc.indent(_a_pc.colon(_a_pc.source_location_str()))
                );
                _a_ttor.write(
                    this->prefix(_a_idx + 1)
                    + _a_pc.indent(_a_pc.source_location(_l_element.first), 2)
                );
                _a_ttor.write(
                    this->prefix(_a_idx + 1)
                    + _a_pc.indent(_a_pc.colon(_a_pc.source_code_str()))
                );
                for (const string_view _l_str : _l_element.second)
                {
                    _a_ttor.write(
                        this->prefix(_a_idx + 1)
                        + _a_pc.indent(
                            _a_pc.source_representation(
                                cast_string_to_u8string(_l_str)
                            ),
                            2
                        )
                    );
                }
            }
        }
        break;
        case MATCHER_OUTPUT:
            _a_ttor.write(
                this->prefix(_a_idx)
                + _a_pc.matcher_output_str(
                    _a_element.get_pass_status(),
                    ( std::same_as<
                          Assertion_Status,
                          _ABC_NS_REPORTS::terminate_t>
                      || std::same_as<
                          Assertion_Status,
                          _ABC_NS_REPORTS::pass_or_terminate_t> )
                )
            );
            _a_ttor.write_line(u8"");
            {
                const auto& _l_matcher_strs{
                    _a_element.matcher_info().matcher_result().str()
                };
                vector<variant<
                    pair<size_t, u8string>,
                    matcher::matcher_result_infos_t>>
                    _l_stack;
                vector<variant<
                    pair<size_t, u8string>,
                    matcher::matcher_result_infos_t>>
                    _l_to_add;
                _l_stack.push_back(_l_matcher_strs);
                matcher::matcher_result_infos_t _l_matcher_element;
                while (_l_stack.size() > 0)
                {
                    _l_to_add.clear();
                    const auto _l_element{_l_stack.front()};
                    _l_stack.erase(_l_stack.begin());
                    switch (_l_element.index())
                    {
                    case 0:
                        _a_ttor.write(this->prefix(_a_idx + 1));
                        _a_ttor.write_line(_a_pc.indent(
                            _a_pc.highlight(get<0>(_l_element).second),
                            get<0>(_l_element).first
                        ));
                        break;
                    case 1:
                        _l_matcher_element = get<1>(_l_element);
                        _a_ttor.write(this->prefix(_a_idx + 1));
                        _a_ttor.write_line(
                            _a_pc.indent(u8"Primary output from matcher:", 1)
                        );
                        _a_ttor.write(this->prefix(_a_idx + 1));
                        _a_ttor.write_line(_a_pc.indent(
                            _a_pc.message_str(_l_matcher_element.primary_data()
                            ),
                            2
                        ));
                        if (_l_matcher_element.get_vector().size() > 0
                            || _l_matcher_element.get_tree().size() > 0)
                        {
                            _a_ttor.write(this->prefix(_a_idx + 1));
                            _a_ttor.write_line(
                                _a_pc.indent(u8"Additional Information: ", 1)
                            );
                        }
                        for (auto& _l_str : _l_matcher_element.get_vector())
                        {
                            _a_ttor.write(this->prefix(_a_idx + 1));
                            _a_ttor.write_line(
                                _a_pc.indent(_a_pc.highlight(_l_str), 2)
                            );
                        }
                        for (auto& _l_str : _l_matcher_element.get_tree())
                        {
                            _l_to_add.push_back(make_pair(1, get<0>(_l_str)));
                            _l_to_add.push_back(make_pair(0, u8"---"));
                            _l_to_add.push_back(*get<2>(_l_str).get());
                        }
                        _l_stack.insert(
                            _l_stack.begin(), _l_to_add.begin(), _l_to_add.end()
                        );
                    }
                }
            }
            break;
        case MATCHER_ANNOTATION:
            _a_ttor.write(
                this->prefix(_a_idx) + _a_pc.colon(_a_pc.matcher_annotation())
            );
            _a_ttor.write(
                this->prefix(_a_idx + 1)
                + _a_pc.indent(
                    _a_pc.message_str(_a_element.matcher_info().annotation())
                )
            );
            break;
        default:
            throw errors::unaccounted_for_enum_exception(*_l_ptr);
        }
    }
    else if (auto _l_ptr{get_if<combined_enum_assertion_fields_t>(&_a_fid)};
             _l_ptr != nullptr)
    {
        return assertion_list_formatter_t<true, Assertion_Status>::get_data(
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
    matcher_based_assertion_single_line_list_formatter_t<Assertion_Status>::
        get_str_representation(
            const reports::assertion_t<true, Assertion_Status>& _a_element,
            const print_config_t&                               _a_pc
        ) const
{
    return construct_str_representation(
        _a_element, u8"Matcher-based assertion"
    );
}

_END_ABC_REPORTERS_NS