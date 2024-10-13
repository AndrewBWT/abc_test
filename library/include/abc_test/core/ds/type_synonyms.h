#pragma once

#include "abc_test/core/ds/gen_data_memoization/for_loop_creation_data.h"
#include "abc_test/utility/internal/macros.h"
#include "abc_test/utility/ptr_utility.h"

#include <functional>

_BEGIN_ABC_DS_NS
// Forward declarations
struct registered_test_data_t;
struct post_setup_test_data_t;

/*!
 * @brief A type synonym to the type of the test function.
 *
 * Note that it is a reference, not a functoin pointer.
 */
using test_function_t         = void (&)();
/*!
 * @brief Type synonym for a test_path_t element.
 */
using test_path_element_t     = std::string;
/*!
 * @brief Type synonym for a reference to a test_path_element_t.
 */
using test_path_element_ref_t = std::string_view;
/*!
 * @brief The type hierarchy type synonym. Conceptually it is a vector of
 * test_path_element_t's.
 */
using test_path_hierarchy_t   = std::vector<test_path_element_t>;
/*!
 * @brief Type synonym for an elemnet in a test_list_t.
 */
using test_list_element_t     = ds::registered_test_data_t;
/*!
 * @brief The type synonym for a test list. 
 * 
 * This is simply just an stl list.
 */
using test_list_t             = std::list<test_list_element_t>;
/*!
 * @brief A type synonym for a list of test lists.
 */
using test_lists_t
    = std::vector<utility::const_shared_and_raw_ptr<test_list_t>>;
/*!
 * @brief The type synonym for the unique id type.
 */
using unique_id_t = std::size_t;
/*!
 * @brief Type synonym for a list of post_setup_test_t elemnets.
 */
using post_setup_test_list_t
    = std::list<std::reference_wrapper<const post_setup_test_data_t>>;
/*!
 * @brief The iterator for the collection of reference wrappers of
 * post_setup_test_data_t elements.
 */
using post_setup_test_list_itt_t = post_setup_test_list_t::const_iterator;
_END_ABC_DS_NS