#pragma once
#include "abc_test/internal/ds/data_generator_memoization/data_generator_memoized_element.h"
#include "abc_test/internal/ds/type_synonyms.h"
#include "abc_test/internal/errors/test_library_exception.h"
#include "abc_test/internal/utility/str/parser_utility.h"
#include "abc_test/internal/utility/str/string_utility.h"
#include "fmt/base.h"
#include "fmt/ranges.h"

#include <charconv>
#include <exception>
#include <functional>

_BEGIN_ABC_DS_NS
// Forward declare
class typeless_data_generator_collection_stack_trie_t;
/*!
 * @brief Type synonym representing the result from parsing a string to a
 * for_loop_stack_trie_t object.
 */

using parse_for_loop_stack_trie_result_t
    = std::expected<typeless_data_generator_collection_stack_trie_t, std::string>;

namespace
{
// Type synonym for the child of a repetition_tree_node_t.
using for_loop_stack_trie_child_t
    = std::shared_ptr<typeless_data_generator_collection_stack_trie_t>;
// Type synonym for a list of rep_tree_child_t elements.
using for_loop_stack_trie_children_t = std::vector<for_loop_stack_trie_child_t>;
// Type synonym for the for-loop-level container of rep_tree_children_t objects.
using for_loop_stack_trie_indexed_children_t
    = std::vector<for_loop_stack_trie_children_t>;
// Type synonym for an optional pointer to a child node of a
// repetition_tree_node_t object.
using opt_itt_t = std::optional<for_loop_stack_trie_children_t::const_iterator>;
// type synonym for a pair of iterators; typically one is an iterator as
// opt_itt_t, and the other is an iterator pointing to the end of the collection
// that element is in.
using opt_itt_and_end_t = std::optional<std::pair<
    for_loop_stack_trie_children_t::const_iterator,
    for_loop_stack_trie_children_t::const_iterator>>;
} // namespace

/*!
 * @brief This object represents a trie of stacks of gen_data_collection_t
 * elements. In essence it can be considered a set of instructions of which data
 * elements to generate from a set of gen_data_collection_t objects, contained
 * in a tree structure.
 *
 * This may appear to be an obscure data structure, but when a user is ordering
 * gen_data_t for loops in a test, this structure allows the system to keep
 * track of which data values have failures associated with them.
 *
 * The structure itself contains gen_collection_creation_data_t as its node data
 * (the root should contain no data), and a 2D vector containing
 * repetition_tree_node_t elements as the children.
 *
 * The root node should not contain node data as there is no
 * gen_data_collection_t element at the start of a test.
 *
 * We go to great care to ensure that the root node data is never accessed.
 * While we could have used a conditional variable and a template to ensure it
 * doesn't exist, doing so would make our code more compcliated as our in-place
 * algorithms would need to be duplicated. Instead we chose to ensure the root
 * data variable is not accessed.
 *
 * In the 2D vector, the outer for loop contains all the sets of data indexed by
 * the for loop index. The inner data is the collection of elements in that for
 * loop index. We use a 2D vector as the outer contains all the data indexed by
 * the for loop index.
 */
class typeless_data_generator_collection_stack_trie_t
{
public:
    /*!
     * @brief Default constructor.
     *
     * Only available public constructor.
     */
    __constexpr
        typeless_data_generator_collection_stack_trie_t() noexcept
        = default;
    /*!
     * @brief Returns a string representing the for_loop_stack_trie_t object in
     * a compressed format.
     *
     * Currently the compressed format is created by taking the output from
     * print_for_loop_stack_trie and turning it into hex.
     *
     * @return Compressed string representing the for_loop_stack_trie_t object.
     */
    __no_constexpr std::string
                   print_for_loop_stack_trie_compressed() const noexcept;
    /*!
     * @brief Returns a string representing the for_loop_stack_trie_t object.
     *
     * It prints a ndoe's generation_collection_index, mode and
     * additional data, as well as its children.
     *
     * Note for a root note, it will only print the children.
     *
     * @return String representing the for_loop_stack_trie_t object.
     */
    __no_constexpr std::string
                   print_for_loop_stack_trie() const noexcept;
    /*!
     * @brief This function firstly finds the trie node representing the
     * for_loop_creation_data_sequence_t argument. It then finds that node's
     * first successor sibling s, and returns the for_loop_creation_data_t which
     * would need to be substituted with the last element in the
     * for_loop_creation_data_sequence_t argument to create s.
     *
     * By "successor sibling", we mean a sibling which is found after the node
     * in an in-order traversal.
     *
     * The reader may think that this function and find_first_child_of_trie
     * could be combined into one function, so as to act as an iterator on the
     * tree. However, in our use-case, we need these functions to be separated.
     * This is because of how they interact with user-defined tests; in essence
     * increment_last_index is called when incrementing an iterator in a for
     * loop, and find_first_child_of_trie is used when a new for loop is
     * entered.
     *
     * If the argument is empty or there is no "next" element, the function
     * returns an empty optional.
     *
     * @param _a_rds The for_loop_creation_data_sequence_t object used to find
     * the successor element in the tree.
     * @return The opt_for_loop_creation_data_t object used to create the
     * successor sibling.
     */
    __constexpr opt_idgc_memoized_element_t
        increment_last_index(const idgc_memoized_element_sequence_t& _a_rds
        ) const noexcept;
    /*!
     * @brief This function finds the node representing the
     * for_loop_creation_data_sequence_t argument. If it exists in the tree,
     * then this function returns true. Else, it returns false.
     *
     * @param _a_rds The for_loop_creation_data_sequence_t object to test.
     * @return True if _a_rds is in the tree. False otherwise.
     */
    __constexpr bool
        is_sequence_in_trie(const idgc_memoized_element_sequence_t& _a_rds
        ) const noexcept;
    /*!
     * @brief Adds a for_loop_creation_data_sequence_t object to the underlying
     * structure.
     *
     * This functin takes the for_loop_creation_data_sequence_t argument, finds
     * any initial sequence which already exists in the tree, and navigates to
     * that node. It then adds nodes representing each remaining
     * repetition_data_t element in the for_loop_creation_data_sequence_t
     * argument until the tree has a representation of the
     * for_loop_creation_data_sequence_t argument within it.
     *
     *
     * @param _a_rds The for_loop_creation_data_sequence_t object to add to the
     * structure.
     */
    __constexpr void
        add_for_loop_creation_data_sequence(
            const idgc_memoized_element_sequence_t& _a_flcds
        ) noexcept;
    /*!
     * @brief This function firstly finds the tree node representing the
     * for_loop_creation_data_sequence_t argument. It then finds the first child
     * node of that element, and returns the for_loop_creation_data_t element
     * which would have to be appended to the for_loop_creation_data_sequence_t
     * argument to create it.
     *
     * If the node has no children, this function returns a nullopt.
     *
     * If the for_loop_creation_data_sequence_t is empty, and there are any
     * elements in the tree, this function will return the first of them.
     *
     * If the for_loop_creation_data_sequence_t argument is not represented in
     * the tree, then the function returns a nullopt.
     *
     * @param _a_rds The for_loop_creation_data_sequence_t object used to find
     * the next element in the tree.
     * @return The opt_for_loop_creation_data_t object used to create _a_rds's
     * first child node.
     */
    __constexpr opt_idgc_memoized_element_t
        find_first_child_of_sequence_in_trie(
            const idgc_memoized_element_sequence_t& _a_rds
        ) const noexcept;
    friend class itdg_collection_stack_trie_t;
    /*!
     * @brief Parses a string into a for_loop_stack_trie_t element.
     *
     * This function assumes the string has been written using
     * print_for_loop_stack_trie_compressed.
     *
     * @param _a_str The string to parse.
     * @return An parse_for_loop_stack_trie_result_t. If the string could be
     * parsed, it is a for_loop_stack_trie_t element. Otherwise it returns
     * nullopt.
     */

    friend __constexpr parse_for_loop_stack_trie_result_t
        parse_compressed_repetition_tree_node(const std::string_view _a_str
        ) noexcept;
private:
    /*!
     * @brief Conditional variable used to hold node data.
     *
     * Only variables which are not the root hold gen_collection_creation_data_t
     * elements, hence the use of the conditional.
     */
    dgc_memoized_element_t                 _m_for_loop_data;
    for_loop_stack_trie_indexed_children_t _m_children;
    /*!
     * @brief Constructor taking gen_collection_creation_data_t element.
     *
     * This class can only be used when the element is not the root of a tree -
     * as the root has no _m_for_loop_data member variable to set.
     */
    __constexpr
        typeless_data_generator_collection_stack_trie_t(const dgc_memoized_element_t& _a_flid) noexcept;
    /*!
     * @brief Finds the element represented by the argument _a_flcds, as well as
     * that element's end iterator element.
     * @param _a_flcds for_loop_creation_data_sequence_t argument used to find
     * the node.
     * @return An optional pair of iterators; nullopt if no node exists
     * representing _a_flcds; otherwise a pair of iterators. The first is an
     * iterator poitning to the node. The second is an iterator pointing to the
     * end of the list the node is contained in.
     */
    __constexpr opt_itt_and_end_t
        find_iterator_and_end(const idgc_memoized_element_sequence_t& _a_flcds
        ) const noexcept;
    /*!
     * @brief If one exists, finds the node which represents the
     * for_loop_creation_data_sequence_t argument.
     *
     * @param _a_flcds for_loop_creation_data_sequence_t argument used to find
     * the node.
     * @return An optional iterator pointing at the node which represents
     * _a_flcds. If nullopt, no node exists.
     */
    __constexpr opt_itt_t
        find_iterator(const idgc_memoized_element_sequence_t& _a_flcds
        ) const noexcept;
    /*!
     * @brief Core printing function.
     *
     * @param _a_is_root Denotes whether the printing functions is being called
     * on the root.
     * @return std::string representing the for_loop_stack_trie.
     */
    __no_constexpr std::string
        inner_print_for_loop_stack_trie(const bool _a_is_root = false)
            const noexcept;
};

using tdg_collection_stack_trie_t = typeless_data_generator_collection_stack_trie_t;
_END_ABC_DS_NS

template <>
struct fmt::formatter<_ABC_NS_DS::typeless_data_generator_collection_stack_trie_t>
    : formatter<string_view>
{
    /*!
     * Provides a formatter for a poset_setup_test_data_t object
     */
    // Can't be constexpr due to use of fmt
    __no_constexpr auto
        format(
            _ABC_NS_DS::typeless_data_generator_collection_stack_trie_t _a_rt,
            format_context&                      _a_cxt
        ) const -> format_context::iterator;
};

_BEGIN_ABC_DS_NS

__no_constexpr_imp std::string
typeless_data_generator_collection_stack_trie_t::print_for_loop_stack_trie_compressed(
    ) const noexcept
{
    using namespace std;
    string                _l_rv{print_for_loop_stack_trie()};
    return abc::utility::str::to_hex(_l_rv);
}

__no_constexpr_imp std::string
typeless_data_generator_collection_stack_trie_t::print_for_loop_stack_trie() const noexcept
{
    return inner_print_for_loop_stack_trie(true);
}

__constexpr_imp opt_idgc_memoized_element_t
typeless_data_generator_collection_stack_trie_t::increment_last_index(
        const idgc_memoized_element_sequence_t& _a_rds
    ) const noexcept
{
    using namespace std;
    if (_a_rds.size() == 0)
    {
        return opt_idgc_memoized_element_t{};
    }
    else
    {
        if (const opt_itt_and_end_t & _l_opt_itt{find_iterator_and_end(_a_rds)};
            _l_opt_itt.has_value())
        {
            auto& [_l_itt, _l_end]{_l_opt_itt.value()};
            if (_l_itt + 1 != _l_end)
            {
                return opt_idgc_memoized_element_t{
                    idgc_memoized_element_t{
                                            _a_rds.back().for_loop_index,
                                            (*(_l_itt + 1))->_m_for_loop_data
                    }
                };
            }
            else
            {
                return opt_idgc_memoized_element_t{};
            }
        }
        else
        {
            return opt_idgc_memoized_element_t{};
        }
    }
}

__constexpr_imp bool
typeless_data_generator_collection_stack_trie_t::is_sequence_in_trie(
        const idgc_memoized_element_sequence_t& _a_rds
    ) const noexcept
{
    return find_iterator(_a_rds).has_value();
}

__constexpr_imp void
typeless_data_generator_collection_stack_trie_t::add_for_loop_creation_data_sequence(
        const idgc_memoized_element_sequence_t& _a_flcds
    ) noexcept
{
    using namespace std;
    std::reference_wrapper<tdg_collection_stack_trie_t> _l_current_ref{*this};
    // Go through each flcd, either creating or navigating as we go.
    for (const idgc_memoized_element_t& _l_flcd : _a_flcds)
    {
        const size_t _l_for_loop_idx{_l_flcd.for_loop_index};
        for_loop_stack_trie_indexed_children_t& _l_for_loop_children{
            _l_current_ref.get()._m_children
        };
        // Resize if this for loop isn't big enough
        if (_l_for_loop_idx >= _l_for_loop_children.size())
        {
            _l_for_loop_children.resize(_l_for_loop_idx + 1);
        }
        for_loop_stack_trie_children_t& _l_children{
            _l_for_loop_children[_l_for_loop_idx]
        };
        if (_l_children.size() == 0 || _l_children.back()->_m_for_loop_data !=
            _l_flcd.for_loop_iteration_data)
        {
            _l_children.push_back(make_unique<tdg_collection_stack_trie_t>(
                tdg_collection_stack_trie_t(
                    _l_flcd.for_loop_iteration_data
                )
            ));
            _l_current_ref = ref(*_l_children.back());
        }
        else
        {
            _l_current_ref = ref(*_l_children.back());
        }

        // Find the equal element; or the subragne around them.
        /*ranges::subrange<for_loop_stack_trie_children_t::iterator>
            _l_equal_subrange{ranges::equal_range(
                _l_children,
                _l_flcd.for_loop_iteration_data,
                ranges::less{},
                [](const for_loop_stack_trie_child_t& _a_flstc)
                {
                    return _a_flstc->_m_for_loop_data;
                }
            )};
        // If they are equal (both greater than the element - no element in the
        // list).
        if (_l_equal_subrange.begin() == _l_equal_subrange.end())
        {
            // So time to insert.
            for_loop_stack_trie_children_t::iterator _l_inserted_itt{
                _l_children.insert(
                    _l_equal_subrange.end(),
                    make_unique<tdg_collection_stack_trie_t>(
                        tdg_collection_stack_trie_t(
                            _l_flcd.for_loop_iteration_data
                        )
                    )
                )
            };
            // Set the new current ref to the inserted element.
            _l_current_ref = ref(*((*_l_inserted_itt).get()));
        }
        else
        {
            // Element already in, just navigate to it.
            _l_current_ref = ref(*((*_l_equal_subrange.begin()).get()));
        }*/
    }
}

__constexpr_imp opt_idgc_memoized_element_t
typeless_data_generator_collection_stack_trie_t::find_first_child_of_sequence_in_trie(
        const idgc_memoized_element_sequence_t& _a_rds
    ) const noexcept
{
    if (const opt_itt_t _l_opt_itt{find_iterator(_a_rds)};
        _l_opt_itt.has_value())
    {
        const for_loop_stack_trie_indexed_children_t& _l_children{
            _l_opt_itt.value()->get()->_m_children
        };
        if (_l_children.size() > 0 && _l_children[0].size() > 0)
        {
            return opt_idgc_memoized_element_t{
                idgc_memoized_element_t{
                                        _a_rds.size() == 0 ? 0 : _a_rds.back().for_loop_index,
                                        _l_children[0][0]->_m_for_loop_data
                }
            };
        }
        else
        {
            return opt_idgc_memoized_element_t{};
        }
    }
    else
    {
        return opt_idgc_memoized_element_t{};
    }
}

__constexpr_imp
typeless_data_generator_collection_stack_trie_t::typeless_data_generator_collection_stack_trie_t(
        const dgc_memoized_element_t& _a_flid
    ) noexcept
    : _m_for_loop_data(_a_flid)
{}

__constexpr_imp opt_itt_and_end_t
typeless_data_generator_collection_stack_trie_t::find_iterator_and_end(
        const idgc_memoized_element_sequence_t& _a_flcds
    ) const noexcept
{
    using namespace std;
    std::reference_wrapper<const tdg_collection_stack_trie_t> _l_current_ref{
        *this
    };
    // Go through each flcd, either creating or navigating as we go.
    for (const idgc_memoized_element_t& _l_flcd : _a_flcds)
    {
        const size_t _l_for_loop_idx{_l_flcd.for_loop_index};
        const for_loop_stack_trie_indexed_children_t& _l_for_loop_children{
            _l_current_ref.get()._m_children
        };
        // Resize if this for loop isn't big enough
        if (_l_for_loop_idx >= _l_for_loop_children.size())
        {
            return opt_itt_and_end_t{};
        }
        else
        {
            const for_loop_stack_trie_children_t& _l_children{
                _l_for_loop_children[_l_for_loop_idx]
            };
            // Find the equal element; or the subragne around them.
            ranges::subrange<for_loop_stack_trie_children_t::const_iterator>
                _l_equal_subrange{ranges::equal_range(
                    _l_children,
                    _l_flcd.for_loop_iteration_data,
                    ranges::less{},
                    [](const for_loop_stack_trie_child_t& _a_flstc)
                    {
                        return _a_flstc->_m_for_loop_data;
                    }
                )};
            // If they are equal (both greater than the element - no element in
            // the list).
            if (_l_equal_subrange.begin() == _l_equal_subrange.end())
            {
                return opt_itt_and_end_t{};
            }
            else
            {
                // Element is in, just navigate to it!
                _l_current_ref = ref(*((*_l_equal_subrange.begin()).get()));
            }
        }
    }
    return opt_itt_and_end_t{};
}

__constexpr_imp opt_itt_t
typeless_data_generator_collection_stack_trie_t::find_iterator(
        const idgc_memoized_element_sequence_t& _a_flcds
    ) const noexcept
{
    if (const opt_itt_and_end_t _l_res{find_iterator_and_end(_a_flcds)};
        _l_res.has_value())
    {
        return opt_itt_t{_l_res.value().first};
    }
    else
    {
        return opt_itt_t{};
    }
}

__no_constexpr_imp std::string
typeless_data_generator_collection_stack_trie_t::inner_print_for_loop_stack_trie(
        const bool _a_is_root
    ) const noexcept
{
    using namespace std;
    using namespace utility::str;
    string _l_children_str{_c_l_square_bracket};
    for (size_t                                _l_idx{0};
         const for_loop_stack_trie_children_t& _l_for_loop_at_idx : _m_children)
    {
        _l_children_str.append(_c_l_square_bracket);
        for (size_t                             _l_jdx{0};
             const for_loop_stack_trie_child_t& _l_child : _l_for_loop_at_idx)
        {
            _l_children_str.append(_l_child->inner_print_for_loop_stack_trie());
            _l_children_str.append(return_str_if_next_index_in_bound(
                _l_jdx++, _l_for_loop_at_idx.size(), _c_comma
            ));
        }
        _l_children_str.append(_c_r_square_bracket);
        _l_children_str.append(return_str_if_next_index_in_bound(
            _l_idx++, _m_children.size(), _c_comma
        ));
    }

    _l_children_str.append(_c_r_square_bracket);
    if (_a_is_root)
    {
        return _l_children_str;
    }
    else
    {
        return fmt::format(
            "({0},{1},\"{2}\",{3})",
            _m_for_loop_data.generation_collection_index,
            _m_for_loop_data.flied.mode,
            _m_for_loop_data.flied.additional_data,
            _l_children_str
        );
    }
}

__constexpr_imp parse_for_loop_stack_trie_result_t
    parse_compressed_repetition_tree_node(
        const std::string_view _a_str
    ) noexcept
{
    using namespace utility::str;
    using namespace std;
    using enum utility::internal::internal_log_enum_t;
    expected<string, string> _l_hex_result{ abc::utility::str::from_hex(_a_str) };
    if (not _l_hex_result.has_value())
    {
        return unexpected(_l_hex_result.error());
    }
    else
    { 
        string _l_str = _l_hex_result.value();
        _LIBRARY_LOG(
            PARSING_SEED,
            fmt::format(
                "Depth {0}. Repetition tree after conversion from hex is "
                "in "
                "form \"{1}\"",
                _a_depth,
                _l_str
            )
        );

        // Separete the string into strings...
        size_t                 _l_current_pos{0};
        size_t                 _l_mode{0};
        size_t                 _l_start{0};
        vector<vector<string>> _l_strs;
        size_t                 _l_depth;
        size_t                 _l_found_pos;
        bool                   _l_end;
        char                   _l_char;
        size_t                 _l_previous_mode;
        string                 _l_error_string;
        size_t                 _l_old_pos;
        constexpr std::size_t  _l_mode_zero_next_mode = 1;
        while (_l_current_pos < _l_str.size())
        {
            _l_previous_mode = _l_mode;
            _l_old_pos       = _l_current_pos;
            switch (_l_mode)
            {
            case 0:
                // Searching for opening bracket
                locate_relevant_character(
                    _l_str, _c_l_square_bracket, _l_current_pos, _l_error_string
                );
                process_mode(_l_mode, _l_current_pos, _l_mode_zero_next_mode);
                break;
            case 1:
                // Mode 1, searhicng for a round bracket to begin a tuple of
                // data.
                locate_relevant_character(
                    _l_str, _c_l_round_bracket, _l_current_pos, _l_error_string
                );
                process_mode(_l_mode, _l_current_pos, 2);
                _l_start = _l_current_pos;
                _l_depth = 1;
                _l_start = _l_current_pos;
                break;
            case 2:
                process_list_elements(
                    _l_str,
                    _l_current_pos,
                    _l_error_string,
                    _l_depth,
                    _l_strs[0],
                    _l_start,
                    _l_mode,
                    3,
                    4
                );
                // Check for end brackets, commas and begin next brackets.
                break;
            case 3:
                // Escaping quotes
                process_string(
                    _l_str, _l_current_pos, _l_error_string, _l_mode, 2
                );
                break;
            case 4:
                // Searching for next comma or end square bracket (, or ])
                process_end_of_list(
                    _l_str, _l_current_pos, _l_error_string, _l_mode, 1
                );
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

        // Put it all together
        tdg_collection_stack_trie_t _l_rv;
        for (const vector<string>& _l_str : _l_strs)
        {
            vector<shared_ptr<tdg_collection_stack_trie_t>> _l_kids;
            for (const string& _l_st : _l_str)
            {
                const expected<tdg_collection_stack_trie_t, string> _l_op{
                    parse_compressed_repetition_tree_node(_l_st)
                };
                if (_l_op.has_value())
                {
                    _l_kids.push_back(shared_ptr<tdg_collection_stack_trie_t>(
                        new tdg_collection_stack_trie_t(_l_op.value())
                    ));
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
}

_END_ABC_DS_NS

__no_constexpr_imp auto
    fmt::formatter<_ABC_NS_DS::tdg_collection_stack_trie_t>::format(
        _ABC_NS_DS::tdg_collection_stack_trie_t _a_rt,
        format_context&                      _a_cxt
    ) const -> format_context::iterator
{
    using namespace std;
    const string _l_rv{fmt::format(
        "{0}"
        "{{{1} = {2}"
        "}}",
        typeid(_a_rt).name(),
        "_m_children",
        _a_rt.print_for_loop_stack_trie()
    )};
    return formatter<string_view>::format(_l_rv, _a_cxt);
}