#pragma once

#include "abc_test/utility/internal/macros.h"
#include <functional>

_BEGIN_ABC_DS_NS
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
using test_failure_func_t = std::function<ds::repetition_data_t()>;
_END_ABC_DS_NS