#pragma once
#include "abc_test/core/test_reports/multi_element_assertion_block.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/multi_element_assertion_block.hpp"
_BEGIN_ABC_REPORTERS_NS

template <typename Assertion_Status>
struct multi_element_test_block_list_formatter_t
    : public list_formattable_t<
          reports::multi_element_assertion_block_t<Assertion_Status>,
          combined_enum_multi_element_assertion_block_fields_t,
          print_config_t>
    , public assertion_list_formatter_t<false, Assertion_Status>
{
public:
    using assertion_list_formatter_t<false, Assertion_Status>::
        assertion_list_formatter_t;
    __constexpr virtual bool
        check_data(
            const combined_enum_multi_element_assertion_block_fields_t& _a_fid,
            const reports::multi_element_assertion_block_t<Assertion_Status>&
                _a_element
        ) const override;
    __constexpr virtual void
        get_data(
            const combined_enum_multi_element_assertion_block_fields_t& _a_fid,
            const reports::multi_element_assertion_block_t<Assertion_Status>&
                                                                _a_element,
            const print_config_t&                               _a_pc,
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
        case SMART_SHOW_ALL_ASSERTIONS:
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
__constexpr_imp void
    multi_element_test_block_list_formatter_t<Assertion_Status>::get_data(
        const combined_enum_multi_element_assertion_block_fields_t& _a_fid,
        const reports::multi_element_assertion_block_t<Assertion_Status>&
                                                            _a_element,
        const print_config_t&                               _a_pc,
        const utility::io::threated_text_output_reporter_t& _a_ttor,
        const std::size_t _a_idx
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
        case SMART_SHOW_ALL_ASSERTIONS:
        {
            using namespace _ABC_NS_UTILITY;
            using namespace _ABC_NS_MATCHER;
            enum_print_pair_collection_t<
                enum_assertion_block_matcher_data_fields_t>
                _l_shared_data;
            enum_print_pair_collection_t<
                enum_assertion_block_matcher_data_fields_t>
                _l_individual_data;
            for (enum_print_pair_t<enum_assertion_block_matcher_data_fields_t>&
                     _l_x :
                 _a_pc.matcher_assertion_multi_block_assertion_list_fields())
            {
                using enum enum_assertion_block_matcher_data_fields_t;
                switch (_l_x.first)
                {
                case MATCHER_ANNOTATION:
                case MATCHER_RESULT_STRING:
                case LOG_INFOS:
                    _l_individual_data.push_back(_l_x);
                    break;
                case MATCHER_SOURCE_MAP:
                case MATCHER_MAIN_SOURCE_REP:
                case MATCHER_MAIN_SOURCE_LOCATION:
                    _l_shared_data.push_back(_l_x);
                    break;
                default:
                    break;
                }
            }
            _a_ttor.write_line(fmt::format(
                u8"{0}{1}",this->prefix(_a_idx), _a_pc.colon(_a_pc.multi_element_collection_grouped_str())
            ));
            vector<pair<
                tuple<
                    bool,
                    enum_bba_inner_assertion_type_t,
                    optional<ds::single_source_t>,
                    matcher_source_map_t>,
                vector<tuple<size_t, matcher_result_t, optional<u8string>, _ABC_NS_DS::log_infos_t>>>>
                   _l_info;
            size_t _l_idx{1};
            for (const bba_inner_assertion_type_t& _l_matcher :
                 _a_element.get_matcher())
            {
                tuple<
                    bool,
                    enum_bba_inner_assertion_type_t,
                    optional<ds::single_source_t>,
                    matcher_source_map_t>
                    _l_first = make_tuple(
                        _l_matcher.terminate(),
                        _l_matcher.assertion_type(),
                        _l_matcher.source(),
                        _l_matcher.source_map()
                    );
                tuple<size_t, matcher_result_t, optional<u8string>, _ABC_NS_DS::log_infos_t>
                    _l_single_element = make_tuple(
                        _l_idx++,
                        _l_matcher.matcher_result(),
                        _l_matcher.annotation(),
                        _l_matcher.log_infos()
                    );
                if (_l_info.size() == 0 || _l_first != _l_info.back().first)
                {
                    _l_info.push_back(make_pair(
                        _l_first,
                        vector<
                            tuple<size_t, matcher_result_t, optional<u8string>,_ABC_NS_DS::log_infos_t>>(
                            1, _l_single_element
                        )
                    ));
                }
                else
                {
                    _l_info.back().second.push_back(_l_single_element);
                }
            }
            for (const pair<
                     tuple<
                         bool,
                         enum_bba_inner_assertion_type_t,
                         optional<ds::single_source_t>,
                         matcher_source_map_t>,
                     vector<tuple<size_t, matcher_result_t, optional<u8string>,_ABC_NS_DS::log_infos_t>>>&
                     _l_element : _l_info)
            {
                if (_l_element.second.size() == 1)
                {
                    std::tuple<
                        matcher_result_t,
                        std::optional<std::u8string>,
                        matcher_source_map_t>;
                    bba_inner_assertion_type_t _l_matcher(
                        get<3>(_l_element.second[0]),
                        get<0>(_l_element.first),
                        get<1>(_l_element.second[0]),
                        get<2>(_l_element.first),
                        get<2>(_l_element.second[0]),
                        get<3>(_l_element.first),
                        get<1>(_l_element.first)
                    );
                    assertion_block_matcher_data_list_formatter_t(
                        get<0>(_l_element.second[0]), _l_info.size(), this->_m_max_str_size, true
                    )
                        .process_all_data(
                            _a_pc
                                .matcher_assertion_multi_block_assertion_list_fields(
                                ),
                            _l_matcher,
                            _a_pc,
                            _a_ttor
                        );
                }
                else
                {
                    _a_ttor.write_line(fmt::format(
                        u8"{0}{1}", this->prefix(_a_idx), _a_pc.indent(fmt::format(
                        u8"The following {0} {1} assertions have the same source "
                        u8"data, "
                        u8"which is as follows:",
                        _l_element.second.size(),
                        get_str(get<1>(_l_element.first))
                    ))));
                    std::tuple<
                        matcher_result_t,
                        std::optional<std::string>,
                        matcher_source_map_t>;
                    bba_inner_assertion_type_t _l_matcher(
                        get<3>(_l_element.second[0]),
                        get<0>(_l_element.first),
                        get<1>(_l_element.second[0]),
                        get<2>(_l_element.first),
                        get<2>(_l_element.second[0]),
                        get<3>(_l_element.first),
                        get<1>(_l_element.first)
                    );
                    assertion_block_matcher_data_list_formatter_t(
                        get<0>(_l_element.second[0]), _l_info.size(), this->_m_max_str_size, false, 2
                    )
                        .process_all_data(
                            _l_shared_data,
                            _l_matcher,
                            _a_pc,
                            _a_ttor
                        );
                    // _l_rv.append_range(_l_data);

                    _a_ttor.write_line(this->prefix(_a_idx) + _a_pc.indent(
                        fmt::format(
                            u8"The data of the {0} {1} assertions:",
                            _l_element.second.size(),
                            get_str(get<1>(_l_element.first))
                        ),
                        1
                    ));
                    for (auto& _l_ki : _l_element.second)
                    {
                        bba_inner_assertion_type_t _l_matcher(
                            get<3>(_l_ki),
                            get<0>(_l_element.first),
                            get<1>(_l_ki),
                            get<2>(_l_element.first),
                            get<2>(_l_ki),
                            get<3>(_l_element.first),
                            get<1>(_l_element.first)
                        );
                        assertion_block_matcher_data_list_formatter_t(
                            get<0>(_l_ki), _l_element.second.size(), this->_m_max_str_size, true
                        )
                            .process_all_data(
                                _l_individual_data,
                                _l_matcher,
                                _a_pc,
                                _a_ttor
                            );
                    }
                }
            }
        }
        break;
        case SHOW_ALL_ASSERTIONS:
        {
            using namespace _ABC_NS_UTILITY;
            _a_ttor.write_line(_a_pc.colon(_a_pc.multi_element_collection_str()));
            size_t              _l_idx{1};
            for (auto& _l_matcher : _a_element.get_matcher())
            {
                assertion_block_matcher_data_list_formatter_t(
                    _l_idx, _a_element.get_matcher().size(), this->_m_max_str_size, true
                )
                    .process_all_data(
                        _a_pc
                            .matcher_assertion_multi_block_assertion_list_fields(
                            ),
                        _l_matcher,
                        _a_pc,
                        _a_ttor
                    );
                ++_l_idx;
            }
        }
        break;
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
                multi_element_test_block_list_formatter_t<Assertion_Status>::
        get_str_representation(
            const reports::assertion_t<false, Assertion_Status>& _a_element,
            const print_config_t&                                _a_pc
        ) const
{
    using namespace _ABC_NS_MATCHER;
    const reports::multi_element_assertion_block_t<Assertion_Status>&
                _l_element{dynamic_cast<
                    const reports::multi_element_assertion_block_t<Assertion_Status>&>(
            _a_element
        )};
    std::size_t _l_passed{0};
    for (const bba_inner_assertion_type_t& _l_ki : _l_element.get_matcher())
    {
        if (_l_ki.matcher_result().passed())
        {
            _l_passed++;
        }
    }
    return construct_str_representation(
        _a_element,
        u8"Multi-element block-based assertion",
        fmt::format(
            u8" {0}/{1} assertions passed.",
            _l_passed,
            _l_element.get_matcher().size()
        )
    );
}

_END_ABC_REPORTERS_NS