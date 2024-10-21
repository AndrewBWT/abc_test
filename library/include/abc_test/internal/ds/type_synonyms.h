#pragma once

#include "abc_test/internal/ds/data_generator_memoization/indexed_data_generator_collection_memoized_element.h"
#include "abc_test/internal/utility/internal/macros.h"
#include "abc_test/internal/utility/ptr_utility.h"

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
    = std::vector<std::reference_wrapper<const test_list_t>>;
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
/*!
 * @brief Type synonym for a set of log info.
 */
using log_infos_t = std::vector<std::string>;

using test_path_delimiter = std::string;
using test_path_delimiter_ref = std::string_view;
using path_t = std::filesystem::path;
_END_ABC_DS_NS
_BEGIN_ABC_NS
struct test_options_base_t;
template<typename T>
struct validated_test_options_t;
_END_ABC_NS
_BEGIN_ABC_LOGGING_NS
struct log_msg_t;
using log_msg_ref_t = std::reference_wrapper<const log_msg_t>;
using log_msg_ptrs_t = std::list<log_msg_ref_t>;
using log_msg_itt_t = log_msg_ptrs_t::iterator;
_END_ABC_LOGGING_NS