#pragma once

#include "abc_test/core/data_generator/data_generator.h"
#include "abc_test/utility/io/file/opt_file_rw.h"

_BEGIN_ABC_NS
// no opt means generate next failed. a value means it was a success.
// Value represents new mode value.
using generate_next_return_type_t = std::optional<std::size_t>;

template <
    typename T,
    typename Rep_Data,
    bool Supports_General_Data_File_IO,
    bool Supports_Rep_Data_File_IO>
class data_generator_with_file_support_t : public data_generator_t<T>
{
public:
    /*!
     * @brief Implementation of base class's has_current_element().
     *
     * Checks both files (if they are contained) to see if there are any
     * elements. If not, moves onto the elements' subclass_current_element
     * method.
     *
     * @return True if there is a current element. False otherwise.
     */
    __constexpr virtual bool
        has_current_element() const final;
    /*!
     * @brief Gets the current element.
     *
     * This function will probe the associated files, if they exist. It will
     * also fail if the mode is not correct.
     *
     * @return A cref to the current element.
     */
    __constexpr virtual const T&
        current_element() const final;
    /*!
     * @brief Implementation of base-classes generate_next() function. Gets the
     * next element in the data generator.
     * @return True if there is a next element; false otherwise.
     */
    __constexpr virtual bool
        generate_next() final;
    /*!
     * @brief Sets the data generator to the exact state as described by the
     * dg_memoized_element_t argument.
     * @param _a_dgme The object describing the state to set this object to.
     * @return
     */
    __constexpr virtual void
        set_data_generator_using_data_generator_memoized_element(
            const ds::dg_memoized_element_t& _a_dgme
        ) final;
    /*!
     * @brief Gets the additional string data for the current element generated.
     * @return A string representation of hte current data.
     */
    __constexpr std::string virtual get_additional_data() const override final;
    /*!
     * @brief Gets the current data element represneted as a
     * dg_memozied_element_t.
     * @param _a_get_data_for_repeating The mode; repetitions or not.
     * @return The ds::dg_memozied_element_t element.
     */
    __constexpr virtual ds::dg_memoized_element_t
        get_data_generator_memoized_element(const bool _a_get_data_for_repeating
        ) const noexcept override final;
protected:
    __constexpr_imp
        data_generator_with_file_support_t(
            utility::io::opt_file_rw_t<T, 0>&&        _a_opt_file_gd,
            utility::io::opt_file_rw_t<Rep_Data, 1>&& _a_opt_sd
        ) noexcept;
    template <typename R = std::vector<T>>
    __constexpr
    data_generator_with_file_support_t(
        const size_t                              _a_elements_generated,
        const utility::io::opt_file_rw_info_t<T>& _a_core_data_rw_info,
        R&&                                       _a_elements,
        const std::string_view                    _a_rep_data_file_extension,
        const utility::io::opt_file_rw_info_t<Rep_Data>& _a_rep_data_rw_info,
        const test_options_base_t*                       _a_test_options
        = &global::get_global_test_options()
    );
    __constexpr
    data_generator_with_file_support_t(
        const size_t                                     _a_elements_generated,
        const utility::io::opt_file_rw_info_t<Rep_Data>& _a_rep_data_rw_info,
        const test_options_base_t*                       _a_test_options
        = &global::get_global_test_options()
    ) noexcept;
    template <typename R = std::vector<T>>
    __constexpr
    data_generator_with_file_support_t(
        const size_t                              _a_elements_generated,
        const utility::io::opt_file_rw_info_t<T>& _a_core_data_rw_info,
        R&&                                       _a_elements,
        const test_options_base_t*                _a_test_options
        = &global::get_global_test_options()
    ) noexcept;
    __constexpr
    data_generator_with_file_support_t(
        const size_t               _a_elements_generated,
        const test_options_base_t* _a_test_options
        = &global::get_global_test_options()
    ) noexcept;
    std::size_t              _m_elements_generated = 0;
    test_runner_t*           _m_test_runner;
    static const std::size_t _STATIC_GENERAL_FILE_IO_MODE{0};
    static const std::size_t _STATIC_REP_DATA_FILE_IO_MODE{1};
    static const std::size_t _STATIC_SUBCLASS_MODE_BEGIN{2};
protected:
    /*!
     * @brief Abstract function tells the caller whether it currently has an
     * element.
     * @return True if it does; false otherwise.
     */
    __constexpr virtual bool
        subclass_has_current_element() const noexcept
        = 0;
    /*!
     * @brief Abstract function which gets the caller a cref to the current
     * element.
     * @return A cref to the current element.
     */
    __constexpr virtual const T&
        subclass_current_element() const noexcept
        = 0;
    /*!
     * @brief Abstract method which generates the next element.
     * @return Returns True if an element was able to be generated; false
     * otherwise.
     */
    __constexpr virtual bool
        subclass_generate_next()
        = 0;
    /*!
     * @brief Abstract function which sets the data using the given mode and
     * rep_data element.
     * @param _a_mode The mode to set the object to.
     * @param _a_rep_data The Rep_Data to use to set the data.
     * @return
     */
    __constexpr virtual void
        subclass_set_data_using_mode_and_repetition_data(
            const std::size_t _a_mode,
            const Rep_Data    _a_rep_data
        )
        = 0;
    /*!
     * @brief Abstract method which is designed to reset the data in this
     * object.
     */
    __constexpr virtual void
        subclass_reset_data() noexcept
        = 0;
    /*!
     * @brief Gets the repetition data for the current element.
     */
    __constexpr virtual Rep_Data
        subclass_get_repetition_data() const noexcept
        = 0;
private:
    /*!
     * @brief Returns a test_library_exception_t object which encodes an
     * incorrect mode for hwen it shoudl be above 2, but is not.
     * @return errors::test_library_exception_t object which encodes the
     * exception.
     */
    __constexpr errors::test_library_exception_t
                generate_incorrect_mode_exception() const noexcept;
    /*!
     * @brief Deals with the logic for generate next with the different
     * competing elements.
     * @return An optinoal size_t, containing the next mode.
     */
    __constexpr generate_next_return_type_t
        generate_next_inner();
    /*!
     * @brief Initialises the rep data level, or mode 1 when moving from mode 0.
     *
     * Can also initialise mode 2 if mode 1 isn't applicable.
     * @return The new mode, or empty if there is no new mode.
     */
    __constexpr generate_next_return_type_t
        initialise_rep_data_level() noexcept;
    /*!
     * @brief Initialises the lowest level this class provides; level 2.
     * @return The new mode.
     */
    __constexpr generate_next_return_type_t
        initialise_subclass_level() noexcept;
    /*!
     * @brief Sets the current data using Rep_Data.
     */
    __constexpr void
        set_element_if_required() noexcept;
private:
    /*!
     * @brief The file containing the core general data.
     */
    utility::io::opt_file_rw_t<T, 0> _m_core_data_rw_file;
    /*!
     * @brief The file containing the rep_data data.
     */
    utility::io::opt_file_rw_t<Rep_Data, 1> _m_rep_data_rw_file;
    /*!
     * @brief The reader-writer for RepData elements. Used when reading rep_data
     * elemnets described in a string.
     */
    utility::str::rw_info_t<Rep_Data> _m_rep_data_rw_info;
    /*!
     * @brief The line-reader for ilne elements. Used when reading repetition
     * data describing a line number.
     */
    utility::str::rw_info_t<std::size_t> _m_line_reader_writer;
    /*!
     * @brief Says whether the element has been memoized to a file or not.
     */
    bool _m_has_current_element_been_written_to_file = false;
};

_END_ABC_NS

_BEGIN_ABC_NS
template <
    typename T,
    typename Rep_Data,
    bool Supports_General_Data_File_IO,
    bool Supports_Rep_Data_File_IO>
__constexpr_imp bool
    data_generator_with_file_support_t<
        T,
        Rep_Data,
        Supports_General_Data_File_IO,
        Supports_Rep_Data_File_IO>::has_current_element() const
{
    if constexpr (Supports_General_Data_File_IO)
    {
        if (this->_m_mode == _STATIC_GENERAL_FILE_IO_MODE
            && _m_core_data_rw_file.has_current_element())
        {
            return true;
        }
    }
    if constexpr (Supports_Rep_Data_File_IO)
    {
        if (this->_m_mode == _STATIC_REP_DATA_FILE_IO_MODE
            && _m_rep_data_rw_file.has_current_element())
        {
            return true;
        }
    }
    if (this->_m_mode >= _STATIC_SUBCLASS_MODE_BEGIN)
    {
        return subclass_has_current_element();
    }
    else
    {
        throw generate_incorrect_mode_exception();
    }
}

template <
    typename T,
    typename Rep_Data,
    bool Supports_General_Data_File_IO,
    bool Supports_Rep_Data_File_IO>
__constexpr_imp const T&
    data_generator_with_file_support_t<
        T,
        Rep_Data,
        Supports_General_Data_File_IO,
        Supports_Rep_Data_File_IO>::current_element() const
{
    if constexpr (Supports_General_Data_File_IO)
    {
        if (this->_m_mode == _STATIC_GENERAL_FILE_IO_MODE)
        {
            return _m_core_data_rw_file.current_element();
        }
    }
    if constexpr (Supports_Rep_Data_File_IO)
    {
        if (this->_m_mode == _STATIC_REP_DATA_FILE_IO_MODE)
        {
            return subclass_current_element();
        }
    }
    if (this->_m_mode >= _STATIC_SUBCLASS_MODE_BEGIN)
    {
        return subclass_current_element();
    }
    else
    {
        throw generate_incorrect_mode_exception();
    }
}

template <
    typename T,
    typename Rep_Data,
    bool Supports_General_Data_File_IO,
    bool Supports_Rep_Data_File_IO>
__constexpr_imp bool
    data_generator_with_file_support_t<
        T,
        Rep_Data,
        Supports_General_Data_File_IO,
        Supports_Rep_Data_File_IO>::generate_next()
{
    const generate_next_return_type_t _l_result{generate_next_inner()};
    if (_l_result.has_value())
    {
        const size_t _l_mode{_l_result.value()};
        _m_has_current_element_been_written_to_file = false;
        this->_m_mode                               = (_l_mode);
        if (_l_mode == 1)
        {
            subclass_set_data_using_mode_and_repetition_data(
                this->mode(), _m_rep_data_rw_file.current_element()
            );
        }
    }
    return _l_result.has_value();
}

template <
    typename T,
    typename Rep_Data,
    bool Supports_General_Data_File_IO,
    bool Supports_Rep_Data_File_IO>
__constexpr_imp void
    data_generator_with_file_support_t<
        T,
        Rep_Data,
        Supports_General_Data_File_IO,
        Supports_Rep_Data_File_IO>::
        set_data_generator_using_data_generator_memoized_element(
            const ds::dg_memoized_element_t& _a_dgme
        )
{
    subclass_set_data_using_mode_and_repetition_data(
        _a_dgme.mode,
        _m_rep_data_rw_info.parser().run_parser_with_exception(
            _a_dgme.additional_data
        )
    );
}

template <
    typename T,
    typename Rep_Data,
    bool Supports_General_Data_File_IO,
    bool Supports_Rep_Data_File_IO>
__constexpr_imp std::string
                data_generator_with_file_support_t<
                    T,
                    Rep_Data,
                    Supports_General_Data_File_IO,
                    Supports_Rep_Data_File_IO>::get_additional_data() const
{
    using namespace errors;
    switch (this->_m_mode)
    {
    case _STATIC_GENERAL_FILE_IO_MODE:
        return _m_line_reader_writer.printer().run_printer(
            _m_core_data_rw_file.elements_read()
        );
    case _STATIC_REP_DATA_FILE_IO_MODE:
        return _m_line_reader_writer.printer().run_printer(
            _m_rep_data_rw_file.elements_read()
        );
    case _STATIC_SUBCLASS_MODE_BEGIN:
        return _m_rep_data_rw_info.printer().run_printer(
            subclass_get_repetition_data()
        );
        break;
    default:
        throw test_library_exception_t(fmt::format(
            "Invalid mode {0}. Must be between 0 and 2 (inclusive)",
            this->_m_mode
        ));
    }
}

template <
    typename T,
    typename Rep_Data,
    bool Supports_General_Data_File_IO,
    bool Supports_Rep_Data_File_IO>
__constexpr_imp ds::dg_memoized_element_t
                data_generator_with_file_support_t<
                    T,
                    Rep_Data,
                    Supports_General_Data_File_IO,
                    Supports_Rep_Data_File_IO>::
        get_data_generator_memoized_element(
            const bool _a_get_data_for_repeating
        ) const noexcept
{
    using namespace std;
    using namespace ds;
    if (_a_get_data_for_repeating)
    {
        if constexpr (Supports_General_Data_File_IO)
        {
            if (optional<dg_memoized_element_t> _l_res{
                    _m_core_data_rw_file.log_failure_and_return_rep_data(
                        _m_has_current_element_been_written_to_file,
                        dg_memoized_element_t{
                            this->_m_mode, get_additional_data()
                        }
                    )
                };
                _l_res.has_value())
            {
                return _l_res.value();
            }
        }
        if constexpr (Supports_Rep_Data_File_IO)
        {
            if (optional<dg_memoized_element_t> _l_res{
                    _m_rep_data_rw_file.log_failure_and_return_rep_data(
                        _m_has_current_element_been_written_to_file,
                        dg_memoized_element_t{this->_m_mode, to_string(0)}
                    )
                };
                _l_res.has_value())
            {
                return _l_res.value();
            }
        }
        return dg_memoized_element_t{this->mode(), get_additional_data()};
    }
    else
    {
        return dg_memoized_element_t{this->mode(), get_additional_data()};
    }
}

template <
    typename T,
    typename Rep_Data,
    bool Supports_General_Data_File_IO,
    bool Supports_Rep_Data_File_IO>
__constexpr errors::test_library_exception_t
            data_generator_with_file_support_t<
                T,
                Rep_Data,
                Supports_General_Data_File_IO,
                Supports_Rep_Data_File_IO>::generate_incorrect_mode_exception()
        const noexcept
{
    return errors::test_library_exception_t(fmt::format(
        "{0} instance in incorrect mode. Mode = {1}, but should be in mode "
        ">="
        "{2}",
        typeid(*this).name(),
        this->_m_mode,
        _STATIC_SUBCLASS_MODE_BEGIN
    ));
}

template <
    typename T,
    typename Rep_Data,
    bool Supports_General_Data_File_IO,
    bool Supports_Rep_Data_File_IO>
__constexpr_imp generate_next_return_type_t
    data_generator_with_file_support_t<
        T,
        Rep_Data,
        Supports_General_Data_File_IO,
        Supports_Rep_Data_File_IO>::generate_next_inner()
{
    // If the constexpr says it supports general file io.
    if constexpr (Supports_General_Data_File_IO)
    {
        // Check the mode and core data file has data.
        if (_m_core_data_rw_file.has_current_element()
            && this->mode == _STATIC_GENERAL_FILE_IO_MODE)
        {
            // If it can generate next, return original.
            return _m_core_data_rw_file.generate_next(this->_m_mode)
                       ? generate_next_return_type_t{_STATIC_GENERAL_FILE_IO_MODE
                         }
                       : initialise_rep_data_level();
        }
    }
    if constexpr (Supports_General_Data_File_IO)
    {
        if (_m_rep_data_rw_file.has_current_element()
            && this->_m_mode == _STATIC_REP_DATA_FILE_IO_MODE)
        {
            subclass_reset_data();
            return _m_rep_data_rw_file.generate_next(this->_m_mode)
                       ? generate_next_return_type_t{_STATIC_REP_DATA_FILE_IO_MODE
                         }
                       : initialise_subclass_level();
        }
    }
    if (this->_m_mode >= _STATIC_SUBCLASS_MODE_BEGIN)
    {
        return subclass_generate_next()
                   ? generate_next_return_type_t{_STATIC_SUBCLASS_MODE_BEGIN}
                   : generate_next_return_type_t{};
    }
    else
    {
        throw generate_incorrect_mode_exception();
    }
}

template <
    typename T,
    typename Rep_Data,
    bool Supports_General_Data_File_IO,
    bool Supports_Rep_Data_File_IO>
__constexpr_imp generate_next_return_type_t
    data_generator_with_file_support_t<
        T,
        Rep_Data,
        Supports_General_Data_File_IO,
        Supports_Rep_Data_File_IO>::initialise_rep_data_level() noexcept
{
    if constexpr (Supports_Rep_Data_File_IO)
    {
        return _m_rep_data_rw_file.has_current_element()
                   ? generate_next_return_type_t{_STATIC_REP_DATA_FILE_IO_MODE}
                   : initialise_subclass_level();
    }
    else
    {
        return initialise_subclass_level();
    }
}

template <
    typename T,
    typename Rep_Data,
    bool Supports_General_Data_File_IO,
    bool Supports_Rep_Data_File_IO>
__constexpr_imp generate_next_return_type_t
    data_generator_with_file_support_t<
        T,
        Rep_Data,
        Supports_General_Data_File_IO,
        Supports_Rep_Data_File_IO>::initialise_subclass_level() noexcept
{
    return subclass_has_current_element()
               ? generate_next_return_type_t{_STATIC_SUBCLASS_MODE_BEGIN}
               : generate_next_return_type_t{};
}

template <
    typename T,
    typename Rep_Data,
    bool Supports_General_Data_File_IO,
    bool Supports_Rep_Data_File_IO>
__constexpr_imp void
    data_generator_with_file_support_t<
        T,
        Rep_Data,
        Supports_General_Data_File_IO,
        Supports_Rep_Data_File_IO>::set_element_if_required() noexcept
{
    if (this->_m_mode == 1)
    {
        subclass_set_data_using_mode_and_repetition_data(
            this->mode(), _m_rep_data_rw_file.current_element()
        );
    }
}

template <
    typename T,
    typename Rep_Data,
    bool Supports_General_Data_File_IO,
    bool Supports_Rep_Data_File_IO>
template <typename R>
__constexpr_imp
    data_generator_with_file_support_t<
        T,
        Rep_Data,
        Supports_General_Data_File_IO,
        Supports_Rep_Data_File_IO>::
        data_generator_with_file_support_t(
            const size_t                              _a_elements_generated,
            const utility::io::opt_file_rw_info_t<T>& _a_core_data_rw_info,
            R&&                                       _a_elements,
            const std::string_view _a_rep_data_file_extension,
            const utility::io::opt_file_rw_info_t<Rep_Data>&
                                       _a_rep_data_rw_info,
            const test_options_base_t* _a_test_options
        )
    : data_generator_with_file_support_t<
          T,
          Rep_Data,
          Supports_General_Data_File_IO,
          Supports_Rep_Data_File_IO>(
          utility::io::opt_file_rw_t<T, 0>(
              _a_core_data_rw_info,
              _a_test_options->general_data_extension,
              std::forward<R>(_a_elements)
          ),
          utility::io::opt_file_rw_t<Rep_Data, 1>(
              _a_rep_data_rw_info,
              _a_rep_data_file_extension
          )
      )
{}

template <
    typename T,
    typename Rep_Data,
    bool Supports_General_Data_File_IO,
    bool Supports_Rep_Data_File_IO>
__constexpr_imp
    data_generator_with_file_support_t<
        T,
        Rep_Data,
        Supports_General_Data_File_IO,
        Supports_Rep_Data_File_IO>::
        data_generator_with_file_support_t(
            const size_t _a_elements_generated,
            const utility::io::opt_file_rw_info_t<Rep_Data>&
                                       _a_rep_data_rw_info,
            const test_options_base_t* _a_test_options
        ) noexcept
    : data_generator_with_file_support_t<
          T,
          Rep_Data,
          Supports_General_Data_File_IO,
          Supports_Rep_Data_File_IO>(
          _a_elements_generated,
          {},
          {},
          _a_rep_data_rw_info,
          _a_test_options
      )
{}

template <
    typename T,
    typename Rep_Data,
    bool Supports_General_Data_File_IO,
    bool Supports_Rep_Data_File_IO>
template <typename R>
__constexpr_imp
    data_generator_with_file_support_t<
        T,
        Rep_Data,
        Supports_General_Data_File_IO,
        Supports_Rep_Data_File_IO>::
        data_generator_with_file_support_t(
            const size_t                              _a_elements_generated,
            const utility::io::opt_file_rw_info_t<T>& _a_core_data_rw_info,
            R&&                                       _a_elements,
            const test_options_base_t*                _a_test_options
        ) noexcept
    : data_generator_with_file_support_t<
          T,
          Rep_Data,
          Supports_General_Data_File_IO,
          Supports_Rep_Data_File_IO>(
          _a_elements_generated,
          _a_core_data_rw_info,
          _a_elements,
          "",
          {},
          _a_test_options
      )
{}

template <
    typename T,
    typename Rep_Data,
    bool Supports_General_Data_File_IO,
    bool Supports_Rep_Data_File_IO>
__constexpr_imp
    data_generator_with_file_support_t<
        T,
        Rep_Data,
        Supports_General_Data_File_IO,
        Supports_Rep_Data_File_IO>::
        data_generator_with_file_support_t(
            const size_t               _a_elements_generated,
            const test_options_base_t* _a_test_options
        ) noexcept
    : data_generator_with_file_support_t<
          T,
          Rep_Data,
          Supports_General_Data_File_IO,
          Supports_Rep_Data_File_IO>(
          _a_elements_generated,
          {},
          {},
          "",
          {},
          _a_test_options
      )
{}

template <
    typename T,
    typename Rep_Data,
    bool Supports_General_Data_File_IO,
    bool Supports_Rep_Data_File_IO>
__constexpr_imp
    data_generator_with_file_support_t<
        T,
        Rep_Data,
        Supports_General_Data_File_IO,
        Supports_Rep_Data_File_IO>::
        data_generator_with_file_support_t(
            utility::io::opt_file_rw_t<T, 0>&&        _a_opt_file_gd,
            utility::io::opt_file_rw_t<Rep_Data, 1>&& _a_opt_sd
        ) noexcept
    : data_generator_t<T>(
          _a_opt_file_gd.has_current_element() ? 0
          : _a_opt_sd.has_current_element()    ? 1
                                               : 2
      )
    , _m_core_data_rw_file(std::move(_a_opt_file_gd))
    , _m_rep_data_rw_file(std::move(_a_opt_sd))
{
    set_element_if_required();
}

/*template<
    typename T,
    typename Rep_Data
>
__constexpr_imp
    std::string
    gen_data_with_repetition_type_t<T, Rep_Data>::get_repetition_data_as_string(
    ) const noexcept
{
    return
_m_rep_data_rw_info.printer().run_printer(subclass_get_repetition_data());
}*/


_END_ABC_NS