#pragma once

/*!
* Macro which adds a constexpr keyword.
*/
#define __constexpr

/*!
* Macro which adds a constexpr and inline keyword
*/
#define __constexpr_imp inline

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
* Macro used to compare two elements in a comparison. Assumes that the elements are identical
* and the argument is called _a_rhs.
*/
#define __cmp_test(name) \
if (name != _a_rhs.name)\
{\
return false;\
}

/*!
* Macro used to compare two elements in a comparison, where both are named. If they are unequal then false is 
* returned. 
*/
#define __cmp_test_2(left,right)\
if (left != right)\
{\
return false;\
}

//Macros which are used to facilitate entities having a unique numerical identifier.

#define INTERNAL_NAMED_COUNTER_2(Name, Counter) Name##Counter
#define INTERNAL_NAMED_COUNTER(Name, Counter) INTERNAL_NAMED_COUNTER_2(Name, Counter)
#define NAMED_COUNTER(Name) INTERNAL_NAMED_COUNTER(Name, __COUNTER__ )

/*!
* Macros for namespace
*/

#define _MAJOR_VERSION 0
#define _MINOR_VERSION 1
#define _MINOR_MINOR_VERSION 2

#define _BEGIN_ABC_NS \
namespace abc {
//    inline namespace v0 {
#define _END_ABC_NS }
//    }
#define _BEGIN_ABC_DS_NS \
_BEGIN_ABC_NS \
namespace ds {
#define _BEGIN_ABC_ERRORS_NS \
_BEGIN_ABC_NS \
namespace errors {
#define _BEGIN_ABC_REPORTERS_NS \
_BEGIN_ABC_NS \
namespace reporters {
#define _BEGIN_ABC_UTILITY_NS \
_BEGIN_ABC_NS \
namespace utility {
#define _BEGIN_ABC_UTILITY_INTERNAL_NS \
_BEGIN_ABC_NS \
namespace utility::internal {
#define _BEGIN_ABC_UTILITY_IO_NS \
_BEGIN_ABC_NS \
namespace utility::io {
#define _BEGIN_ABC_UTILITY_STR_NS \
_BEGIN_ABC_NS \
namespace utility::str {
#define _BEGIN_ABC_GLOBAL_NS \
_BEGIN_ABC_NS \
namespace global {

#define _BEGIN_ABC_REPORTS_NS \
_BEGIN_ABC_NS \
namespace reports {


#define _END_ABC_DS_NS \
_END_ABC_NS \
}
#define _END_ABC_ERRORS_NS \
_END_ABC_NS \
}
#define _END_ABC_REPORTERS_NS \
_END_ABC_NS \
}
#define _END_ABC_UTILITY_NS \
_END_ABC_NS \
}
#define _END_ABC_UTILITY_INTERNAL_NS \
_END_ABC_NS \
}
#define _END_ABC_UTILITY_IO_NS \
_END_ABC_NS \
}
#define _END_ABC_UTILITY_STR_NS \
_END_ABC_NS \
}
#define _END_ABC_GLOBAL_NS \
_END_ABC_NS \
}
#define _END_ABC_REPORTS_NS \
_END_ABC_NS \
}