#pragma once
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/multi_element_assertion_block.hpp"
#include "abc_test/internal/test_reports/multi_element_assertion_block.hpp"
#include "abc_test/internal/utility/str/string_table.hpp"
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
            vector<string> _l_rv;
            _l_rv.push_back(
                _a_pc.colon(_a_pc.multi_element_collection_grouped_str())
            );
            vector<pair<
                pair<optional<ds::single_source_t>, matcher_source_map_t>,
                vector<tuple<size_t, matcher_result_t, optional<string>>>>>
                   _l_info;
            size_t _l_idx{1};
            for (const matcher_result_with_annotation_and_source_info_t&
                     _l_matcher : _a_element.get_matcher())
            {
                pair<optional<ds::single_source_t>, matcher_source_map_t>
                    _l_first
                    = make_pair(_l_matcher.source(), _l_matcher.source_map());
                tuple<size_t, matcher_result_t, optional<string>>
                    _l_single_element = make_tuple(
                        _l_idx++,
                        _l_matcher.matcher_result(),_l_matcher.annotation()
                    );
                if (_l_info.size() == 0 || _l_first != _l_info.back().first)
                {
                    _l_info.push_back(make_pair(
                        _l_first,
                        vector<
                            tuple<size_t, matcher_result_t, optional<string>>>(
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
                     pair<optional<ds::single_source_t>, matcher_source_map_t>,
                     vector<tuple<size_t, matcher_result_t, optional<string>>>>&
                     _l_element : _l_info)
            {
                if (_l_element.second.size() == 1)
                {
                    str::string_table_t _l_st({0});
                    std::tuple<
                        matcher_result_t,
                        std::optional<std::string>,
                        matcher_source_map_t>;
                    matcher_result_with_annotation_and_source_info_t _l_matcher(
                    get<1>(_l_element.second[0]),
                        _l_element.first.first,
                        get<2>(_l_element.second[0]),
                        _l_element.first.second
                    );
                    vector<string> _l_data = get_all_data(
                        _a_pc
                            .matcher_assertion_multi_block_assertion_list_fields(
                            ),
                        _l_matcher,
                        _a_pc,
                        assertion_block_matcher_data_list_formatter_t()
                    );
                    size_t _l_data_idx{0};
                    if (_l_data.size() != 0)
                    {
                        for (const string_view _l_sv : _l_data)
                        {
                            _l_st.push_back(
                                _l_data_idx++ == 0
                                    ? fmt::format(
                                          "{0}) ", get<0>(_l_element.second[0])
                                      )
                                    : ""
                            );
                            _l_st.push_back(_l_sv);
                            _l_st.new_line();
                        }
                    }
                    for (string _l_s : _l_st.as_lines())
                    {
                        _l_rv.push_back(_l_s);
                    }
                }
                else
                {
                    _l_rv.push_back(_a_pc.indent(fmt::format(
                        "The following {0} matchers have the same source data, "
                        "which is as follows:",
                        _l_element.second.size()
                    )));
                    std::tuple<
                        matcher_result_t,
                        std::optional<std::string>,
                        matcher_source_map_t>;
                    matcher_result_with_annotation_and_source_info_t _l_matcher(
                        get<1>(_l_element.second[0]),
                        _l_element.first.first,
                        get<2>(_l_element.second[0]),
                        _l_element.first.second
                    );
                    vector<string> _l_data = get_all_data(
                        _l_shared_data,
                        _l_matcher,
                        _a_pc,
                        assertion_block_matcher_data_list_formatter_t(2)
                    );
                    _l_rv.append_range(_l_data);

                    _l_rv.push_back(_a_pc.indent(
                        fmt::format(
                            "The data of the {0} matchers:",
                            _l_element.second.size()
                        ),
                        1
                    ));
                    str::string_table_t _l_st({0});
                    for (auto& _l_ki : _l_element.second)
                    {
                        matcher_result_with_annotation_and_source_info_t _l_matcher(
                            get<1>(_l_ki),
                            _l_element.first.first,
                            get<2>(_l_ki),
                            _l_element.first.second
                        );
                        vector<string> _l_data = get_all_data(
                            _l_individual_data,
                            _l_matcher,
                            _a_pc,
                            assertion_block_matcher_data_list_formatter_t(0)
                        );
                        size_t _l_data_idx{0};
                        if (_l_data.size() != 0)
                        {
                            for (const string_view _l_sv : _l_data)
                            {
                                _l_st.push_back(
                                    _l_data_idx++ == 0
                                        ? fmt::format("{0}) ", get<0>(_l_ki))
                                        : ""
                                );
                                _l_st.push_back(_l_sv);
                                _l_st.new_line();
                            }
                        }
                    }
                    for (string _l_s : _l_st.as_lines())
                    {
                        _l_rv.push_back(_a_pc.indent(_l_s, 2));
                    }
                }
            }
            return _l_rv;
        }
        case SHOW_ALL_ASSERTIONS:
        {
            using namespace _ABC_NS_UTILITY;
            vector<string> _l_rv;
            _l_rv.push_back(_a_pc.colon(_a_pc.multi_element_collection_str()));
            str::string_table_t _l_st({0});
            size_t              _l_idx{1};
            for (auto& _l_matcher : _a_element.get_matcher())
            {
                vector<string> _l_data = get_all_data(
                    _a_pc.matcher_assertion_multi_block_assertion_list_fields(),
                    _l_matcher,
                    _a_pc,
                    assertion_block_matcher_data_list_formatter_t(0)
                );
                size_t _l_data_idx{0};
                if (_l_data.size() != 0)
                {
                    for (const string_view _l_sv : _l_data)
                    {
                        _l_st.push_back(
                            _l_data_idx++ == 0 ? fmt::format("{0}) ", _l_idx)
                                               : ""
                        );
                        _l_st.push_back(_l_sv);
                        _l_st.new_line();
                    }
                }
                ++_l_idx;
            }
            for (string _l_s : _l_st.as_lines())
            {
                _l_rv.push_back(_l_s);
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
    using namespace _ABC_NS_MATCHER;
    const reports::multi_element_assertion_block_t<Assertion_Status>&
                _l_element{dynamic_cast<
                    const reports::multi_element_assertion_block_t<Assertion_Status>&>(
            _a_element
        )};
    std::size_t _l_passed{0};
    for (const matcher_result_with_annotation_and_source_info_t& _l_ki : _l_element.get_matcher())
    {
        if (_l_ki.matcher_result().passed())
        {
            _l_passed++;
        }
    }
    return construct_str_representation(
        _a_element,
        "Multi-element block-based assertion",
        fmt::format(
            " {0}/{1} matchers passed.",
            _l_passed,
            _l_element.get_matcher().size()
        )
    );
}

_END_ABC_REPORTERS_NS