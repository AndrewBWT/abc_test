#pragma once
#include <charconv>
#include "abc_test/core/ds/repetitions/for_loop_data.h"
#include "abc_test/core/errors/test_library_exception.h"
#include "abc_test/utility/str/parser_utility.h"
#include "fmt/base.h"
#include "fmt/ranges.h"
#include <exception>

_BEGIN_ABC_DS_NS
/*!
 * @brief Represents the node in a repetition tree, a structure used
 * when navigating through gen_data for loops to either memoize data so 
 * parts of tests can be repeated, or to read memoized data and tell other
 * parts of abc_test whether to skip for loops or not.
 */
template<
    typename bool Is_Root
>
class repetition_tree_node_t
{
public:
    /*!
     * @brief Default constructor.
     */
    __constexpr
        repetition_tree_node_t(
        ) noexcept = default;
    /*!
     * @brief Prints the underlying tree in a "compressed format".
     *
     * Currently this means taking the string from 
     * "print_repetition_tree" and turning it into hex.
     * 
     * @return Compressed string representing the object.
     */
    __constexpr
        std::string
        print_repetition_tree_compressed(
        ) const noexcept;
    /*!
     * @brief Prints a string representing the object.
     * 
     * If the element is a root, then it returns a string in the form:
     * 
     * "[_m_children]"
     * 
     * If it is not the root, it returns a string in the form:
     * 
     * "(_m_for_loop_data.generation_collection_index,
     *  _m_for_loop_data.mode,
     *  "_m_for_loop_data.additional_data",
     *  _m_children)"
     * 
     * @return The string representing the object.
     */
    __constexpr
        std::string
        print_repetition_tree(
        ) const noexcept;
    /*!
     * @brief Given a repetition_data_sequnece_t object, analysis the 
     * repetition_tree_node_t object and, if there is one available in the tree,
     * returns an incremented version of the last element in the 
     * repetition_data_sequence_t object.
     * 
     * The algorithm analysis the internal repetition_tree, and finds the place
     * of the given repetition_data_sequnece_t object. If there exists a "next"
     * version of the last element in _a_rds, it is returned. 
     * 
     * The reader may think that this function and find_next_for_loop could
     * be combined into one function. However, due to the way in which
     * these functions interact with the ranged-based for loop, these
     * must be separated, and called from different parts of the
     * gen_data_collection_iterator_t class. 
     * 
     * If _a_rds is empty, then the function returns an empty optional.
     * 
     * If there is no "next" element, the function returns an empty optional.
     * 
     * @param _a_rds The repetition_data_sequence_t object to increment.
     * @return The "incremented" version of the last element of _a_rds.
     */
    __constexpr
        std::optional<repetition_data_t>
        increment_last_index(
            const repetition_data_sequence_t& _a_rds
        ) const noexcept;
    /*!
     * @brief Given a repetition_data_sequence_t object _a_rds, tells the caller
     * whether that _a_rds is contained within this repetition_tree.
     * 
     * @param _a_rds The reptition_data_stequence_t object to test against.
     * @return True if _a_rds is contained in the tree, false if not.
     */
    __constexpr
        bool
        is_repetition_to_be_repeated(
            const repetition_data_sequence_t& _a_rds
        ) const noexcept;
    /*!
     * @brief Adds a repetition_data_sequence_t object to the object.
     * 
     * This is the function which is used to build repetition_tree_node_t instances;
     * they are built up from processing successive repetition_data_sequence_t objects.
     * 
     * @param _a_rds The repetition_data_sequence_t object to add to the object.
     */
    __constexpr
        void
        add_repetition(
            const repetition_data_sequence_t& _a_rds
        ) noexcept;
    /*!
     * @brief Finds the next element to be added to the repetition_data_sequence_t
     * given as the argument.
     * 
     * This function essentially analysis the given repetition_data_sequence_t
     * object _a_rds, and finds whether there is a "next" for loop for it;
     * in essence, whether it has any children in the tree. 
     * 
     * If given an empty repetition_data_sequence_t, then this function
     * simply finds the first element in the first for loop.
     * 
     * @param _a_rds repetition_data_sequence_t to find the next element for.
     * @return Nullopt if the element has no child, a repetition_data_t object 
     * representing it if it does.
     */
    __constexpr
        std::optional<repetition_data_t>
        find_next_for_loop(
            const repetition_data_sequence_t& _a_rds
        ) const noexcept;
    friend class repetition_tree_node_t<not Is_Root>;
    /*!
     * @brief Static function used to parse a string to a repetition_data_node_t element.
     * @tparam Is_Root Whether the element created is a root node.
     * @param _a_str The input string.
     * @return An expected value; either the constructed repetition_tree_node_t 
     * or a std::string error message. 
     */
    template<
        bool Is_Root
    >
    __constexpr
        friend
        std::expected<repetition_tree_node_t<Is_Root>, std::string>
        parse_compressed_repetition_tree_node(
            const std::string_view _a_str
        ) noexcept;
private:
    std::conditional_t<not Is_Root, for_loop_iteration_data_t, std::monostate> _m_for_loop_data;
    std::vector<std::vector<std::shared_ptr<repetition_tree_node_t<false>>>> _m_children;
    template < typename = typename std::enable_if<not Is_Root>::type >
    /*!
    * @brief Constructor taking for_loop_iteration_data_t element.
    *
    * This class can only be used when the element is not the root of a tree -
    * as the root has no _m_for_loop_data member variable to set.
    */
    __constexpr
        repetition_tree_node_t(
            const for_loop_iteration_data_t& _a_flid
        ) noexcept;
    __constexpr
        std::optional<repetition_data_t>
        increment_last_index(
            const repetition_data_sequence_t& _a_rds,
            const std::size_t _a_idx
        ) const noexcept;
    __constexpr
        std::optional<repetition_data_t>
        find_next_for_loop(
            const repetition_data_sequence_t& _a_rds,
            const std::size_t _a_idx
        ) const noexcept;
    __constexpr
        bool
        is_repetition_to_be_repeated(
            const repetition_data_sequence_t& _a_rds,
            const std::size_t _a_idx
        ) const noexcept;
    __constexpr
        void
        add_repetition(
            const repetition_data_sequence_t& _a_rds,
            const std::size_t _a_idx
        ) noexcept;
};
using repetition_tree_t = repetition_tree_node_t<true>;
_END_ABC_DS_NS

template<
    typename bool Is_Root
>
struct fmt::formatter<abc::ds::repetition_tree_node_t<Is_Root>> : formatter<string_view>
{
    /*!
    * Provides a formatter for a poset_setup_test_data_t object
    */
    //Can't be constexpr due to use of fmt
    __no_constexpr
        auto
        format(
            abc::ds::repetition_tree_node_t<Is_Root> _a_rt,
            format_context& _a_cxt
        ) const
        ->format_context::iterator;
};

_BEGIN_ABC_DS_NS
template<
    typename bool Is_Root
>
__constexpr_imp
std::string
repetition_tree_node_t<Is_Root>::print_repetition_tree_compressed(
) const noexcept
{
    using namespace std;
    string _l_rv{ print_repetition_tree() };
    vector<unsigned char> _l_chars;
    for (char _l_char : _l_rv)
    {
        _l_chars.push_back(static_cast<unsigned char>(_l_char));
    }
    _l_rv.clear();
    for (unsigned char _l_char : _l_chars)
    {
        _l_rv.append(fmt::format("{:x}", _l_char));
    }
    return _l_rv;
}
/*!
* Prints a repretition tree without a test options.
*/
template<
    typename bool Is_Root
>
__constexpr_imp
std::string
repetition_tree_node_t<Is_Root>::print_repetition_tree(
) const noexcept
{
    using namespace std;
    using namespace utility::str;
    string _l_children_str{ _c_l_square_bracket };
    for (size_t _l_idx{ 0 }; _l_idx < _m_children.size(); ++_l_idx)
    {
        vector<shared_ptr< repetition_tree_node_t<false>>> _l_for_loop_at_idx{ _m_children[_l_idx] };
        _l_children_str.append(_c_l_square_bracket);
        for (size_t _l_jdx{ 0 }; _l_jdx < _l_for_loop_at_idx.size(); ++_l_jdx)
        {
            _l_children_str.append(_l_for_loop_at_idx[_l_jdx]->print_repetition_tree());
            _l_children_str.append(_l_jdx + 1 < _l_for_loop_at_idx.size() ? _c_comma : "");
        }
        _l_children_str.append(_c_r_square_bracket);
        _l_children_str.append(_l_idx + 1 < _m_children.size() ? _c_comma : "");
    }
    _l_children_str.append(_c_r_square_bracket);
    if constexpr (Is_Root)
    {
        return _l_children_str;
    }
    else
    {
        return fmt::format("({0},{1},\"{2}\",{3})",
            _m_for_loop_data.generation_collection_index,
            _m_for_loop_data.mode,
            _m_for_loop_data.additional_data,
            _l_children_str);
    }
}
template<
    typename bool Is_Root
>
__constexpr_imp
std::optional<repetition_data_t>
repetition_tree_node_t<Is_Root>::increment_last_index(
    const repetition_data_sequence_t& _a_rds
) const noexcept
{
    return increment_last_index(_a_rds, 0);
}
template<
    typename bool Is_Root
>
__constexpr_imp
bool
repetition_tree_node_t<Is_Root>::is_repetition_to_be_repeated(
    const repetition_data_sequence_t& _a_rds
) const noexcept
{
    return is_repetition_to_be_repeated(_a_rds, 0);
}
template<
    typename bool Is_Root
>
__constexpr_imp
void
repetition_tree_node_t<Is_Root>::add_repetition(
    const repetition_data_sequence_t& _a_rds
) noexcept
{
    add_repetition(_a_rds, 0);
}
template<
    typename bool Is_Root
>
__constexpr_imp
std::optional<repetition_data_t>
repetition_tree_node_t<Is_Root>::find_next_for_loop(
    const repetition_data_sequence_t& _a_rds
) const noexcept
{
    return find_next_for_loop(_a_rds, 0);
}
template<
    typename bool Is_Root
>
template < typename>
__constexpr_imp
repetition_tree_node_t<Is_Root>::repetition_tree_node_t(
    const for_loop_iteration_data_t& _a_flid
) noexcept
    : _m_for_loop_data(_a_flid)
{

}
template<
    typename bool Is_Root
>
__constexpr_imp
std::optional<repetition_data_t>
repetition_tree_node_t<Is_Root>::increment_last_index(
    const repetition_data_sequence_t& _a_rds,
    const std::size_t _a_idx
) const noexcept
{
    using namespace std;
    using enum utility::internal::internal_log_enum_t;
    if (_a_idx >= _a_rds.size())
    {
        return optional<repetition_data_t>{};
    }
    else if (_a_idx + 1 >= _a_rds.size())
    {
        const repetition_data_t& _l_rd{ _a_rds[_a_idx] };
        vector < shared_ptr<repetition_tree_node_t<false>>> _l_rtn{ _m_children[_l_rd.for_loop_index] };
        size_t _l_idx{ 0 };
        for (const shared_ptr<repetition_tree_node_t<false>>& _l_rfl : _l_rtn)
        {
            if (_l_rfl->_m_for_loop_data == _l_rd.for_loop_iteration_data &&
                _l_idx + 1 < _l_rtn.size())
            {
                return repetition_data_t{
                    _l_rd.for_loop_index,_l_rtn[_l_idx + 1]->_m_for_loop_data };
            }
            _l_idx++;
        }
    }
    else
    {
        const repetition_data_t& _l_rd{ _a_rds[_a_idx] };
        vector < shared_ptr<repetition_tree_node_t<false>>> _l_rtn{ _m_children[_l_rd.for_loop_index] };
        for (const shared_ptr<repetition_tree_node_t<false>>& _l_rfl : _l_rtn)
        {
            if (_l_rfl.get()->_m_for_loop_data == _l_rd.for_loop_iteration_data)
            {
                return _l_rfl->increment_last_index(_a_rds, _a_idx + 1);
            }
        }
        return std::optional<repetition_data_t>{};
    }
    return std::optional<repetition_data_t>{};
}
template<
    typename bool Is_Root
>
__constexpr_imp
std::optional<repetition_data_t>
repetition_tree_node_t<Is_Root>::find_next_for_loop(
    const repetition_data_sequence_t& _a_rds,
    const std::size_t _a_idx
) const noexcept
{
    using namespace std;
    using enum utility::internal::internal_log_enum_t;
    if (_a_idx >= _a_rds.size())
    {
        if (_m_children.size() > 0 &&
            _m_children[0].size() > 0)
        {
            return repetition_data_t{ 0,_m_children[0][0].get()->_m_for_loop_data };
        }
        else
        {
            return optional<repetition_data_t>{};
        }
    }
    else
    {
        const repetition_data_t& _l_rd{ _a_rds[_a_idx] };
        vector < shared_ptr<repetition_tree_node_t<false>>> _l_rtn{ _m_children[_l_rd.for_loop_index] };
        for (const shared_ptr<repetition_tree_node_t<false>>& _l_rfl : _l_rtn)
        {
            if (_l_rfl.get()->_m_for_loop_data == _l_rd.for_loop_iteration_data)
            {
                return _l_rfl->find_next_for_loop(_a_rds, _a_idx + 1);
            }
        }
        return std::optional<repetition_data_t>{};
    }
    return std::optional<repetition_data_t>{};
}
template<
    typename bool Is_Root
>
__constexpr_imp
bool
repetition_tree_node_t<Is_Root>::is_repetition_to_be_repeated(
    const repetition_data_sequence_t& _a_rds,
    const std::size_t _a_idx
) const noexcept
{
    using namespace std;
    using enum utility::internal::internal_log_enum_t;
    if (_a_idx >= _a_rds.size())
    {
        return false;
    }
    else if (_a_idx + 1 >= _a_rds.size())
    {
        const repetition_data_t& _l_rd{ _a_rds[_a_idx] };
        size_t _l_idx{ 0 };
        vector < shared_ptr<repetition_tree_node_t<false>>> _l_rtn{ _m_children[_l_rd.for_loop_index] };
        for (const shared_ptr<repetition_tree_node_t<false>>& _l_rfl : _l_rtn)
        {
            if (_l_rfl.get()->_m_for_loop_data == _l_rd.for_loop_iteration_data)
            {
                return true;
            }
            _l_idx++;
        }
        return false;
    }
    else
    {
        const repetition_data_t& _l_rd{ _a_rds[_a_idx] };
        vector < shared_ptr<repetition_tree_node_t<false>>> _l_rtn{ _m_children[_l_rd.for_loop_index] };
        for (const shared_ptr<repetition_tree_node_t<false>>& _l_rfl : _l_rtn)
        {
            if (_l_rfl.get()->_m_for_loop_data == _l_rd.for_loop_iteration_data)
            {
                return _l_rfl->is_repetition_to_be_repeated(_a_rds, _a_idx + 1);
            }
        }
        return false;
    }
}
template<
    typename bool Is_Root
>
__constexpr_imp
void
repetition_tree_node_t<Is_Root>::add_repetition(
    const repetition_data_sequence_t& _a_rds,
    const std::size_t _a_idx
) noexcept
{
    using namespace std;
    using enum utility::internal::internal_log_enum_t;
    _LIBRARY_LOG(REPETITION_INFO, fmt::format("Adding repetition sequence {0}", _a_rds));
    if (_a_idx < _a_rds.size())
    {
        const repetition_data_t& _l_rd{ _a_rds[_a_idx] };
        if (_l_rd.for_loop_index >= _m_children.size())
        {
            _m_children.resize(_l_rd.for_loop_index + 1);
        }
        vector < shared_ptr<repetition_tree_node_t<false>>>& _l_rtn{ _m_children[_l_rd.for_loop_index] };
        for (const shared_ptr<repetition_tree_node_t<false>>& _l_rfl : _l_rtn)
        {
            if (_l_rfl->_m_for_loop_data == _l_rd.for_loop_iteration_data)
            {
                _l_rfl->add_repetition(_a_rds, _a_idx + 1);
                return;
            }
        }
        shared_ptr < repetition_tree_node_t<false>> _l_new_child{
            shared_ptr<repetition_tree_node_t<false>>(new repetition_tree_node_t<false>(_l_rd.for_loop_iteration_data)) };
        vector < shared_ptr<repetition_tree_node_t<false>>>::iterator _l_x =
            upper_bound(_l_rtn.begin(), _l_rtn.end(), _l_new_child,
                [](const auto& _a_left, const auto& _a_right) {
                    return _a_left->_m_for_loop_data < _a_right->_m_for_loop_data; });
        vector < shared_ptr<repetition_tree_node_t<false>>>::iterator _l_itt = _l_rtn.insert(
            _l_x,
            _l_new_child
        );
        (*_l_itt)->add_repetition(_a_rds, _a_idx + 1);
    }
}
template<
    bool Is_Root
>
__constexpr_imp
std::expected<repetition_tree_node_t<Is_Root>, std::string>
parse_compressed_repetition_tree_node(
    const std::string_view _a_str
) noexcept
{
    using namespace utility::str;
    using namespace std;
    using enum utility::internal::internal_log_enum_t;
    std::string _l_str{};
    
    //Parse the bytes.
    const char* _l_position = _a_str.data();
    vector<unsigned char> _l_chars;
    for (size_t _l_idx{ 0 }; _l_idx < (_a_str.size() / 2); ++_l_idx)
    {
        unsigned char value;
        if (_l_idx * 2 + 1 >= _a_str.size())
        {
            return unexpected(
                fmt::format(
                    "Expected pairs of bytes, there was no element at index {0} after element {1}",
                    _l_idx * 2, *_l_position));
        }
        auto [_l_ptr, _l_ec] = from_chars(_l_position, _l_position + 2, value, 16);
        _l_position += 2;
        //If no error, pass the byte to the list of chars
        if (_l_ec == errc())
        {
            _l_chars.push_back(value);
        }
        else
        {
            //Else return failure
            return unexpected{
                fmt::format("Could not parse byte pair {0}{1}", *_l_position, *(_l_position + 1)) };
        }
    }
    //Add all the chars to the string.
    _l_str = string(_l_chars.begin(), _l_chars.end());
    _LIBRARY_LOG(PARSING_SEED, fmt::format("Depth {0}. Repetition tree after conversion from hex is in form \"{1}\"",
        _a_depth, _l_str));

    //Separete the string into strings...
    size_t _l_current_pos{ 0 };
    size_t _l_mode{ 0 };
    size_t _l_start{ 0 };
    vector<vector<string>> _l_strs;
    size_t _l_depth;
    size_t _l_found_pos;
    bool _l_end;
    char _l_char;
    size_t _l_previous_mode;
    string _l_error_string;
    size_t _l_old_pos;
    constexpr std::size_t _l_mode_zero_next_mode =
        Is_Root ? 1 : 2;
    while (_l_current_pos < _l_str.size())
    {
        _l_previous_mode = _l_mode;
        _l_old_pos = _l_current_pos;
        switch (_l_mode)
        {
        case 0:
            //Searching for opening bracket
            locate_relevant_character(_l_str, _c_l_square_bracket, _l_current_pos, _l_error_string);
            process_mode(_l_mode, _l_current_pos, _l_mode_zero_next_mode);
            break;
        case 1:
            //Mode 1, searhicng for a round bracket to begin a tuple of data.
            locate_relevant_character(_l_str, _c_l_round_bracket, _l_current_pos, _l_error_string);
            process_mode(_l_mode, _l_current_pos, 2);
            _l_start = _l_current_pos;
            _l_depth = 1;
            _l_start = _l_current_pos;
            break;
        case 2:
            process_list_elements(_l_str, _l_current_pos, _l_error_string, _l_depth, _l_strs[0], _l_start, _l_mode, 3, 4);
            //Check for end brackets, commas and begin next brackets.
            break;
        case 3:
            //Escaping quotes
            process_string(_l_str, _l_current_pos, _l_error_string, _l_mode, 2);
            break;
        case 4:
            //Searching for next comma or end square bracket (, or ])
            process_end_of_list(_l_str, _l_current_pos, _l_error_string, _l_mode, 1);
            break;
        default:
            return unexpected("default case");
        }
        if (not _l_error_string.empty())
        {
            return unexpected(_l_error_string);
        }
        _l_current_pos++;
    }

    //Put it all together
    repetition_tree_node_t<Is_Root> _l_rv;
    for (const vector<string>& _l_str : _l_strs)
    {
        vector < shared_ptr<repetition_tree_node_t<false>>> _l_kids;
        for (const string& _l_st : _l_str)
        {
            const expected<repetition_tree_node_t<false>, string> _l_op{ parse_compressed_repetition_tree_node<false>(_l_st) };
            if (_l_op.has_value())
            {
                _l_kids.push_back(
                    shared_ptr<repetition_tree_node_t<false>>(
                        new repetition_tree_node_t<false>(_l_op.value())));
            }
            else
            {
                return unexpected("");
            }
        }
        _l_rv._m_children.push_back(_l_kids);
    }
    return _l_rv;
}
_END_ABC_DS_NS