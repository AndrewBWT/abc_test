#pragma once
#include "abc_test/internal/ds/data_generator_memoization/typeless_data_generator_collection_stack_trie.h"

#include <scn/scan.h>

_BEGIN_ABC_DS_NS
struct map_unique_id_to_tdg_collection_stack_trie_t;
using parse_map_unique_id_to_tdg_collection_stack_trie_result_t
    = std::expected<map_unique_id_to_tdg_collection_stack_trie_t, std::string>;
using key_t = std::string;

class map_unique_id_to_tdg_collection_stack_trie_t
{
public:
    __no_constexpr
        map_unique_id_to_tdg_collection_stack_trie_t() noexcept
        = default;
    __no_constexpr bool
        insert(
            const key_t&                  _a_id,
            const tdg_collection_stack_trie_t& _a_trie
        ) noexcept;
    __no_constexpr const std::map<key_t, tdg_collection_stack_trie_t>&
                         map() const noexcept;
    friend __no_constexpr
        parse_map_unique_id_to_tdg_collection_stack_trie_result_t
        parse_compressed_map_of_unique_ids_to_tdg_collection_stack_tries(
            const std::string_view _a_str
        ) noexcept;
    friend __no_constexpr std::string
        print_compressed_map_of_unique_ids_to_tdg_collection_stack_tries(
            const map_unique_id_to_tdg_collection_stack_trie_t& _a_map
        ) noexcept;
    __no_constexpr std::size_t
                   size() const noexcept;
private:
    std::map<key_t, tdg_collection_stack_trie_t> _m_internal_map;
};

_END_ABC_DS_NS

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
        const key_t&                  _a_id,
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
        const std::string_view _a_str
    ) noexcept
{
    // Assumes the string is in the format:
    //<string> = <empty_string> |
    //           <unsigned_integer>:<hex_string>:<string>
    using namespace std;
    vector<pair<string_view, string_view>> _l_strs;
    size_t                                 _l_idx{0};
    while (_l_idx < _a_str.size())
    {
        size_t _l_next_colon{_a_str.find(_l_idx, ':')};
        if (_l_next_colon == string::npos)
        {
            return unexpected(fmt::format(
                "Failure to parse part of "
                "map_unique_id_to_tdg_collection_stack_trie_t string. "
                "Unable to find colon (':') in text, even though there should "
                "be atleast one in the string. "
                "This error occoured after index {0}, containing the subtring "
                "\"{1}\".",
                _l_idx,
                _a_str.substr(_l_idx)
            ));
        }
        else
        {
            const string_view _l_number{
                _a_str.substr(_l_idx, _l_next_colon - _l_idx)
            };
            _l_idx = _l_next_colon + 1;
            // Finding the next colon. This one doesn't have to exist.
            size_t            _l_next_colon{_a_str.find(_l_idx, ':')};
            const string_view _l_hex_numbers{_a_str.substr(
                _l_idx,
                _l_next_colon == string::npos ? _a_str.size() - _l_idx
                                              : _l_next_colon - _l_idx
            )};
            _l_strs.push_back({_l_number, _l_hex_numbers});
            _l_idx = _l_next_colon == string::npos ? _a_str.size()
                                                   : _l_next_colon + 1;
        }
    }
    // At this point got a vector of string_views.
    map_unique_id_to_tdg_collection_stack_trie_t _l_map;
    for (size_t _l_idx{0}; _l_idx < _l_strs.size(); ++_l_idx)
    {
        auto& [_l_str_hex, _l_compressed_str]{_l_strs[_l_idx]};
        string _l_str = abc::utility::str::from_hex_with_exception(_l_str_hex);
        const parse_for_loop_stack_trie_result_t _l_compressed_scan_result{
            parse_compressed_repetition_tree_node(_l_compressed_str)
        };
        if (_l_compressed_scan_result.has_value())
        {
            const typeless_data_generator_collection_stack_trie_t& _l_it{
                _l_compressed_scan_result.value()
            };
            const bool _l_result{
                _l_map.insert(_l_str, _l_compressed_scan_result.value())
            };
            if (not _l_result)
            {
                return unexpected(fmt::format(
                    "Could not add element {0} to internal map. This is "
                    "because there is already an element with that unique ID "
                    "in the map. Unique ID = {1}",
                    (_l_idx + 1),
                    _l_str
                ));
            }
        }
        else
        {
            return unexpected(fmt::format(
                "Could not parse compressed string \"{0}\", which is element "
                "{1}. "
                "internal parser returned error message \"{2}\".",
                _l_compressed_str,
                (_l_idx + 1),
                _l_compressed_scan_result.error()
            ));
        }
    }
    return _l_map;
}

__no_constexpr_imp std::string
    print_compressed_map_of_unique_ids_to_tdg_collection_stack_tries(
        const map_unique_id_to_tdg_collection_stack_trie_t& _a_map
    ) noexcept
{
    using namespace std;
    string _l_rv;
    for (const pair<key_t, tdg_collection_stack_trie_t>& _l_element :
         _a_map.map())
    {
        string _l_key_compressed, _l_key_compressed_2;
        for (char _l_char : _l_element.first)
        {
            _l_key_compressed.push_back(static_cast<unsigned char>(_l_char));
        }
        for (unsigned char _l_char : _l_key_compressed)
        {
            _l_key_compressed_2.append(fmt::format("{:x}", _l_char));
        }
        _l_rv.append(fmt::format(
            "{0}:{1}:",
            _l_key_compressed_2,
            _l_element.second.print_for_loop_stack_trie_compressed()
        ));
    }
    _l_rv.erase(_l_rv.end()-1);
    return _l_rv;
}

_END_ABC_DS_NS

__no_constexpr_imp auto
    fmt::formatter<_ABC_NS_DS::map_unique_id_to_tdg_collection_stack_trie_t>::
        format(
            _ABC_NS_DS::map_unique_id_to_tdg_collection_stack_trie_t _a_rtd,
            format_context&                                          _a_ctx
        ) const -> format_context::iterator
{
    using namespace std;
    const string _l_rv{fmt::format(
        "{0}{{{1} = {2}}}",
        typeid(_a_rtd).name(),
        "_m_internal_map",
        _a_rtd.map()
    )};
    return formatter<string_view>::format(_l_rv, _a_ctx);
}