#pragma once
#include "abc_test/core/ds/gen_data_memoization/gen_collection_creation_data.h"
#include "abc_test/core/ds/type_synonyms.h"
#include "abc_test/core/errors/test_library_exception.h"
#include "abc_test/utility/str/parser_utility.h"
#include "abc_test/utility/str/string_utility.h"
#include "fmt/base.h"
#include "fmt/ranges.h"

#include <charconv>
#include <exception>
#include <functional>

_BEGIN_ABC_DS_NS
// Forward declare
template <typename bool Is_Root>
class for_loop_stack_trie_t;
/*!
 * @brief Type synonym representing a rooted for_loop_stack_trie_t.
 */
using for_loop_stack_trie_root_t = for_loop_stack_trie_t<true>;
/*!
 * @brief Type synonym representing the result from parsing a string to a
 * for_loop_stack_trie_t object.
 * @tparam Is_Root Denotes whether the created object is a root tree node or
 * not.
 */
template <bool Is_Root>
using parse_for_loop_stack_trie_result_t
    = std::expected<for_loop_stack_trie_t<Is_Root>, std::string>;

namespace
{
// A synonym for the internal node type.
using for_loop_stack_trie_non_root_t = for_loop_stack_trie_t<false>;
// Type synonym for the child of a repetition_tree_node_t.
using for_loop_stack_trie_child_t
    = std::shared_ptr<for_loop_stack_trie_t<false>>;
// Type synonym for a list of rep_tree_child_t elements.
using for_loop_stack_trie_children_t = std::vector<for_loop_stack_trie_child_t>;
// Type synonym for the for-loop-level container of rep_tree_children_t objects.
using for_loop_stack_trie_indexed_children_t
    = std::vector<for_loop_stack_trie_children_t>;
// Type synonym for an optional pointer to a child node of a
// repetition_tree_node_t object.
using opt_itt_t = std::optional<for_loop_stack_trie_children_t::const_iterator>;
} // namespace

/*!
 * @brief This class is used to represent repetition trees, a structure used
 * in this library to store information about entities generated from gen_data_t
 * objects.
 *
 * The structure itself consists of node data in the form repetition_data_t
 * (except the root, which hhas no data), and 2D vector containing
 * repetition_tree_node_t elements. Each outer vector's index represents the
 * for loop index. The inner vector is the collection of entities at that index.
 *
 * A repetition tree can be thought of as a trie for data generated from
 * gen_data_t elements. Because each gen_data_t element can have multiple
 * children generated from differnet children, we organise the data using this
 * object.
 *
 * @tparam Is_Root Denotes whether an instance of this object is the root node
 * in a tree or not.
 */


/*!
 * @brief This object represents a trie of stacks of gen_data_collection_t
 * elements. In essence it can be considered a set of instructions of which data
 * elements to generate from which gen_data_collection_t objects.
 *
 * The structure itself contains gen_collection_creation_data_t as its node data
 * (except the root, which hhas no data), and a 2D vector containing
 * repetition_tree_node_t elements as the children.
 *
 * We use a 2D vector as the outer contains all the data indexed by the for loop
 * index.
 *
 * The root node does not contain node data as there is no gen_data_collection_t
 * element at the start of a test.
 *
 * @tparam Is_Root Denotes whether an instance of this object is the root node
 * in the trie or not.
 */
template <typename bool Is_Root>
class for_loop_stack_trie_t
{
public:
    /*!
     * @brief Default constructor.
     *
     * Only available public constructor.
     */
    __constexpr
    for_loop_stack_trie_t() noexcept
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
    __constexpr std::string
                print_for_loop_stack_trie_compressed() const noexcept;
    /*!
     * @brief Returns a string representing the for_loop_stack_trie_t object.
     *
     * The output differs depending on whether the object is a root or not.
     *
     * If it is, then it only prints out a list of the children.
     *
     * If it is not, then it also prints out the node data.
     *
     * @return String representing the for_loop_stack_trie_t object.
     */
    __constexpr std::string
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
    __constexpr opt_for_loop_creation_data_t
        increment_last_index(const for_loop_creation_data_sequence_t& _a_rds
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
        is_sequence_in_trie(const for_loop_creation_data_sequence_t& _a_rds
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
        add_for_loop_creation_data_sequence(const for_loop_creation_data_sequence_t& _a_rds
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
    __constexpr opt_for_loop_creation_data_t
        find_first_child_of_sequence_in_trie(const for_loop_creation_data_sequence_t& _a_rds
        ) const noexcept;
    friend class for_loop_stack_trie_t<not Is_Root>;
    /*!
     * @brief Parses a string into a for_loop_stack_trie_t element.
     *
     * This function assumes the string has been written using
     * print_for_loop_stack_trie_compressed.
     *
     * @tparam Is_Root The template of the returned for_loop_stack_trie_t
     * element.
     * @param _a_str The string to parse.
     * @return An parse_for_loop_stack_trie_result_t. If the string could be
     * parsed, it is a for_loop_stack_trie_t element. Otherwise it returns
     * nullopt.
     */
    template <bool Is_Root>
    friend __constexpr parse_for_loop_stack_trie_result_t<Is_Root>
        parse_compressed_repetition_tree_node(const std::string_view _a_str
        ) noexcept;
private:
    /*!
     * @brief Conditional variable used to hold node data.
     *
     * Only variables which are not the root hold gen_collection_creation_data_t
     * elements, hence the use of the conditional.
     */
    std::conditional_t<
        not Is_Root,
        gen_collection_creation_data_t,
        std::monostate>
                                           _m_for_loop_data;
    for_loop_stack_trie_indexed_children_t _m_children;
    /*!
     * @brief Constructor taking gen_collection_creation_data_t element.
     *
     * This class can only be used when the element is not the root of a tree -
     * as the root has no _m_for_loop_data member variable to set.
     */
    template <typename = typename std::enable_if<not Is_Root>::type>
    __constexpr
    for_loop_stack_trie_t(const gen_collection_creation_data_t& _a_flid
    ) noexcept;
    /*!
     * @brief Internal increment_last_index function. The
     * integer argument represents the element currently being processed in the
     * for_loop_creation_data_sequence_t argument.
     * @param _a_rds The for_loop_creation_data_sequence_t to use.
     * @param _a_idx The index of the current for_loop_creation_data_sequence_t
     * element.
     * @return Optional for_loop_creation_data_t, representing the "sucessor"
     * element in the tree. Nullopt if there is not one.
     */
    __constexpr opt_for_loop_creation_data_t
        increment_last_index(
            const for_loop_creation_data_sequence_t& _a_rds,
            const std::size_t                        _a_idx
        ) const noexcept;
    /*!
     * @brief Internal find_first_child_of_trie function. The
     * integer argument represents the element currently being processed in the
     * for_loop_creation_data_sequence_t argument.
     * @param _a_rds The for_loop_creation_data_sequence_t to use.
     * @param _a_idx The index of the current for_loop_creation_data_sequence_t
     * element.
     * @return Optional for_loop_creation_data_t, representing _a_rds's child
     * element in the tree. Nullopt if there is not one.
     */
    __constexpr opt_for_loop_creation_data_t
        find_first_child_of_sequence_in_trie(
            const for_loop_creation_data_sequence_t& _a_rds,
            const std::size_t                        _a_idx
        ) const noexcept;
    /*!
     * @brief Internal is_sequence_in_trie function. The
     * integer argument represents the element currently being processed in the
     * for_loop_creation_data_sequence_t argument.
     * @param _a_rds The for_loop_creation_data_sequence_t to use.
     * @param _a_idx The index of the current for_loop_creation_data_sequence_t
     * element.
     * @return True if the element is in the tree, false otherwise.
     */
    __constexpr bool
        is_sequence_in_trie(
            const for_loop_creation_data_sequence_t& _a_rds,
            const std::size_t                        _a_idx
        ) const noexcept;
    /*!
     * @brief Internal function to add a for_loop_creation_data_sequence_t to
     * the structure. The integer argument represents the element currently
     * being processed in the for_loop_creation_data_sequence_t argument.
     *
     * See add_for_loop_creation_data_sequence for more information.
     *
     * @param _a_rds The for_loop_creation_data_sequence_t argument.
     * @param _a_idx The index of the for_loop_creation_data_sequence_t
     * currently being considered.
     */
    __constexpr void
        add_for_loop_creation_data_sequence(
            const for_loop_creation_data_sequence_t& _a_rds,
            const std::size_t                        _a_idx
        ) noexcept;
    /*!
     * @brief If the for_loop_creation_data_t argument is represented in this nodes
     * chidlren, then this function returns it. Otherwise, it returns an empty
     * optional.
     *
     * As the elements are ordered in each vector, and for_loop_creation_data_t has an
     * exact for loop index, this function is O(log n), where n is the size of
     * the largest inner vector in the node's children.
     *
     * @param _a_rd The for_loop_creation_data_t representing the element to find in
     * the nodes children.
     * @return An optional iterator to the child which matches _a_rd. If there
     * is such an element, the iterator pointing to that element is returned.
     * Otherwise, a nullopt is returned.
     */
    __constexpr opt_itt_t
        get_itt_to_rep_tree_node(const for_loop_creation_data_t& _a_rd
        ) const noexcept;
};

_END_ABC_DS_NS

template <typename bool Is_Root>
struct fmt::formatter<abc::ds::for_loop_stack_trie_t<Is_Root>>
    : formatter<string_view>
{
    /*!
     * Provides a formatter for a poset_setup_test_data_t object
     */
    // Can't be constexpr due to use of fmt
    __no_constexpr auto
        format(
            abc::ds::for_loop_stack_trie_t<Is_Root> _a_rt,
            format_context&                         _a_cxt
        ) const -> format_context::iterator;
};

_BEGIN_ABC_DS_NS
template <typename bool Is_Root>
__constexpr_imp std::string
    for_loop_stack_trie_t<Is_Root>::print_for_loop_stack_trie_compressed(
    ) const noexcept
{
    using namespace std;
    string                _l_rv{print_for_loop_stack_trie()};
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
template <typename bool Is_Root>
__constexpr_imp std::string
    for_loop_stack_trie_t<Is_Root>::print_for_loop_stack_trie() const noexcept
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
            _l_children_str.append(_l_child->print_for_loop_stack_trie());
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
    if constexpr (Is_Root)
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

template <typename bool Is_Root>
__constexpr_imp opt_for_loop_creation_data_t
    for_loop_stack_trie_t<Is_Root>::increment_last_index(
        const for_loop_creation_data_sequence_t& _a_rds
    ) const noexcept
{
    return increment_last_index(_a_rds, 0);
}

template <typename bool Is_Root>
__constexpr_imp bool
    for_loop_stack_trie_t<Is_Root>::is_sequence_in_trie(
        const for_loop_creation_data_sequence_t& _a_rds
    ) const noexcept
{
    return is_sequence_in_trie(_a_rds, 0);
}

template <typename bool Is_Root>
__constexpr_imp void
    for_loop_stack_trie_t<Is_Root>::add_for_loop_creation_data_sequence(
        const for_loop_creation_data_sequence_t& _a_rds
    ) noexcept
{
    add_for_loop_creation_data_sequence(_a_rds, 0);
}

template <typename bool Is_Root>
__constexpr_imp opt_for_loop_creation_data_t
    for_loop_stack_trie_t<Is_Root>::find_first_child_of_sequence_in_trie(
        const for_loop_creation_data_sequence_t& _a_rds
    ) const noexcept
{
    return find_first_child_of_sequence_in_trie(_a_rds, 0);
}

template <typename bool Is_Root>
template <typename>
__constexpr_imp
    for_loop_stack_trie_t<Is_Root>::for_loop_stack_trie_t(
        const gen_collection_creation_data_t& _a_flid
    ) noexcept
    : _m_for_loop_data(_a_flid)
{}

template <typename bool Is_Root>
__constexpr_imp opt_for_loop_creation_data_t
    for_loop_stack_trie_t<Is_Root>::increment_last_index(
        const for_loop_creation_data_sequence_t& _a_rds,
        const std::size_t                        _a_idx
    ) const noexcept
{
    using namespace std;
    using enum utility::internal::internal_log_enum_t;
    if (_a_idx >= _a_rds.size())
    {
        return opt_for_loop_creation_data_t{};
    }
    else if (_a_idx + 1 >= _a_rds.size())
    {
        if (const opt_itt_t _l_opt_itt{get_itt_to_rep_tree_node(_a_rds[_a_idx])
            };
            _l_opt_itt.has_value())
        {
            const for_loop_stack_trie_children_t::const_iterator _l_itt{
                _l_opt_itt.value() + 1
            };
            const for_loop_creation_data_t& _l_rd{_a_rds[_a_idx]};
            if (_l_itt != _m_children[_l_rd.for_loop_index].cend())
            {
                return for_loop_creation_data_t{
                    _l_rd.for_loop_index, (*_l_itt).get()->_m_for_loop_data
                };
            }
            else
            {
                return opt_for_loop_creation_data_t{};
            }
        }
        else
        {
            return opt_for_loop_creation_data_t{};
        }
    }
    else
    {
        if (const opt_itt_t _l_opt_itt{get_itt_to_rep_tree_node(_a_rds[_a_idx])
            };
            _l_opt_itt.has_value())
        {
            return (*_l_opt_itt.value())
                .get()
                ->increment_last_index(_a_rds, _a_idx + 1);
        }
        else
        {
            return opt_for_loop_creation_data_t{};
        }
    }
}

template <typename bool Is_Root>
__constexpr_imp opt_for_loop_creation_data_t
    for_loop_stack_trie_t<Is_Root>::find_first_child_of_sequence_in_trie(
        const for_loop_creation_data_sequence_t& _a_rds,
        const std::size_t                        _a_idx
    ) const noexcept
{
    using namespace std;
    using enum utility::internal::internal_log_enum_t;
    if (_a_idx >= _a_rds.size())
    {
        if (_m_children.size() > 0 && _m_children[0].size() > 0)
        {
            return for_loop_creation_data_t{
                0, _m_children[0][0]->_m_for_loop_data
            };
        }
        else
        {
            return optional<for_loop_creation_data_t>{};
        }
    }
    else
    {
        if (const opt_itt_t _l_opt_itt{get_itt_to_rep_tree_node(_a_rds[_a_idx])
            };
            _l_opt_itt.has_value())
        {
            return (*_l_opt_itt.value())
                .get()
                ->find_first_child_of_sequence_in_trie(_a_rds, _a_idx + 1);
        }
        else
        {
            return opt_for_loop_creation_data_t{};
        }
    }
}

template <typename bool Is_Root>
__constexpr_imp bool
    for_loop_stack_trie_t<Is_Root>::is_sequence_in_trie(
        const for_loop_creation_data_sequence_t& _a_rds,
        const std::size_t                        _a_idx
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
        return get_itt_to_rep_tree_node(_a_rds[_a_idx]).has_value();
    }
    else
    {
        if (const opt_itt_t _l_opt_itt{get_itt_to_rep_tree_node(_a_rds[_a_idx])
            };
            _l_opt_itt.has_value())
        {
            return (*_l_opt_itt.value())
                .get()
                ->is_sequence_in_trie(_a_rds, _a_idx + 1);
        }
        else
        {
            return false;
        }
    }
}

template <typename bool Is_Root>
__constexpr_imp void
    for_loop_stack_trie_t<Is_Root>::add_for_loop_creation_data_sequence(
        const for_loop_creation_data_sequence_t& _a_rds,
        const std::size_t                        _a_idx
    ) noexcept
{
    using namespace std;
    using enum utility::internal::internal_log_enum_t;
    _LIBRARY_LOG(
        REPETITION_INFO, fmt::format("Adding repetition sequence {0}", _a_rds)
    );
    if (_a_idx < _a_rds.size())
    {
        const for_loop_creation_data_t& _l_rd{_a_rds[_a_idx]};
        if (_l_rd.for_loop_index >= _m_children.size())
        {
            _m_children.resize(_l_rd.for_loop_index + 1);
        }
        vector<shared_ptr<for_loop_stack_trie_t<false>>>& _l_rtn{
            _m_children[_l_rd.for_loop_index]
        };
        for (const shared_ptr<for_loop_stack_trie_t<false>>& _l_rfl : _l_rtn)
        {
            if (_l_rfl->_m_for_loop_data == _l_rd.for_loop_iteration_data)
            {
                _l_rfl->add_for_loop_creation_data_sequence(_a_rds, _a_idx + 1);
                return;
            }
        }
        shared_ptr<for_loop_stack_trie_t<false>> _l_new_child{
            shared_ptr<for_loop_stack_trie_t<false>>(
                new for_loop_stack_trie_t<false>(_l_rd.for_loop_iteration_data)
            )
        };
        vector<shared_ptr<for_loop_stack_trie_t<false>>>::iterator _l_x
            = upper_bound(
                _l_rtn.begin(),
                _l_rtn.end(),
                _l_new_child,
                [](const auto& _a_left, const auto& _a_right)
                {
                    return _a_left->_m_for_loop_data
                           < _a_right->_m_for_loop_data;
                }
            );
        vector<shared_ptr<for_loop_stack_trie_t<false>>>::iterator _l_itt
            = _l_rtn.insert(_l_x, _l_new_child);
        (*_l_itt)->add_for_loop_creation_data_sequence(_a_rds, _a_idx + 1);
    }
}

template <bool Is_Root>
__constexpr_imp opt_itt_t
    for_loop_stack_trie_t<Is_Root>::get_itt_to_rep_tree_node(
        const for_loop_creation_data_t& _a_rd
    ) const noexcept
{
    using namespace std;
    // We don't use equal range here as we would need implicit conversions from
    // rep_tree_child_t to either for_loop_iteration_data_t or some third type,
    // and from for_loop_iteration_data_t to some third type or to
    // rep_tree_child_t.

    // Easier just to write our own binary search...
    const for_loop_stack_trie_children_t& _l_vect{
        _m_children[_a_rd.for_loop_index]
    };
    for_loop_stack_trie_children_t::const_iterator _l_lower{_l_vect.begin()};
    for_loop_stack_trie_children_t::const_iterator _l_upper{_l_vect.end()};
    const gen_collection_creation_data_t&          _l_rd_fid{
        _a_rd.for_loop_iteration_data
    };
    do
    {
        if (_l_lower == _l_upper)
        {
            return opt_itt_t{};
        }
        else if (_l_lower + 1 == _l_upper)
        {
            return _l_lower;
        }
        else
        {
            for_loop_stack_trie_children_t::const_iterator _l_itt_to_check{
                _l_lower + ((_l_upper - _l_lower) / 2)
            };
            const gen_collection_creation_data_t& _l_flid{
                (*_l_itt_to_check).get()->_m_for_loop_data
            };
            if (_l_flid < _l_rd_fid)
            {
                _l_lower = _l_itt_to_check;
            }
            else if (_l_flid > _l_rd_fid)
            {
                _l_upper = _l_itt_to_check + 1;
            }
            else
            {
                return opt_itt_t{_l_itt_to_check};
            }
        }
    }
    while (_l_lower != _l_vect.cend() || _l_lower != _l_vect.cend());
    return opt_itt_t{};
}

template <bool Is_Root>
__constexpr_imp parse_for_loop_stack_trie_result_t<Is_Root>
                parse_compressed_repetition_tree_node(
                    const std::string_view _a_str
                ) noexcept
{
    using namespace utility::str;
    using namespace std;
    using enum utility::internal::internal_log_enum_t;
    if ((_a_str.size() % 2) != 0)
    {
        return unexpected(fmt::format("Compressed repetition_tree_t is in the "
                                      "form "
                                      "a string "
                                      "in "
                                      "hex."
                                      " "
                                      "As each "
                                      "hex "
                                      "digit "
                                      "requires two bytes, "
                                      "the "
                                      "total size of "
                                      "the string "
                                      "must be "
                                      "divisble "
                                      "by"
                                      " 2"
                                      ". "
                                      "This string is not"));
    }
    else
    {
        std::string _l_str{};

        // Parse the bytes.
        const char*           _l_position = _a_str.data();
        vector<unsigned char> _l_chars;
        for (size_t _l_idx{0}; _l_idx < (_a_str.size() / 2); ++_l_idx)
        {
            unsigned char value;
            auto [_l_ptr, _l_ec]
                = from_chars(_l_position, _l_position + 2, value, 16);
            _l_position += 2;
            // If no error, pass the byte to the list of chars
            if (_l_ec == errc())
            {
                _l_chars.push_back(value);
            }
            else
            {
                // Else return failure
                return unexpected{fmt::format(
                    "Could not parse byte pair {0}{1}",
                    *_l_position,
                    *(_l_position + 1)
                )};
            }
        }

        // Add all the chars to the string.
        _l_str = string(_l_chars.begin(), _l_chars.end());
        _LIBRARY_LOG(
            PARSING_SEED,
            fmt::format(
                "Depth {0}. Repetition tree after "
                "conversion "
                "from "
                "hex "
                "is "
                "i"
                "n"
                " "
                "f"
                "o"
                "r"
                "m"
                " "
                "\"{"
                "1}"
                "\"",
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
        constexpr std::size_t  _l_mode_zero_next_mode = Is_Root ? 1 : 2;
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
        for_loop_stack_trie_t<Is_Root> _l_rv;
        for (const vector<string>& _l_str : _l_strs)
        {
            vector<shared_ptr<for_loop_stack_trie_t<false>>> _l_kids;
            for (const string& _l_st : _l_str)
            {
                const expected<for_loop_stack_trie_t<false>, string> _l_op{
                    parse_compressed_repetition_tree_node<false>(_l_st)
                };
                if (_l_op.has_value())
                {
                    _l_kids.push_back(shared_ptr<for_loop_stack_trie_t<false>>(
                        new for_loop_stack_trie_t<false>(_l_op.value())
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