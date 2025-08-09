#pragma once
#include "abc_test/core/ds/data_generator_memoization/typeless_data_generator_collection_stack_trie.hpp"
#include "abc_test/utility/parsers/default_parser.hpp"
#include "abc_test/utility/printers/default_printer.hpp"
#include "abc_test/utility/types.hpp"

#include <cuchar>

_BEGIN_ABC_DS_NS
class map_unique_id_to_tdg_collection_stack_trie_t;
using parse_map_unique_id_to_tdg_collection_stack_trie_result_t
    = result_t<map_unique_id_to_tdg_collection_stack_trie_t>;
using key_t = std::u8string;

class map_unique_id_to_tdg_collection_stack_trie_t
{
public:
    __no_constexpr
        map_unique_id_to_tdg_collection_stack_trie_t() noexcept
        = default;
    __no_constexpr bool
        insert(const key_t& _a_id, const tdg_collection_stack_trie_t& _a_trie)
            noexcept;
    __no_constexpr const std::map<key_t, tdg_collection_stack_trie_t>&
                         map() const noexcept;
    friend __no_constexpr
        parse_map_unique_id_to_tdg_collection_stack_trie_result_t
        parse_compressed_map_of_unique_ids_to_tdg_collection_stack_tries(
            const std::u8string_view _a_str
        ) noexcept;
    template <bool To_Hex = true>
    friend __no_constexpr std::u8string
                          print_map_of_unique_ids_to_tdg_collection_stack_tries(
                              const map_unique_id_to_tdg_collection_stack_trie_t& _a_map
                          ) noexcept;
    friend __no_constexpr std::u8string
        print_compressed_map_of_unique_ids_to_tdg_collection_stack_tries(
            const map_unique_id_to_tdg_collection_stack_trie_t& _a_map
        ) noexcept;
    __no_constexpr std::size_t
                   size() const noexcept;
private:
    std::map<key_t, tdg_collection_stack_trie_t> _m_internal_map;
};

_END_ABC_DS_NS
_BEGIN_ABC_UTILITY_PARSER_NS

template <>
struct default_parser_t<abc::ds::map_unique_id_to_tdg_collection_stack_trie_t>
    : public parser_base_t<
          abc::ds::map_unique_id_to_tdg_collection_stack_trie_t>
{
    __no_constexpr virtual result_t<
        abc::ds::map_unique_id_to_tdg_collection_stack_trie_t>
        run_parser(parser_input_t& _a_parse_input) const;
    // using namespace abc::ds;
    // return parse_compressed_map_of_unique_ids_to_tdg_collection_stack_tries(
    //   _a_parse_input.sv()
    // );
};

_END_ABC_UTILITY_PARSER_NS
_BEGIN_ABC_UTILITY_PRINTER_NS

template <>
struct default_printer_t<abc::ds::map_unique_id_to_tdg_collection_stack_trie_t>
    : public printer_base_t<
          abc::ds::map_unique_id_to_tdg_collection_stack_trie_t>
{
    static constexpr bool is_specialized{true};

    __no_constexpr virtual std::u8string
        run_printer(const abc::ds::map_unique_id_to_tdg_collection_stack_trie_t&
                        _a_parse_input) const;
};

_END_ABC_UTILITY_PRINTER_NS

template <>
struct fmt::formatter<_ABC_NS_DS::map_unique_id_to_tdg_collection_stack_trie_t>
    : formatter<string_view>
{
    // parse is inherited from formatter<string_view>.
    // Can'tbe constexpr due to use of fmt::format
    __no_constexpr auto
        format(
            _ABC_NS_DS::map_unique_id_to_tdg_collection_stack_trie_t _a_rtd,
            format_context&                                          _a_ctx
        ) const -> format_context::iterator;
};

_BEGIN_ABC_DS_NS
__no_constexpr_imp bool
    map_unique_id_to_tdg_collection_stack_trie_t::insert(
        const key_t&                       _a_id,
        const tdg_collection_stack_trie_t& _a_trie
    ) noexcept
{
    if (_m_internal_map.contains(_a_id))
    {
        return false;
    }
    else
    {
        _m_internal_map.insert({_a_id, _a_trie});
        return true;
    }
}

__no_constexpr_imp const std::map<key_t, tdg_collection_stack_trie_t>&
    map_unique_id_to_tdg_collection_stack_trie_t::map() const noexcept
{
    return _m_internal_map;
}

__no_constexpr_imp std::size_t
    map_unique_id_to_tdg_collection_stack_trie_t::size() const noexcept
{
    return _m_internal_map.size();
}

__no_constexpr_imp parse_map_unique_id_to_tdg_collection_stack_trie_result_t
    parse_compressed_map_of_unique_ids_to_tdg_collection_stack_tries(
        const std::u8string_view _a_u8_str
    ) noexcept
{
    using namespace std;
    using namespace abc::utility::parser;
    using namespace abc::ds;
    vector<pair<u8string_view, u8string_view>> _l_strs;
    size_t                                     _l_idx{0};
    while (_l_idx < _a_u8_str.size())
    {
        size_t _l_next_colon{_a_u8_str.find(char8_t(':'), _l_idx)};
        if (_l_next_colon == string::npos)
        {
            return unexpected(fmt::format(
                u8"Failure to parse part of "
                u8"map_unique_id_to_tdg_collection_stack_trie_t string. "
                u8"Unable to find colon (':') in text, even though there "
                u8"should "
                u8"be atleast one in the string. "
                u8"This error occoured after index {0}, containing the "
                u8"subtring "
                u8"\"{1}\".",
                _l_idx,
                _a_u8_str.substr(_l_idx)
            ));
        }
        else
        {
            const u8string_view _l_number{
                _a_u8_str.substr(_l_idx, _l_next_colon - _l_idx)
            };
            _l_idx = _l_next_colon + 1;
            // Finding the next colon. This one doesn't have to exist.
            size_t _l_next_colon{_a_u8_str.find(char8_t(':'), _l_idx)};
            const u8string_view _l_hex_numbers{_a_u8_str.substr(
                _l_idx,
                _l_next_colon == u8string::npos ? _a_u8_str.size() - _l_idx
                                                : _l_next_colon - _l_idx
            )};
            _l_strs.push_back({_l_number, _l_hex_numbers});
            _l_idx = _l_next_colon == string::npos ? _a_u8_str.size()
                                                   : _l_next_colon + 1;
        }
    }
    // At this point got a vector of string_views.
    map_unique_id_to_tdg_collection_stack_trie_t _l_map;
    for (size_t _l_idx{0}; _l_idx < _l_strs.size(); ++_l_idx)
    {
        auto& [_l_str_hex, _l_compressed_str]{_l_strs[_l_idx]};
        const parser_result_t<typeless_data_generator_collection_stack_trie_t>
                 _l_compressed_scan_result{parse(
                _l_compressed_str,
                mk_parser(
                    compressed_typless_data_generator_collection_stack_trie_parser_t<
                             true>{}
                )
            )};
        result_t<u8string> _l_str_opt
            = abc::utility::str::from_hex(_l_str_hex);
        if (_l_str_opt.has_value())
        {
            if (_l_compressed_scan_result.has_value())
            {
                const bool _l_result{
                    _l_map.insert(_l_str_opt.value(), _l_compressed_scan_result.value())
                };
                if (not _l_result)
                {
                    return unexpected(fmt::format(
                        u8"Could not add element {0} to internal map. This is "
                        u8"because there is already an element with that unique ID "
                        u8"in the map. Unique ID = {1}",
                        (_l_idx + 1),
                        _l_str_opt.value()
                    ));
                }
            }
            else
            {
                return unexpected(fmt::format(
                    u8"Could not parse compressed string \"{0}\", which is element "
                    u8"{1}. "
                    u8"internal parser returned error message \"{2}\".",
                    _l_compressed_str,
                    (_l_idx + 1),
                    _l_compressed_scan_result.error().errors
                ));
            }
        }
        else
        {
            return unexpected(fmt::format(
                u8"Could not parse compressed string \"{0}\", which is element "
                u8"{1}. "
                u8"internal parser returned error message \"{2}\".",
                _l_str_hex,
                (_l_idx + 1),
                _l_str_opt.error()
            ));
        }
    }
    return _l_map;
}

template <bool To_Hex>
__no_constexpr_imp std::u8string
                   print_map_of_unique_ids_to_tdg_collection_stack_tries(
                       const map_unique_id_to_tdg_collection_stack_trie_t& _a_map
                   ) noexcept
{
    using namespace std;
    u8string _l_rv;
    for (const pair<key_t, tdg_collection_stack_trie_t> _l_element :
         _a_map.map())
    {
        u8string _l_key_compressed, _l_key_compressed_2;
        for (char _l_char : _l_element.first)
        {
            _l_key_compressed.push_back(static_cast<unsigned char>(_l_char));
        }
        if constexpr (To_Hex)
        {
            for (unsigned char _l_char : _l_key_compressed)
            {
                _l_key_compressed_2.append(fmt::format(u8"{:x}", _l_char));
            }
        }
        else
        {
            _l_key_compressed_2 = _l_key_compressed;
        }
        _l_rv.append(fmt::format(
            u8"{0}:{1}:",
            _l_key_compressed_2,
            compressed_typless_data_generator_collection_stack_trie_printer_t<
                To_Hex>{}
                .run_printer(_l_element.second)
        ));
    }
    if (_l_rv.size() > 0)
    {
        _l_rv.erase(_l_rv.end() - 1);
    }
    return _l_rv;
}

__no_constexpr_imp std::u8string
    print_compressed_map_of_unique_ids_to_tdg_collection_stack_tries(
        const map_unique_id_to_tdg_collection_stack_trie_t& _a_map
    ) noexcept
{
    using namespace std;
    using namespace abc::utility::printer;
    u8string _l_rv;
    for (const pair<key_t, tdg_collection_stack_trie_t> _l_element :
         _a_map.map())
    {
        u8string _l_key_compressed, _l_key_compressed_2;
        for (char _l_char : _l_element.first)
        {
            _l_key_compressed.push_back(static_cast<unsigned char>(_l_char));
        }
        for (unsigned char _l_char : _l_key_compressed)
        {
            _l_key_compressed_2.append(fmt::format(u8"{:x}", _l_char));
        }
        _l_rv.append(fmt::format(
            u8"{0}:{1}:",
            _l_key_compressed_2,
            compressed_typless_data_generator_collection_stack_trie_printer_t<
                true>{}
                .run_printer(_l_element.second)
        ));
    }
    if (_l_rv.size() > 0)
    {
        _l_rv.erase(_l_rv.end() - 1);
    }
    return _l_rv;
}

_END_ABC_DS_NS

_BEGIN_ABC_UTILITY_PRINTER_NS
__no_constexpr_imp std::u8string
    default_printer_t<abc::ds::map_unique_id_to_tdg_collection_stack_trie_t>::
        run_printer(
            const abc::ds::map_unique_id_to_tdg_collection_stack_trie_t&
                _a_parse_input
        ) const
{
    using namespace std;
    using namespace ds;
    return default_printer<map<key_t, tdg_collection_stack_trie_t>>()
        ->run_printer(_a_parse_input.map());
}

_END_ABC_UTILITY_PRINTER_NS

_BEGIN_ABC_UTILITY_PARSER_NS
__no_constexpr_imp
    result_t<abc::ds::map_unique_id_to_tdg_collection_stack_trie_t>
    default_parser_t<abc::ds::map_unique_id_to_tdg_collection_stack_trie_t>::
        run_parser(
            parser_input_t& _a_parse_input
        ) const
{
    using namespace abc::ds;
    return parse_compressed_map_of_unique_ids_to_tdg_collection_stack_tries(
        _a_parse_input.get_u8string()
    );
}

_END_ABC_UTILITY_PARSER_NS
__no_constexpr_imp auto
    fmt::formatter<_ABC_NS_DS::map_unique_id_to_tdg_collection_stack_trie_t>::
        format(
            _ABC_NS_DS::map_unique_id_to_tdg_collection_stack_trie_t _a_rtd,
            format_context&                                          _a_ctx
        ) const -> format_context::iterator
{
    using namespace std;
    const string _l_rv{fmt::format(
        ""
        // "{0}{{{1} = {2}}}",
        // typeid(_a_rtd).name(),
        // "_m_internal_map",
        // abc::u8string_to_string(fmt::format(u8"{}", _a_rtd.map()))
    )};
    return formatter<string_view>::format(_l_rv, _a_ctx);
}