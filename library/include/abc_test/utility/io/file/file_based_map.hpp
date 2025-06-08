#pragma once

#include "abc_test/utility/io/file/general_data.hpp"
#include "abc_test/utility/str/rw_info.hpp"

_BEGIN_ABC_UTILITY_IO_NS

template <class Key, class Val>
class file_based_map_t
{
public:
    inline file_based_map_t(
        const std::string                        _a_file_name,
        const abc::utility::str::rw_info_t<Val>& _a_rw_info
        = abc::utility::str::rw_info_t<Val>{}
    )
        : file_based_map_t<Key, Val>(abc::gdf(_a_file_name), _a_rw_info)
    {}

    inline file_based_map_t(
        const abc::utility::io::general_data_t&  _a_gdf,
        const abc::utility::str::rw_info_t<Val>& _a_rw_info
        = abc::utility::str::rw_info_t<Val>{}
    )
        : _m_file_name(_a_gdf.path())
        , _m_element_printer(abc::utility::printer::default_printer<map_type_t>(
              abc::utility::printer::default_printer<key_t>(),
              abc::utility::printer::default_printer<value_t>(
                  _a_rw_info.internal_printer
              )
          ))
        , _m_element_parser(abc::utility::parser::default_parser<map_type_t>(
              abc::utility::parser::default_parser<key_t>(),
              abc::utility::parser::default_parser<value_t>(
                  _a_rw_info.internal_parser
              )
          ))
    {
        using namespace std;
        using namespace abc::utility::io;
        using namespace abc;
        if (not std::filesystem::exists(_m_file_name))
        {
            file_line_writer_t _l_flw(_m_file_name);
            _l_flw.write_comment(
                fmt::format(u8"{0}", type_id<decltype(_m_inner_map)>())
            );
        }
        {
            size_t             _l_line_idx{0};
            file_line_reader_t _l_flr(_m_file_name);
            while (_l_flr.has_current_line())
            {
                using namespace abc::utility::parser;
                const result_t<map_type_t> _l_read_element{
                    parse(_l_flr.current_line(), _m_element_parser)
                };
                if (_l_read_element.has_value())
                {
                    const auto& _l_parsed_element{_l_read_element.value()};
                    if (auto _l_finder{_m_inner_map.find(_l_parsed_element.first
                        )};
                        _l_finder == _m_inner_map.end())
                    {
                        _m_inner_map.insert(_l_parsed_element);
                    }
                    else
                    {
                        throw abc::errors::test_library_exception_t(fmt::format(
                            u8"{0} was able to successfully parse the {1} line "
                            u8"of file {2}"
                            u8"\"{3}\" to the the entity {4} of type {5}. "
                            u8"However, it was found that the internal map "
                            u8"already contains an element with this key. The "
                            u8"current element is {6}.",
                            type_id<decltype(*this)>(),
                            positive_integer_to_placement(_l_line_idx),
                            _m_file_name.u8string(),
                            _l_flr.current_line(),
                            _m_element_printer->run_printer(_l_parsed_element),
                            type_id<map_type_t>(),
                            _m_element_printer->run_printer(*_l_finder)
                        ));
                    }
                }
                else
                {
                    throw abc::errors::test_library_exception_t(fmt::format(
                        u8"{0} could not parse line \"{1}\" of file {2}, which "
                        u8"should "
                        u8"contain a parsable entity of type {3}.",
                        type_id<decltype(*this)>(),
                        _l_flr.current_line(),
                        _m_file_name.u8string(),
                        type_id<map_type_t>()
                    ));
                }
                _l_flr.get_next_line();
                ++_l_line_idx;
            }
        }
        _m_flw = file_line_writer_t(_m_file_name);
    }

    template <typename F>
    requires std::is_invocable_r_v<abc::matcher_t, F, const Val&>
    inline abc::matcher_t
        generate_matcher(
            const Key& _a_key,
            F          _a_function
        )
    {
        using namespace abc;
        if (auto _l_finder{_m_inner_map.find(_a_key)};
            _l_finder != _m_inner_map.end())
        {
            if (_l_finder->second.has_value())
            {
                return std::invoke_r<abc::matcher_t>(
                    _a_function, _l_finder->second.value()
                );
            }
            else
            {
                using namespace abc::matcher;
                using namespace std;
                using namespace abc::utility::printer;
                return mk_matcher_using_result(matcher_result_t(
                    false,
                    matcher_result_infos_t(fmt::format(
                        u8"basic_file_reader_t<T>, pointing to file {0}, "
                        u8"contains an entry for the {1} {2}. "
                        u8"However, the entry is set to an empty {3} value. "
                        u8"It should be set to an exact value of type {4}. "
                        u8"Please ammend the file's entry accordingly.",
                        _m_file_name.u8string(),
                        type_id<key_t>(),
                        default_printer_t<Key>{}.run_printer(_a_key),
                        type_id<value_t>(),
                        type_id<Val>()
                    ))
                ));
            }
        }
        else
        {
            using namespace abc::matcher;
            using namespace std;
            using namespace abc::utility::printer;
            auto _l_new_map_element{make_pair(_a_key, nullopt)};
            _m_flw.write_line(abc::utility::printer::print<map_type_t>(
                _l_new_map_element, _m_element_printer
            ));
            _m_inner_map.insert(_l_new_map_element);
            return mk_matcher_using_result(matcher_result_t(
                false,
                matcher_result_infos_t(fmt::format(
                    u8"basic_file_reader_t<T>, pointing to file {0}, "
                    u8"does not contain an entry for the {1} \"{2}\". "
                    u8"The file has been ammended to contain an entriy for the "
                    u8"{1}, "
                    u8"set at an empty {3} value. "
                    u8"Please ammend the file to set this entry to an exact "
                    u8"value of type {4}.",
                    _m_file_name.u8string(),
                    type_id<key_t>(),
                    default_printer_t<Key>{}.run_printer(_a_key),
                    type_id<value_t>(),
                    type_id<Val>()
                )
            )));
        }
    }
private:
    using key_t      = Key;
    using value_t    = std::optional<Val>;
    using map_type_t = std::pair<key_t, value_t>;
    std::map<key_t, value_t>                     _m_inner_map;
    std::filesystem::path                        _m_file_name;
    abc::utility::io::file_line_writer_t         _m_flw;
    abc::utility::printer::printer_t<map_type_t> _m_element_printer;
    abc::utility::parser::parser_t<map_type_t>   _m_element_parser;
};

_END_ABC_UTILITY_IO_NS