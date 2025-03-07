#pragma once
#include "abc_test/core/data_generator/data_generator.hpp"
#include "abc_test/core/data_generator/data_generator_collection_sentinel.hpp"

#include <cstdarg>
#include <memory>
#include <vector>

_BEGIN_ABC_DG_NS
// Forward declaration
template <typename T>
struct data_generator_collection_iterator_t;
struct data_generator_collection_sentinel_t;

template <typename T>
using dgc_element_t = data_generator_ptr_t<T>;
template <typename T>
using dgc_internal_t = std::vector<dgc_element_t<T>>;
template <typename T>
using dgc_internal_itt_t = dgc_internal_t<T>::iterator;

/*!
 * Describes a collection of gen_data elements. This object contains the logic
 * for allowing for loops to work properly with a collection of gen_data
 * elements.
 * @tparam T The type of each of the data_generator_collection_t elements.
 * @tparam Single_Element Whether there is a single element inside or many
 * elements.
 *
 * The Single_Element parameter is there to ensure the user can't create "trees"
 * of data_generator_collection_t elements - there is only one constructor which
 * takes a collection of data_generator_collection_t elements, which themselves
 * can only have a Single_Element.
 *
 * Doing this ensures that the code which is written looks similar, but there is
 * type-safety ensuring unintended things cannot be done.
 */
template <typename T>
struct data_generator_collection_t
{
public:
    /*!
     * @brief Type synonym for the collectinn's iterator.
     */
    using iterator         = data_generator_collection_iterator_t<T>;
    /*!
     * @brief Type synonym for the collection's const iterator.
     */
    using const_iterator   = data_generator_collection_iterator_t<T>;
    /*!
     * @brief Type synonym for the collection's sentinel.
     */
    using sentinel_t       = data_generator_collection_sentinel_t;
    /*!
     * @brief Type synonym for the collection's const sentinel.
     */
    using const_sentinel_t = sentinel_t;
    /*!
     * @brief Default constructor. Contains no gdc_element_t's.
     * @return
     */
    __constexpr
    data_generator_collection_t() noexcept
        = default;
    /*!
     * @brief Constructor taking single gdc_element_t member.
     * @param _a_element The only element in the data_generator_collection_t
     * object.
     */
    __constexpr
    data_generator_collection_t(const dgc_element_t<T>& _a_element) noexcept;
    /*!
     * @brief Constructor taking an arbitrary collection of
     * data_generator_collection_t elements. Each of these is extracted out and
     * put into this object.
     * @tparam ...Args Set of type arguments. They all must be of type
     * data_generator_collection_t<T>.
     * @param ..._a_elements The set of data_generator_collection_t<T>
     * arguments.
     */
    template <typename... Args>
    requires (std::same_as<Args, data_generator_collection_t<T>> && ...)
    __constexpr
                data_generator_collection_t(Args... _a_elements) noexcept;
    __constexpr data_generator_collection_t<T>
                operator&(
            const data_generator_collection_t<T>& _a_arg
        ) const noexcept
    {
        dgc_internal_t<T> _l_elements{ get_elements()};
        _l_elements.append_range(_a_arg.get_elements());
        return data_generator_collection_t<T>(_l_elements);
    }

    /*!
     * @brief Returns the begin iterator to the caller.
     * @return The begin iterator of this object.
     */
    __constexpr iterator
        begin();
    /*!
     * @brief Returns the end iterator to the caller.
     *
     * @return The end iterator of this object.
     */
    __constexpr sentinel_t
        end();
    /*!
     * @brief Returns the const begin iterator to the caller.
     * @return The const begin iterator of this object.
     */
    __constexpr const_iterator
        begin() const;
    /*!
     * @brief Returns the const end iterator to the caller.
     * @return The const end iterator of this object.
     */
    __constexpr const_sentinel_t
        end() const;
    /*!
     * @brief Returns the const begin iterator to the caller.
     * @return The const begin iterator of this object.
     */
    __constexpr const_iterator
        cbegin() const;
    /*!
     * @brief Returns the const end iterator to the caller.
     * @return The const end iterator of this object.
     */
    __constexpr const_sentinel_t
        cend() const;
    /*!
     * @brief Returns a cref to the internal vecttor of data_generator elements.
     * @return A cref to the object's internal vector of data_generator
     * elements.
     */
    __constexpr const dgc_internal_t<T>&
                      get_elements() const noexcept;
    __constexpr
        data_generator_collection_t(const dgc_internal_t<T>& _a_elements) noexcept
        : _m_elements(_a_elements)
    {

    }
private:
    dgc_internal_t<T> _m_elements;
    __constexpr iterator
        gen_iterator_begin();
};

/*!
 * @brief Factory function which creates sets of data generators.
 * @tparam ...Args The variadic arguments. These must all be
 * data_generator_collection_t elements.
 * @tparam T The type of the data collection elements.
 * @param ..._a_elements The elemsnts to be added to the return type.
 * @return A data_generation_collection_t element.
 */
template <typename T, typename... Args>
requires (std::same_as<Args, data_generator_collection_t<T>> && ...)
__constexpr data_generator_collection_t<T>
            data_generator_collection(Args... _a_elements) noexcept;
/*!
 * @brief Factory function which creates a data_generator_collection_t element
 * with a single internal element.
 * @tparam T The type of the data collection elements.
 * @param _a_pointer A pointer to the element to be created.
 * @return A data_generator_collection_t with a single element.
 *
 * This function is designed to be called from instances of data_generator_t,
 * rather than by the user. Using this allows sets of data_generators to be
 * appended to one-another.
 */
template <typename T>
__constexpr data_generator_collection_t<T>
            unary_collection(const dgc_element_t<T>& _a_pointer) noexcept;

namespace
{
/*!
 * @brief The top level processing function. Takes a list of
 * data_generator_collection_t elements and turns that into a vector of
 * data_generator_ptrs.
 * @tparam T The internal type of each of the data generator_collection_t
 * elements.
 * @tparam ...Args The data_generator_collection_t types themsevles. Must be
 * variadic, but is checked by concept.
 * @param ..._a_elements The actual data data_generator_collection_t variables.
 * @return A gdc_internal_t<T> object containing all the internals inside the
 * data_generator_collection_t elements.
 */
template <typename T, typename... Args>
requires (std::same_as<Args, data_generator_collection_t<T>> && ...)
__constexpr dgc_internal_t<T>
            process_all_data_generator_elements(Args... _a_elements) noexcept;
/*!
 * @brief The recursive data_generator_collection_T procesor.
 * @tparam T the type of the data_generator_collection_T elements.
 * @tparam Args The variaidc arguments types.
 */
template <typename T, typename... Args>
requires (std::same_as<Args, data_generator_collection_t<T>> && ...)
__constexpr void
    process_all_data_generator_elements_with_result(
        std::vector<data_generator_ptr_t<T>>&       _l_vect,
        const data_generator_collection_t<T>& _a_gdc,
        Args... _a_elements
    ) noexcept;
/*!
 * @brief The base case.
 */
template <typename T>
__constexpr void
    process_all_data_generator_elements_with_result(
        std::vector<data_generator_ptr_t<T>>&       _l_vect,
        const data_generator_collection_t<T>& _a_gdc
    ) noexcept;
} // namespace

_END_ABC_DG_NS

_BEGIN_ABC_DG_NS
template <typename T>
template <typename... Args>
requires (std::same_as<Args, data_generator_collection_t<T>> && ...)
__constexpr_imp
    data_generator_collection_t<T>::data_generator_collection_t(
        Args... _a_elements
    ) noexcept
    : _m_elements(process_all_data_generator_elements<T>(_a_elements...))
{}

template <typename T>
__constexpr_imp data_generator_collection_t<T>::iterator
                data_generator_collection_t<T>::begin()
{
    return gen_iterator_begin();
}

template <typename T>
__constexpr_imp data_generator_collection_t<T>::sentinel_t
                data_generator_collection_t<T>::end()
{
    return sentinel_t();
}

template <typename T>
__constexpr_imp data_generator_collection_t<T>::const_iterator
                data_generator_collection_t<T>::begin() const
{
    return gen_iterator_begin();
}

template <typename T>
__constexpr_imp data_generator_collection_t<T>::const_sentinel_t
                data_generator_collection_t<T>::end() const
{
    return const_sentinel_t();
}

template <typename T>
__constexpr_imp data_generator_collection_t<T>::const_iterator
                data_generator_collection_t<T>::cbegin() const
{
    return gen_iterator_begin();
}

template <typename T>
__constexpr_imp data_generator_collection_t<T>::const_sentinel_t
                data_generator_collection_t<T>::cend() const
{
    return const_sentinel_t();
}

template <typename T>
__constexpr_imp const dgc_internal_t<T>&
    data_generator_collection_t<T>::get_elements(
    ) const noexcept
{
    return _m_elements;
}

template <typename T>
__constexpr_imp
    data_generator_collection_t<T>::data_generator_collection_t(
        const dgc_element_t<T>& _a_element
    ) noexcept
    : _m_elements(
          _a_element.get()->has_current_element()
              ? dgc_internal_t<T>(1, _a_element)
              : dgc_internal_t<T>()
      )
{}

template <typename T>
__constexpr_imp data_generator_collection_t<T>::iterator
    data_generator_collection_t<T>::gen_iterator_begin()
{
    return iterator(
        _m_elements.begin(),
        std::distance(_m_elements.begin(), _m_elements.end()),
        global::get_this_threads_test_runner_ptr()
    );
}

template <typename T, typename... Args>
requires (std::same_as<Args, data_generator_collection_t<T>> && ...)
__constexpr data_generator_collection_t<T>
            data_generator_collection(
                Args... _a_elements
            ) noexcept
{
    return data_generator_collection_t<T>(_a_elements...);
}

template <typename T>
__constexpr_imp data_generator_collection_t<T>
                unary_collection(
                    const dgc_element_t<T>& _a_pointer
                ) noexcept
{
    return data_generator_collection_t<T>(_a_pointer);
}

namespace
{

template <typename T, typename... Args>
requires (std::same_as<Args, data_generator_collection_t<T>> && ...)
__constexpr_imp std::vector<data_generator_ptr_t<T>>
                process_all_data_generator_elements(
                    Args... _a_elements
                ) noexcept
{
    using namespace std;
    std::vector<data_generator_ptr_t<T>> _l_rv{};
    process_all_data_generator_elements_with_result<T>(_l_rv, _a_elements...);
    return _l_rv;
}

template <typename T, typename... Args>
requires (std::same_as<Args, data_generator_collection_t<T>> && ...)
__constexpr_imp void
    process_all_data_generator_elements_with_result(
        std::vector<data_generator_ptr_t<T>>&       _l_vect,
        const data_generator_collection_t<T>& _a_gdc,
        Args... _a_elements
    ) noexcept
{
    process_all_data_generator_elements_with_result(_l_vect, _a_gdc);
    process_all_data_generator_elements_with_result(_l_vect, _a_elements...);
}

template <typename T>
__constexpr_imp void
    process_all_data_generator_elements_with_result(
        std::vector<data_generator_ptr_t<T>>&       _l_vect,
        const data_generator_collection_t<T>& _a_gdc
    ) noexcept
{
    for (const dgc_element_t<T>& _l_element : _a_gdc.get_elements())
    {
        if (_l_element.get()->has_current_element())
        {
            _l_vect.push_back(_l_element);
        }
    }
}
} // namespace

_END_ABC_DG_NS