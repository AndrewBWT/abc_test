#pragma once


#include "abc_test/core/ds/test_collections/test_tree_iterator.hpp"
#include "abc_test/core/ds/type_synonyms.hpp"

_BEGIN_ABC_DS_NS

/*!
 * @brief Structure containing a collection of post_setup_test_data_t elements.
 *
 * This class organises the post-processing of registered_test_data_t elements,
 * organising them for running, and checking them for logical inconsistencies.
 */
struct test_collection_t
{
public:
    /*!
     * @brief Constructor for test_collection_t.
     * @param _a_test_options The test options used in the object.
     * @param _a_error_reporter_controller The error reporter, used to report
     * any errors to.
     */
    __no_constexpr
        test_collection_t(
            const test_options_base_t& _a_test_options,
            const reporters::error_reporter_controller_t&
                _a_error_reporter_controller
        ) noexcept;
    /*!
     * @brief Adds tests to the object.
     *
     * It takes a list of lists of registered_test_data_t objects. This allows
     * the caller to add several test lists at once.
     *
     * @param _a_test_lists The list of lists of tests which are to be added to
     * the object.
     */
    __no_constexpr void
        add_tests(const test_lists_t& _a_test_lists) noexcept;
    /*!
     * @brief Builds a post_setup_test_list_t for the caller.
     *
     * This function returns a single list of post_setup_test_data_t elements,
     * in the order the system should run them in. It does not include tests
     * which should not be ran, and will have ensured all which do are logically
     * sound (at least as far as it can tell).
     *
     * @return A list of post_setup_test_data_t elements.
     */
    __no_constexpr post_setup_test_list_t
        make_finalied_post_setup_test_list_in_run_order() const noexcept;
private:
    std::list<post_setup_test_data_t>      _m_post_setup_tests;
    test_tree_t                            _m_test_tree;
    reporters::error_reporter_controller_t _m_error_reporter_controller;
    unique_id_t                            _m_test_discovery_id;
    const test_options_base_t&             _m_options;
};

__constexpr std::vector<ds::test_path_hierarchy_t>
            process_test_path_hierarchies(
                const std::vector<std::u8string>& _a_strs,
                const std::u8string               _a_delimiter
            ) noexcept
{
    using namespace std;
    vector<test_path_hierarchy_t> _l_rv;
    for (const std::u8string_view _l_str : _a_strs)
    {
        _l_rv.emplace_back(
            utility::str::split_string<char8_t>(_l_str, _a_delimiter)
        );
    }
    return _l_rv;
}

_END_ABC_DS_NS

_BEGIN_ABC_DS_NS
__no_constexpr_imp
    test_collection_t::test_collection_t(
        const test_options_base_t& _a_test_options,
        const reporters::error_reporter_controller_t&
            _a_error_reporter_controller
    ) noexcept
    : _m_test_tree(test_tree_t())
    , _m_test_discovery_id(unique_id_t{1})
    , _m_options(_a_test_options)
{}

__no_constexpr_imp void
    test_collection_t::add_tests(
        const test_lists_t& _a_test_list_collection
    ) noexcept
{
    using namespace std;
    using namespace ds;
    using namespace utility;
    using namespace errors;
    using namespace _ABC_NS_UTILITY_STR;
    const map<key_t, tdg_collection_stack_trie_t>& _l_map{
        _m_options.map_of_unique_ids_and_for_loop_stack_tries.map()
    };
    std::vector<test_path_hierarchy_t> _l_test_paths_to_use
        = process_test_path_hierarchies(
            _m_options.test_paths_to_run, _m_options.path_delimiter
        );
    for (std::reference_wrapper<const test_list_t> _l_test_list_element :
         _a_test_list_collection)
    {
        for (const test_list_element_t& _l_test_element :
             _l_test_list_element.get())
        {
            const u8string _l_id{
                _l_test_element._m_user_data.make_uid(_m_options.path_delimiter)
            };
            const tdg_collection_stack_trie_t* _l_reps{
                _l_map.contains(_l_id) ? &_l_map.at(_l_id) : nullptr
            };
            // If size zero or force running, set to true.
            const bool _l_test_ran_override{
                _m_options.map_of_unique_ids_and_for_loop_stack_tries.size()
                    == 0
                || global::get_global_test_options().force_run_all_tests
            };
            const test_path_hierarchy_t _l_test_path_hierarchy{
                abc::utility::str::split_string<char8_t>(
                    cast_string_to_u8string(_l_test_element._m_user_data.path),
                    _m_options.path_delimiter
                )
            };
            const bool _l_test_in_path_set{check_if_element_in_path_set(
                _l_test_path_hierarchy, _l_test_paths_to_use
            )};
            _m_post_setup_tests.push_back(post_setup_test_data_t(
                _l_test_element,
                _l_test_path_hierarchy,
                _m_test_discovery_id,
                _l_test_in_path_set
                    && (_l_test_ran_override || (_l_reps != nullptr)),
                _l_reps,
                _m_options.threads,
                _l_id
            ));
            const opt_setup_error_t _l_res{_m_test_tree.add_test(
                std::cref(_m_post_setup_tests.back()), _m_options
            )};
            if (_l_res.has_value())
            {
                _m_post_setup_tests.pop_back();
                _m_error_reporter_controller.report_error(_l_res.value());
            }
            _m_test_discovery_id++;
        }
    }
}

/*__no_constexpr_imp
test_tree_iterator_t
test_collection_t::begin(
) const noexcept
{
    return std::begin(_m_test_tree);
}
__no_constexpr_imp
    test_tree_iterator_t
    test_collection_t::end(
    ) const noexcept
{
    return std::end(_m_test_tree);
}*/
__no_constexpr_imp post_setup_test_list_t
    test_collection_t::make_finalied_post_setup_test_list_in_run_order(
    ) const noexcept
{
    using namespace std;
    post_setup_test_list_t _l_rv;
    for (const reference_wrapper<const post_setup_test_data_t>& _l_element :
         _m_test_tree)
    {
        if (_l_element.get().is_test_to_be_ran())
        {
            _l_rv.push_back(_l_element);
        }
    }
    return _l_rv;
}

_END_ABC_DS_NS