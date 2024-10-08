#pragma once

#include "abc_test/utility/internal/macros.h"
#include <functional>
#include "abc_test/core/ds/repetitions/repetition_data.h"
#include "abc_test/utility/ptr_utility.h"

_BEGIN_ABC_DS_NS
//Forward declarations
struct registered_test_data_t;
struct post_setup_test_data_t;

//! The type of test functions; simply a reference to a function with no arguments and
//! which returns nothing.
using test_function_t = void(&)();
//! Type synonym for a test_path_hierarchy_t element.
using test_path_element_t = std::string;
//! A reference to a test_path_hierarchy_element_t. 
using test_path_element_ref_t = std::string_view;
//! Synonym for a vector of strings, used to represent the hierarchy a test path represents.
using test_path_hierarchy_t = std::vector<test_path_element_t>;
//! Type synonym for a failure function, called when a function fails and book-keeping
//! needs to be done.
using test_failure_func_t = std::function<repetition_data_t()>;

/*!
* A type synonym for a test_lit_t element.
*/
using test_list_element_t = ds::registered_test_data_t;
/*!
* Type synonym for a collection of test elements. This is the structure used
* to collect registered_test_t elements.
*/
using test_list_t = std::list<test_list_element_t>;
/*!
 * @brief Type synonym for a collection of test_list_t elements.
 */
using test_lists_t = std::vector<utility::const_shared_and_raw_ptr<test_list_t>>;
/*!
 * @brief Type synonym for the unique id type.
 */
using unique_id_t = std::size_t;
/*!
 * @brief Type synonym for the collection of reference wrappers of post_setup_test_data_t elements.
 * 
 * These are used when working with collections of tests, rather than creating
 * a new collection each time one is required.
 */
using post_setup_test_list_t = std::list<std::reference_wrapper<const post_setup_test_data_t>>;
/*!
 * @brief The iterator for the collection of reference wrappers of post_setup_test_data_t elements.
 */
using post_setup_test_list_itt_t = post_setup_test_list_t::const_iterator;
_END_ABC_DS_NS