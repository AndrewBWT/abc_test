#pragma once

#include "abc_test/utility/internal/macros.hpp"

_BEGIN_ABC_REPORTS_NS

/*!
 * @brief Class to store assertions.
 *
 * This class exists solely to allow us to have vectors of derived instances of
 * this class.
 *
 * The virtual destructor allows us to do this.
 */
class assertion_base_t
{
public:
    /*!
     * @brief Destructor. Defined as virtual so that we can have collections of
     * derived subclass instances.
     */
    __constexpr virtual ~assertion_base_t() noexcept;
};
/*!
 * @brief Type synonym for a pointer to a assertion_base_t type.
 */
using assertion_base_ptr_t = std::unique_ptr<const assertion_base_t>;
/*!
 * @brief Type synonym for a collection of assertion_base_ptr_t elements.
 */
using assertion_base_collection_t = std::vector<assertion_base_ptr_t>;
_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS
__constexpr_imp assertion_base_t::~assertion_base_t() noexcept
{}

_END_ABC_REPORTS_NS