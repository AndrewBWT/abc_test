#pragma once

/*!
 * Macro which adds a constexpr keyword.
 */
#define __constexpr     constexpr

/*!
 * Macro which adds a constexpr and inline keyword
 */
#define __constexpr_imp constexpr inline

/*!
 * Macro which adds no constexpr keyword.
 */
#define __no_constexpr
/*!
 * Macro which adds a inline keyword.
 */
#define __no_constexpr_imp inline

/*!
 * Macro which adds no keywords. Used for spacing and consitency.
 */
#define __no_constexpr_or_inline
/*!
 * Macro used in cppf iles for function definitions.
 */
#define __no_constexpr_or_inline_imp

/*!
 * Macro used to compare two elements in a comparison. Assumes that the elements
 * are identical and the argument is called _a_rhs.
 */
#define __cmp_test(name)     \
    if (name != _a_rhs.name) \
    {                        \
        return false;        \
    }

/*!
 * Macro used to compare two elements in a comparison, where both are named. If
 * they are unequal then false is returned.
 */
#define __cmp_test_2(left, right) \
    if (left != right)            \
    {                             \
        return false;             \
    }

// Macros which are used to facilitate entities having a unique numerical
// identifier.

#define __ABC_TEST_INTERNAL_NAMED_COUNTER_2(Name, Counter) Name##Counter
#define __ABC_TEST_INTERNAL_NAMED_COUNTER(Name, Counter) \
    __ABC_TEST_INTERNAL_NAMED_COUNTER_2(Name, Counter)
#define __ABC_TEST_NAMED_COUNTER(Name) \
    __ABC_TEST_INTERNAL_NAMED_COUNTER(Name, __COUNTER__)

/*!
 * Macros for namespace
 */

#define _MAJOR_VERSION       0
#define _MINOR_VERSION       1
#define _MINOR_MINOR_VERSION 2

#define _BEGIN_ABC_NS \
    namespace abc     \
    {
// inline namespace v0 {
#define _BEGIN_ABC_INTERNAL_NS _BEGIN_ABC_NS
#define _END_ABC_NS            }
// }
#define _END_ABC_INTERNAL_NS   _END_ABC_NS
#define _BEGIN_ABC_DS_NS   \
    _BEGIN_ABC_INTERNAL_NS \
    namespace ds           \
    {

#define _ABC_NS           abc

#define _ABC_NS_DS        _ABC_NS::ds

#define _ABC_NS_ERRORS    _ABC_NS::errors

#define _ABC_NS_UTILITY   _ABC_NS::utility

#define _ABC_NS_UTILITY_STR _ABC_NS_UTILITY::str

#define _ABC_NS_UTILITY_PRINTER _ABC_NS_UTILITY::printer

#define _ABC_NS_REPORTERS _ABC_NS::reporters

#define _ABC_NS_REPORTS   _ABC_NS::reports

#define _ABC_NS_LOGGING   _ABC_NS::logging

#define _ABC_NS_GLOBAL    _ABC_NS::global

#define _BEGIN_ABC_ERRORS_NS \
    _BEGIN_ABC_INTERNAL_NS   \
    namespace errors         \
    {
#define _BEGIN_ABC_REPORTERS_NS \
    _BEGIN_ABC_INTERNAL_NS      \
    namespace reporters         \
    {
#define _BEGIN_ABC_UTILITY_NS \
    _BEGIN_ABC_INTERNAL_NS    \
    namespace utility         \
    {
#define _BEGIN_ABC_UTILITY_INTERNAL_NS \
    _BEGIN_ABC_UTILITY_NS             \
    namespace internal        \
    {

#define _BEGIN_ABC_UTILITY_CLI_NS \
    _BEGIN_ABC_UTILITY_NS \
    namespace cli         \
    {

#define _BEGIN_ABC_UTILITY_IO_NS \
    _BEGIN_ABC_UTILITY_NS       \
    namespace io        \
    {
#define _BEGIN_ABC_UTILITY_STR_NS \
    _BEGIN_ABC_UTILITY_NS        \
    namespace str        \
    {

#define _BEGIN_ABC_UTILITY_PARSER_NS \
    _BEGIN_ABC_UTILITY_NS           \
    namespace parser        \
    {
#define _BEGIN_ABC_UTILITY_PRINTER_NS \
    _BEGIN_ABC_UTILITY_NS            \
    namespace printer        \
    {

#define _BEGIN_ABC_GLOBAL_NS \
    _BEGIN_ABC_INTERNAL_NS   \
    namespace global         \
    {

#define _BEGIN_ABC_REPORTS_NS \
    _BEGIN_ABC_INTERNAL_NS    \
    namespace reports         \
    {

#define _BEGIN_ABC_DG_NS   \
    _BEGIN_ABC_INTERNAL_NS \
    namespace data_gen     \
    {

#define _ABC_NS_DG _ABC_NS::data_gen

#define _ABC_NS_UTILITY_CLI _ABC_NS_UTILITY::cli

#define _BEGIN_ABC_MATCHER_NS \
    _BEGIN_ABC_INTERNAL_NS    \
    namespace matcher         \
    {

#define _ABC_NS_MATCHER _ABC_NS::matcher

#define _ABC_NS_CLI _ABC_NS_UTILITY::cli

#define _BEGIN_ABC_LOGGING_NS \
    _BEGIN_ABC_INTERNAL_NS    \
    namespace logging         \
    {


#define _END_ABC_DS_NS   \
    _END_ABC_INTERNAL_NS \
    }
#define _END_ABC_ERRORS_NS \
    _END_ABC_INTERNAL_NS   \
    }
#define _END_ABC_REPORTERS_NS \
    _END_ABC_INTERNAL_NS      \
    }
#define _END_ABC_UTILITY_NS \
    _END_ABC_INTERNAL_NS    \
    }
#define _END_ABC_UTILITY_INTERNAL_NS \
    _END_ABC_UTILITY_NS             \
    }
#define _END_ABC_UTILITY_IO_NS \
    _END_ABC_UTILITY_NS       \
    }
#define _END_ABC_UTILITY_STR_NS \
    _END_ABC_UTILITY_NS        \
    }
#define _END_ABC_UTILITY_PARSER_NS \
    _END_ABC_UTILITY_NS           \
    }

#define _END_ABC_UTILITY_PRINTER_NS \
    _END_ABC_UTILITY_NS            \
    }

#define _END_ABC_UTILITY_CLI_NS \
    _END_ABC_UTILITY_NS        \
    }

#define _END_ABC_GLOBAL_NS \
    _END_ABC_INTERNAL_NS   \
    }
#define _END_ABC_REPORTS_NS \
    _END_ABC_INTERNAL_NS    \
    }

#define _END_ABC_DG_NS   \
    _END_ABC_INTERNAL_NS \
    }

#define _END_ABC_LOGGING_NS \
    _END_ABC_INTERNAL_NS    \
    }
#define _END_ABC_MATCHER_NS \
    _END_ABC_INTERNAL_NS    \
    }

// Appears to be a regression regarding static_assert(false, msg). This is a
// macro
//  which performs a workaround.

namespace abc::internal
{
template <typename>
constexpr bool dependent_false = false; // workaround before CWG2518/P2593R1
} // namespace abc::internal

#define __STATIC_ASSERT_NO_TYPE(_a_msg)\
    static_assert(abc::internal::dependent_false<bool>, _a_msg);
#define __STATIC_ASSERT(_a_type, _a_msg) \
    static_assert(abc::internal::dependent_false<_a_type>, _a_msg);

#if 0
// Testing off
#define _TESTING_BUILD 0
#else
#define _TESTING_BUILD 1
#endif