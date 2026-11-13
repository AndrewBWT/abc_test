#pragma once
#include "abc_test/core/ds/data_generator_memoization/data_generator_memoized_element.hpp"
#include "abc_test/core/ds/type_synonyms.hpp"

#include "abc_test/utility/pnp/hex_pnp.hpp"


#include "abc_test/utility/str/string_utils.hpp"

#include <charconv>
#include <exception>
#include <fmt/base.h>
#include <fmt/ranges.h>
#include <functional>
#include <pnp/support/internal/std/smart_pointers.hpp>
#include <pnp/support/internal/tuple_like.hpp>

_BEGIN_ABC_DS_NS
// Forward declare
class typeless_data_generator_collection_stack_trie_t;
struct tdg_collection_stack_trie_object_pnp;
template <bool From_Hex, bool Head_Node>
struct compressed_typless_data_generator_collection_stack_trie_parser_t;
template <bool Hex, bool Head_Node>
struct compressed_typless_data_generator_collection_stack_trie_pnp;

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
            const std::size_t                       _a_current_for_loop_index,
            const idgc_memoized_element_sequence_t& _a_rds
        ) const noexcept;
    friend class itdg_collection_stack_trie_t;
    template <bool Is_Hex, bool Is_Head>
    friend struct
        compressed_typless_data_generator_collection_stack_trie_parser_t;
    template <bool Hex, bool Head_Node>
    friend struct compressed_typless_data_generator_collection_stack_trie_pnp;
    friend struct tdg_collection_stack_trie_object_pnp;

    __constexpr const dgc_memoized_element_t&
        for_loop_data() const noexcept
    {
        return _m_for_loop_data;
    }

    __constexpr const for_loop_stack_trie_indexed_children_t&
        children() const noexcept
    {
        return _m_children;
    }
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
    typeless_data_generator_collection_stack_trie_t(
        const dgc_memoized_element_t& _a_flid
    ) noexcept;
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
};

using tdg_collection_stack_trie_t
    = typeless_data_generator_collection_stack_trie_t;

struct tdg_collection_stack_trie_traits
{
    static constexpr std::u8string_view
        consistent_name() noexcept
    {
        return u8"typeless_data_generator_collection_stack_trie_t";
    }

    static auto
        to_tuple(
            const typeless_data_generator_collection_stack_trie_t& object_arg
        ) noexcept
    {
        return std::tie(object_arg.for_loop_data(), object_arg.children());
    }
};

struct tdg_collection_stack_trie_object_pnp
{
    void
        run_printer(
            const typeless_data_generator_collection_stack_trie_t& object_arg,
            pnp::text_sink&                                        sink_arg
        ) const;

    pnp::basic_parser_return_value
        run_parser(
            pnp::parser_input&                               input_arg,
            typeless_data_generator_collection_stack_trie_t& object_arg,
            pnp::parser_run_key                              run_key_arg
        ) const;
};

namespace detail
{
// The pnps for the trie's fields. Must come after the definition of
// tdg_collection_stack_trie_object_pnp: children are printed and parsed with
// it, recursively, so it has to be a complete type here. The recursion is fine
// because it has no data members.
struct tdg_collection_stack_trie_object_parts
{
    using child_pnp = pnp::support::generic_smart_pointer_pnp<
        for_loop_stack_trie_child_t,
        tdg_collection_stack_trie_object_pnp>;
    using children_pnp = pnp::support::
        generic_sequence_pnp<for_loop_stack_trie_children_t, child_pnp>;
    using indexed_children_pnp = pnp::support::generic_sequence_pnp<
        for_loop_stack_trie_indexed_children_t,
        children_pnp>;
    using fields_t = std::
        tuple<dgc_memoized_element_t, for_loop_stack_trie_indexed_children_t>;
    using fields_pnp = pnp::support::generic_tuple_pnp<
        fields_t,
        std::tuple<
            data_generator_collection_memoized_element_pnp,
            indexed_children_pnp>>;
};
} // namespace detail
static_assert(pnp::printer_c<data_generator_collection_memoized_element_pnp, dgc_memoized_element_t>);
static_assert(pnp::parser_c<data_generator_collection_memoized_element_pnp, dgc_memoized_element_t>);
static_assert(::pnp::printer_c<
    detail::tdg_collection_stack_trie_object_parts::indexed_children_pnp,
    for_loop_stack_trie_indexed_children_t>);
inline void
    tdg_collection_stack_trie_object_pnp::run_printer(
        const typeless_data_generator_collection_stack_trie_t& object_arg,
        pnp::text_sink&                                        sink_arg
    ) const
{
    using parts = detail::tdg_collection_stack_trie_object_parts;
    pnp::object_pnp<
        typeless_data_generator_collection_stack_trie_t,
        tdg_collection_stack_trie_traits,
        parts::fields_pnp,
        parts::fields_pnp>{}
        .run_printer(object_arg, sink_arg);
}

inline pnp::basic_parser_return_value
    tdg_collection_stack_trie_object_pnp::run_parser(
        pnp::parser_input&                               input_arg,
        typeless_data_generator_collection_stack_trie_t& object_arg,
        [[maybe_unused]] pnp::parser_run_key             run_key_arg
    ) const
{
    using parts = detail::tdg_collection_stack_trie_object_parts;
    const std::u8string_view name{
        tdg_collection_stack_trie_traits::consistent_name()
    };
    if (not input_arg.advance_if_starts_with(name))
    {
        return input_arg.fail(u8"Expected \"" + std::u8string(name) + u8"\".");
    }
    input_arg.skip_whitespace();
    parts::fields_t fields;
    if (auto result{input_arg.run_parser(parts::fields_pnp{}, fields)};
        result.failed())
    {
        return result;
    }
    // Only stored once every field has parsed.
    auto& [for_loop_data, children]{fields};
    object_arg._m_for_loop_data = std::move(for_loop_data);
    object_arg._m_children      = std::move(children);
    return {};
}

_END_ABC_DS_NS
#if 0
_BEGIN_ABC_UTILITY_PRINTER_NS

template <>
struct default_printer_t<
    abc::ds::typeless_data_generator_collection_stack_trie_t>
    : public printer_base_t<
          abc::ds::typeless_data_generator_collection_stack_trie_t>
{
    static constexpr bool is_specialized{true};

    __no_constexpr_imp    std::u8string
                          run_printer(
                              const abc::ds::typeless_data_generator_collection_stack_trie_t&
                                  _a_object
                          ) const
    {
        using namespace std;
        using namespace _ABC_NS_UTILITY_STR;
        return object_printer_with_field_names(
            object_printer_parser_t{},
            type_id<decltype(_a_object)>(),
            {u8"for_loop_data", u8"children"},
            _a_object.for_loop_data(),
            _a_object.children()
        );
    }
};

_END_ABC_UTILITY_PRINTER_NS
#endif
_BEGIN_ABC_DS_NS

template <bool To_Hex, bool Head_Node = true>
struct compressed_typless_data_generator_collection_stack_trie_pnp
{
    void
        run_printer(
            const typeless_data_generator_collection_stack_trie_t& object_arg,
            pnp::text_sink&                                        sink_arg
        ) const;

    pnp::basic_parser_return_value
        run_parser(
            pnp::parser_input&                               input_arg,
            typeless_data_generator_collection_stack_trie_t& value_arg,
            pnp::parser_run_key                              run_key_arg
        ) const;
};

namespace detail
{
// The data held by a non-head node, in the order it is printed.
using trie_node_data_t = std::tuple<
    std::size_t,
    std::size_t,
    std::u8string,
    for_loop_stack_trie_indexed_children_t>;

// The old JUST_DATA: a child pointer prints as just its pointee, with no
// delimiters around it.
struct trie_child_pointer_defaults : pnp::support::shared_ptr_pnp_default
{
    static constexpr std::u8string_view open_delimiter  = u8"";
    static constexpr std::u8string_view close_delimiter = u8"";
};

// The pnps for the parts of the trie. Default is pnp::default_printer when
// printing, pnp::default_parser when parsing.
//
// These are only named inside the member function bodies below, where the trie
// pnp is a complete type. Naming them earlier would check pnp's constraints on
// an incomplete type. The recursion (a node's children are nodes) is fine
// because the trie pnp has no data members: nothing contains itself.
template <template <typename> typename Default>
struct trie_pnp_parts
{
    using node_pnp
        = compressed_typless_data_generator_collection_stack_trie_pnp<
            false,
            false>;
    using child_pnp = pnp::support::generic_smart_pointer_pnp<
        for_loop_stack_trie_child_t,
        node_pnp,
        trie_child_pointer_defaults>;
    using children_pnp = pnp::support::
        generic_sequence_pnp<for_loop_stack_trie_children_t, child_pnp>;
    using indexed_children_pnp = pnp::support::generic_sequence_pnp<
        for_loop_stack_trie_indexed_children_t,
        children_pnp>;
    using node_data_pnp = pnp::support::generic_tuple_pnp<
        trie_node_data_t,
        std::tuple<
            Default<std::size_t>,
            Default<std::size_t>,
            Default<std::u8string>,
            indexed_children_pnp>>;
};
} // namespace detail

template <bool Hex, bool Head_Node>
inline void
    compressed_typless_data_generator_collection_stack_trie_pnp<
        Hex,
        Head_Node>::
        run_printer(
            const typeless_data_generator_collection_stack_trie_t& object_arg,
            pnp::text_sink&                                        sink_arg
        ) const
{
    using namespace abc::utility;
    using parts = detail::trie_pnp_parts<pnp::default_printer>;
    if constexpr (Head_Node)
    {
        maybe_hex_pnp_t<Hex, typename parts::indexed_children_pnp>{}
            .run_printer(object_arg.children(), sink_arg);
    }
    else
    {
        const dgc_memoized_element_t&  fld{object_arg.for_loop_data()};
        const detail::trie_node_data_t node_data{
            fld.generation_collection_index,
            fld.flied.mode,
            fld.flied.additional_data,
            object_arg.children()
        };
        maybe_hex_pnp_t<Hex, typename parts::node_data_pnp>{}.run_printer(
            node_data, sink_arg
        );
    }
}

template <bool Hex, bool Head_Node>
inline pnp::basic_parser_return_value
    compressed_typless_data_generator_collection_stack_trie_pnp<
        Hex,
        Head_Node>::
        run_parser(
            pnp::parser_input&                               input_arg,
            typeless_data_generator_collection_stack_trie_t& value_arg,
            [[maybe_unused]] pnp::parser_run_key             run_key_arg
        ) const
{
    using namespace abc::utility;
    using parts = detail::trie_pnp_parts<pnp::default_parser>;
    if constexpr (Head_Node)
    {
        for_loop_stack_trie_indexed_children_t children;
        if (auto result{input_arg.run_parser(
                maybe_hex_pnp_t<Hex, typename parts::indexed_children_pnp>{},
                children
            )};
            result.failed())
        {
            result.add_msg(u8"while parsing the children of a trie's head node"
            );
            return result;
        }
        value_arg._m_children = std::move(children);
    }
    else
    {
        detail::trie_node_data_t node_data;
        if (auto result{input_arg.run_parser(
                maybe_hex_pnp_t<Hex, typename parts::node_data_pnp>{}, node_data
            )};
            result.failed())
        {
            result.add_msg(u8"while parsing a trie node");
            return result;
        }
        auto& [generation_collection_index, mode, additional_data, children]{
            node_data
        };
        value_arg._m_for_loop_data = dgc_memoized_element_t{
            generation_collection_index,
            dg_memoized_element_t{mode, std::move(additional_data)}
        };
        value_arg._m_children = std::move(children);
    }
    return {};
}

_END_ABC_DS_NS
_BEGIN_ABC_DS_NS

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
    typeless_data_generator_collection_stack_trie_t::
        add_for_loop_creation_data_sequence(
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
        if (_l_children.size() == 0
            || _l_children.back()->_m_for_loop_data
                   != _l_flcd.for_loop_iteration_data)
        {
            _l_children.push_back(make_unique<tdg_collection_stack_trie_t>(
                tdg_collection_stack_trie_t(_l_flcd.for_loop_iteration_data)
            ));
            _l_current_ref = ref(*_l_children.back());
        }
        else
        {
            _l_current_ref = ref(*_l_children.back());
        }
    }
}

__constexpr_imp opt_idgc_memoized_element_t
    typeless_data_generator_collection_stack_trie_t::
        find_first_child_of_sequence_in_trie(
            const std::size_t                       _a_current_for_loop_index,
            const idgc_memoized_element_sequence_t& _a_rds
        ) const noexcept
{
    if (_a_rds.size() == 0)
    {
        if (_m_children.size() == 0
            || _m_children[_a_current_for_loop_index].size() == 0)
        {
            return opt_idgc_memoized_element_t{};
        }
        else
        {
            return opt_idgc_memoized_element_t{
                idgc_memoized_element_t{
                                        .for_loop_index = _a_current_for_loop_index,
                                        .for_loop_iteration_data
                    = _m_children[_a_current_for_loop_index][0]
                          ->_m_for_loop_data
                }
            };
        }
    }
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
    typeless_data_generator_collection_stack_trie_t::
        typeless_data_generator_collection_stack_trie_t(
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
    pair<
        for_loop_stack_trie_children_t::const_iterator,
        for_loop_stack_trie_children_t::const_iterator>
                                                              _l_rv;
    std::reference_wrapper<const tdg_collection_stack_trie_t> _l_current_ref{
        *this
    };
    if (_a_flcds.size() == 0)
    {
        return opt_itt_and_end_t{};
    }
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
            // This narrows down the range. every element within the bounds of
            // the begin and end has a generation_collection_index and mode
            // which is the same as the target.
            ranges::subrange<for_loop_stack_trie_children_t::const_iterator>
                _l_equal_subrange{ranges::equal_range(
                    _l_children,
                    make_pair(
                        _l_flcd.for_loop_iteration_data
                            .generation_collection_index,
                        _l_flcd.for_loop_iteration_data.flied.mode
                    ),
                    ranges::less{},
                    [](const for_loop_stack_trie_child_t& _a_flstc)
                    {
                        return make_pair(
                            _a_flstc->_m_for_loop_data
                                .generation_collection_index,
                            _a_flstc->_m_for_loop_data.flied.mode
                        );
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
                bool _l_found{false};
                _l_rv.second = _l_children.end();
                for (for_loop_stack_trie_children_t::const_iterator _l_itt
                     = _l_equal_subrange.begin();
                     _l_itt != _l_equal_subrange.end();
                     ++_l_itt)
                {
                    const for_loop_stack_trie_child_t& _l_element = *_l_itt;
                    if (_l_element.get()->_m_for_loop_data.flied.additional_data
                        == _l_flcd.for_loop_iteration_data.flied
                               .additional_data)
                    {
                        _l_rv.first    = _l_itt;
                        _l_current_ref = ref(*_l_element);
                        _l_found       = true;
                        break;
                    }
                }
                if (not _l_found)
                {
                    return opt_itt_and_end_t{};
                }
            }
        }
    }
    return _l_rv;
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

_END_ABC_DS_NS
