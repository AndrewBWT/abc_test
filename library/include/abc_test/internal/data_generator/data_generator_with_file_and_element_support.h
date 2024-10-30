#pragma once

#include "abc_test/internal/data_generator/data_generator_with_file_support.h"
#include "abc_test/internal/utility/io/file/opt_file_rw.h"

_BEGIN_ABC_DG_NS
#if 0
/*!
 * @brief Abstract data generator with support for files and stored elements.
 * @tparam T The type of data.
 * @tparam Rep_Data The rep data.
 * @tparam Supports_General_Data_File_IO Whether it supports file IO.
 * @tparam Supports_Rep_Data_File_IO Whether it supports Rep_Data IO.
 */
template <
    typename T,
    typename Rep_Data,
    bool Supports_General_Data_File_IO,
    bool Supports_Rep_Data_File_IO>
class data_generator_with_file_support_and_element_storage_t
    : public data_generator_with_file_support_t<
          T,
          Rep_Data,
          Supports_General_Data_File_IO,
          Supports_Rep_Data_File_IO>
{
public:
    __constexpr
    data_generator_with_file_support_and_element_storage_t()
        = default;
    /*!
     * @brief Constructor
     * @tparam R The range of elements.
     * @param _a_elements_generated
     * @param _a_core_data_rw_info
     * @param _a_elements
     * @param _a_rep_data_file_extension
     * @param _a_rep_data_rw_info
     * @param _a_element
     * @param _a_test_options
     * @return
     */
    template <typename R = std::vector<T>>
    __constexpr
    data_generator_with_file_support_and_element_storage_t(
        const size_t                              _a_elements_generated,
        const utility::io::opt_file_rw_info_t<T>& _a_core_data_rw_info,
        R&&                                       _a_elements,
        const std::string_view                    _a_rep_data_file_extension,
        const utility::io::opt_file_rw_info_t<Rep_Data>& _a_rep_data_rw_info,
        const T&                                         _a_element,
        const test_options_base_t*                       _a_test_options
        = &_ABC_NS_GLOBAL::get_inner_global_test_options()
    ) noexcept;
    __constexpr virtual const T&
        subclass_current_element() const noexcept override final;
protected:
    T _m_element = T{};
};

_END_ABC_DG_NS

_BEGIN_ABC_DG_NS

template <
    typename T,
    typename Rep_Data,
    bool Supports_General_Data_File_IO,
    bool Supports_Rep_Data_File_IO>
template <typename R>
__constexpr
data_generator_with_file_support_and_element_storage_t<
    T,
    Rep_Data,
    Supports_General_Data_File_IO,
    Supports_Rep_Data_File_IO>::
    data_generator_with_file_support_and_element_storage_t(
        const size_t                              _a_elements_generated,
        const utility::io::opt_file_rw_info_t<T>& _a_core_data_rw_info,
        R&&                                       _a_elements,
        const std::string_view                    _a_rep_data_file_extension,
        const utility::io::opt_file_rw_info_t<Rep_Data>& _a_rep_data_rw_info,
        const T&                                         _a_element,
        const test_options_base_t*                       _a_test_options
    ) noexcept
    : data_generator_with_file_support_t<
          T,
          Rep_Data,
          Supports_General_Data_File_IO,
          Supports_Rep_Data_File_IO>(
          _a_elements_generated,
          _a_core_data_rw_info,
          std::forward<R>(_a_elements),
          _a_rep_data_file_extension,
          _a_rep_data_rw_info,
          _a_test_options
      )
    , _m_element(_a_element)
{}

template <
    typename T,
    typename Rep_Data,
    bool Supports_General_Data_File_IO,
    bool Supports_Rep_Data_File_IO>
__constexpr_imp const T&
    data_generator_with_file_support_and_element_storage_t<
        T,
        Rep_Data,
        Supports_General_Data_File_IO,
        Supports_Rep_Data_File_IO>::subclass_current_element() const noexcept
{
    return _m_element;
}
#endif
_END_ABC_DG_NS