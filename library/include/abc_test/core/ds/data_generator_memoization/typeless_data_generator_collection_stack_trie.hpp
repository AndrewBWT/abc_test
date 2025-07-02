#pragma once
#include "abc_test/core/ds/data_generator_memoization/data_generator_memoized_element.hpp"
#include "abc_test/core/ds/type_synonyms.hpp"
#include "abc_test/core/errors/test_library_exception.hpp"
#include "abc_test/utility/parsers/default_parser.hpp"
#include "abc_test/utility/printers/default_printer.hpp"
#include "abc_test/utility/printers/function_printer.hpp"
#include "abc_test/utility/str/string_utils.hpp"

#include <charconv>
#include <exception>
#include <fmt/base.h>
#include <fmt/ranges.h>
#include <functional>

_BEGIN_ABC_DS_NS
// Forward declare
class typeless_data_generator_collection_stack_trie_t;
template <bool From_Hex, bool Head_Node>
struct compressed_typless_data_generator_collection_stack_trie_parser_t;

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
_END_ABC_DS_NS
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
_BEGIN_ABC_DS_NS

template <bool To_Hex, bool Head_Node = true>
struct compressed_typless_data_generator_collection_stack_trie_printer_t
    : public abc::utility::printer::printer_base_t<
          abc::ds::typeless_data_generator_collection_stack_trie_t>
{
    __no_constexpr_imp std::u8string
                       run_printer(
                           const abc::ds::typeless_data_generator_collection_stack_trie_t&
                               _a_object
                       ) const
    {
        using namespace std;
        using namespace abc::ds;
        using namespace abc::utility::printer;
        using namespace abc::utility;
        const auto  _l_child_printer{default_printer<
             vector<for_loop_stack_trie_children_t>>(
            default_printer<vector<for_loop_stack_trie_child_t>>(
                default_printer<shared_ptr<
                     typeless_data_generator_collection_stack_trie_t>>(
                    mk_printer(
                        compressed_typless_data_generator_collection_stack_trie_printer_t<
                             To_Hex,
                             false>{}
                    ),
                    enum_pointer_print_parse_type_t::JUST_DATA
                )
            )
        )};
        const auto& _l_children{_a_object.children()};
        if constexpr (Head_Node == false)
        {
            auto& _l_fld{_a_object.for_loop_data()};
            auto& _l_flied{_l_fld.flied};
            using node_type_t = tuple<
                size_t,
                size_t,
                u8string,
                for_loop_stack_trie_indexed_children_t>;
            node_type_t _l_node_data{
                _l_fld.generation_collection_index,
                _l_flied.mode,
                _l_flied.additional_data,
                _l_children
            };
            auto _l_printer{default_printer_t<node_type_t>()};
            get<3>(_l_printer.get_printers_ref()) = _l_child_printer;
            if constexpr (To_Hex)
            {
                return hex_printer<node_type_t>{mk_printer(_l_printer)}
                    .run_printer(_l_node_data);
            }
            else
            {
                return _l_printer.run_printer(_l_node_data);
            }
        }
        else
        {
            if constexpr (To_Hex)
            {
                return hex_printer<vector<for_loop_stack_trie_children_t>>{
                    _l_child_printer
                }
                    .run_printer(_l_children);
            }
            else
            {
                return _l_child_printer->run_printer(_l_children);
            }
        }
    }
};

template <bool From_Hex, bool Head_Node = true>
struct compressed_typless_data_generator_collection_stack_trie_parser_t
    : public abc::utility::parser::parser_base_t<
          abc::ds::typeless_data_generator_collection_stack_trie_t>
{
    __no_constexpr_imp
        result_t<abc::ds::typeless_data_generator_collection_stack_trie_t>
        run_parser(
            abc::utility::parser::parser_input_t& _a_parse_input
        ) const

    {
        using namespace std;
        using namespace abc::ds;
        using namespace abc::utility::parser;
        using namespace abc::utility;
        auto _l_child_parser{default_parser<
            vector<for_loop_stack_trie_children_t>>(
            default_parser<vector<for_loop_stack_trie_child_t>>(
                default_parser<shared_ptr<
                    typeless_data_generator_collection_stack_trie_t>>(
                    mk_parser(
                        compressed_typless_data_generator_collection_stack_trie_parser_t<
                            false,
                            false>{}
                    ),
                    enum_pointer_print_parse_type_t::JUST_DATA
                )
            )
        )};
        if constexpr (Head_Node == false)
        {
            using tuple_type_t = tuple<
                size_t,
                size_t,
                u8string,
                for_loop_stack_trie_indexed_children_t>;
            parser_t<tuple_type_t> _l_node_parser{default_parser<tuple_type_t>(
                default_parser<size_t>(),
                default_parser<size_t>(),
                default_parser<u8string>(),
                _l_child_parser
            )};
            if constexpr (From_Hex)
            {
                _l_node_parser
                    = mk_parser(hex_parser_t<tuple_type_t>{_l_node_parser});
            }
            return _l_node_parser->run_parser(_a_parse_input)
                .transform(
                    [](const tuple_type_t& _l_node)
                    {
                        const auto& [_l_generation_collection_index, _l_mode, _l_additional_data, _l_children]{
                            _l_node
                        };
                        dgc_memoized_element_t _l_dgc{
                            _l_generation_collection_index,
                            dg_memoized_element_t{_l_mode, _l_additional_data}
                        };
                        typeless_data_generator_collection_stack_trie_t _l_rv;
                        _l_rv._m_for_loop_data = _l_dgc;
                        _l_rv._m_children      = _l_children;
                        return _l_rv;
                    }
                );
        }
        else
        {
            if constexpr (From_Hex)
            {
                _l_child_parser = mk_parser(
                    hex_parser_t<vector<for_loop_stack_trie_children_t>>(
                        _l_child_parser
                    )
                );
            }
            return _l_child_parser->run_parser(_a_parse_input)
                .transform(
                    [](const auto& _l_children)
                    {
                        typeless_data_generator_collection_stack_trie_t _l_rv;
                        _l_rv._m_children = _l_children;
                        return _l_rv;
                    }
                );
        }
    }
};

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
                                        _a_current_for_loop_index, _m_children[_a_current_for_loop_index][0]->_m_for_loop_data
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
