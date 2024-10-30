#pragma once

#include "abc_test/internal/data_generator/data_generator_collection.h"
#include "abc_test/internal/data_generator/data_generator_with_file_support.h"
#include "abc_test/internal/test_runner.h"

#include <initializer_list>
#include <vector>

_BEGIN_ABC_DG_NS

template <typename T>
class static_data_t
{
public:
    using generator_type = T;
    using tertiary_type  = std::size_t;

    __constexpr ~static_data_t()
    {
        delete[] _m_elements;
    }

    template <typename R>
    __constexpr
    static_data_t(
        R&& _a_elements
    )
        : _m_elements(new T[std::ranges::size(_a_elements)])
        , _m_elements_size(std::ranges::size(_a_elements))
    {
        std::ranges::copy(_a_elements, _m_elements);
    }
    static_data_t& operator=(const static_data_t& other) // III. copy assignment
    {
        if (this == &other)
        {
            return *this;
        }
        static_data_t<T> temp(other);
        std::swap(_m_rw_info, temp._m_rw_info);
        std::swap(_m_idx, temp._m_idx);
        std::swap(_m_elements, temp._m_elements);
        std::swap(_m_elements_size, temp._m_elements_size);
        return *this;
    }
    static_data_t(const static_data_t& other) // II. copy constructor
        : _m_rw_info(other._m_rw_info)
        , _m_idx(other._m_idx)
        , _m_elements_size(other._m_elements_size)
        , _m_elements(new T[other._m_elements_size])
    {
        std::ranges::copy(other._m_elements, other._m_elements + _m_elements_size, _m_elements);
    }
    __constexpr void
        set_generator_using_tertiary_data(
            const tertiary_type& _a_tertiary_data
        )
    {
        _m_idx = _a_tertiary_data;
    }

    __constexpr bool
        has_current_element() const
    {
        return _m_idx < _m_elements_size;
    }

    __constexpr bool
        generate_next()
    {
        return ++_m_idx < _m_elements_size;
    }

    __constexpr const T&
        current_element() const
    {
        return _m_elements[_m_idx];
    }

    __constexpr void
        reset()
    {
        _m_idx = 0;
    }

    __constexpr const tertiary_type&
        tertiary_data() const
    {
        return _m_idx;
    }
private:
    abc::utility::str::rw_info_t<std::size_t> _m_rw_info;
    std::size_t                               _m_idx{0};
    T*                                        _m_elements{nullptr};
    std::size_t                               _m_elements_size{0};
};

#if 0
using static_rep_data_t = std::size_t;

template <typename T>
struct static_data_t
    : public data_generator_with_file_support_t<T, static_rep_data_t,false,false>
{
public:
    /*!
     * Default constructor which uses an empty list
     */
    __constexpr
    static_data_t() noexcept;
    /*!
     * Constructor which takes a range.
     */
    template <typename R>
    requires std::ranges::sized_range<R>
             && std::same_as<std::ranges::range_value_t<R>, T>
    __constexpr
    static_data_t(R&& _a_init_list) noexcept;
    /*!
     * Destructor which destroys the internal array.
     */
    __constexpr ~static_data_t();
    /*!
     * Overridden function which gets the string form of the relevant data.
     * Specifically in this case its the index of the elemnet.
     */
    __constexpr virtual static_rep_data_t
        subclass_get_repetition_data() const noexcept;
    /*!
     * Determines whether there is a current element in the collection.
     */
    __constexpr virtual bool
        subclass_has_current_element() const noexcept;
    /*!
     * Returns a const reference to the current element.
     */
    __constexpr virtual const T&
        subclass_current_element() const noexcept;
    /*!
     * Increments the object, getting the next element in the internal
     * collection.
     */
    __constexpr virtual bool
        subclass_generate_next();
    /*!
     * Increments the internal mechanisms using a string to represent additinoal
     * data.
     */
    __constexpr virtual void
        subclass_set_data_using_mode_and_repetition_data(
            const std::size_t       _a_mode,
            const static_rep_data_t _a_rep_data
        );
    /*!
     * Determines whether failed values can be written to a file. For this
     * class, they cannot.
     */
    __constexpr virtual bool
        are_failed_values_written_to_files() const noexcept;
private:
    T*          _m_elements;
    std::size_t _m_elements_size;
};
#endif

_END_ABC_DG_NS
_BEGIN_ABC_NS
/*!
 * Constructor which should be used. It creates a singleton collection.
 */
template <typename T, typename R = std::initializer_list<T>>
requires std::same_as<std::ranges::range_value_t<R>, T>
__constexpr
    _ABC_NS_DG::data_generator_collection_t<std::ranges::range_value_t<R>, true>
    iterate_over(R&& _a_init_list);
_END_ABC_NS

_BEGIN_ABC_DG_NS
#if 0
template <typename T>
__constexpr_imp
    static_data_t<T>::static_data_t() noexcept
    : data_generator_t<T>(0, std::optional<utility::io::file_rw_info_t<T>>{})
    , _m_elements(nullptr)
    , _m_elements_size(0)
{}

template <typename T>
template <typename R>
requires std::ranges::sized_range<R>
             && std::same_as<std::ranges::range_value_t<R>, T>
__constexpr_imp
    static_data_t<T>::static_data_t(
        R&& _a_init_list
    ) noexcept
    : data_generator_with_file_support_t<T, static_rep_data_t,false,false>(0)
    //	: gen_data_base_t<T>(0, std::optional<utility::io::file_rw_info_t<T>>{},
    //"", std::vector<T>())
    , _m_elements(
          std::ranges::size(_a_init_list) == 0
              ? nullptr
              : new T[std::ranges::size(_a_init_list)]
      )
    , _m_elements_size(_a_init_list.size())
{
    for (std::size_t _l_idx{0}; auto&& _l_element : _a_init_list)
    {
        _m_elements[_l_idx++] = _l_element;
    }
}

template <typename T>
__constexpr_imp static_data_t<T>::~static_data_t()
{
    if (_m_elements_size > 0)
    {
        delete _m_elements;
    }
}

template <typename T>
__constexpr_imp static_rep_data_t
    static_data_t<T>::subclass_get_repetition_data() const noexcept
{
    using namespace std;
    return this->_m_elements_generated;
}

template <typename T>
__constexpr_imp bool
    static_data_t<T>::subclass_has_current_element() const noexcept
{
    return this->_m_elements_generated < _m_elements_size;
}

template <typename T>
__constexpr_imp const T&
    static_data_t<T>::subclass_current_element() const noexcept
{
    return _m_elements[this->_m_elements_generated];
}

template <typename T>
__constexpr_imp bool
    static_data_t<T>::subclass_generate_next()
{
    this->_m_elements_generated++;
    return subclass_has_current_element();
}

template <typename T>
__constexpr_imp void
    static_data_t<T>::subclass_set_data_using_mode_and_repetition_data(
        const std::size_t       _a_mode,
        const static_rep_data_t _a_rep_data
    )
{
    using namespace utility::str;
    using namespace errors;
    using namespace std;
    string _a_additional_data;
    if (_a_mode == 1)
    {
        this->_m_elements_generated
            = parser_t<size_t>().run_parser_with_exception(_a_additional_data);
        if (this->_m_elements_generated >= _m_elements_size)
        {
            throw test_library_exception_t(fmt::format(
                "Could not instantiate static_data_t element at index {0} "
                "as size of static_data_t array is {1}.",
                this->_m_elements_generated,
                _m_elements_size
            ));
        }
    }
    else
    {
        throw unsupported_mode_exception(typeid(*this), _a_mode, {0, 1});
    }
}

template <typename T>
__constexpr_imp bool
    static_data_t<T>::are_failed_values_written_to_files() const noexcept
{
    return false;
}
#endif
_END_ABC_DG_NS

_BEGIN_ABC_NS
template <typename T, typename R>
requires std::same_as<std::ranges::range_value_t<R>, T>
__constexpr_imp
    _ABC_NS_DG::data_generator_collection_t<std::ranges::range_value_t<R>, true>
    iterate_over(
        R&& _a_init_list
    )
{
    using namespace std;
    using namespace _ABC_NS_DG;
    return unary_collection<T>(
        make_shared<data_generator_with_file_support_t<static_data_t<T>, false>>(
            static_data_t<T>(_a_init_list)
        )
    );
}

_END_ABC_NS