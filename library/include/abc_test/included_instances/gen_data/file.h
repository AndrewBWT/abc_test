#pragma once

#include "abc_test/core.h"
#include "abc_test/internal/data_generator/data_generator_collection.h"
#include "abc_test/internal/data_generator/data_generator_with_file_and_element_support.h"
#include "abc_test/internal/data_generator/data_generator_with_file_io.h"
#include "abc_test/internal/global.h"
#include "abc_test/internal/test_runner.h"
#include "abc_test/internal/utility/io/file/file_reader.h"
#include "abc_test/internal/utility/io/file/file_writer.h"

#include <filesystem>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <scn/ranges.h>
#include <scn/scan.h>
#include <vector>
_BEGIN_ABC_NS

_END_ABC_NS
_BEGIN_ABC_DG_NS

template <typename T>
class file_data_t
{
public:
    using generator_type = T;
    using tertiary_type = std::size_t;
    __constexpr const tertiary_type&
        tertiary_data() const
    {
        return _m_elements_read;
    }
    __constexpr void
        set_generator_using_tertiary_data(
            const tertiary_type& _a_tertiary_data
        )
    {
        if (_a_tertiary_data > _m_elements_read)
        {
            reset_file();
        }
        if (_a_tertiary_data < _m_elements_read)
        {
            skip_lines(_a_tertiary_data - _m_elements_read);
        }
        if (_a_tertiary_data == _m_elements_read)
        {
            _m_elements_read = _a_tertiary_data;
            _m_element = _m_element_rw_info.parser().run_parser_with_exception(
                _m_line_reader.current_line()
            );
        }
    }
    template <typename R>
    __constexpr
        file_data_t(
            const general_data_t& _a_gdf,
            R&& _a_elements
        )
        : _m_path(_a_gdf._m_path)
    {
        using namespace abc::utility::io;
        using namespace std::filesystem;
        if (not exists(_m_path))
        {
            file_line_writer_t _l_flw(_m_path);
            _l_flw.write_comment(typeid(T).name());
            for (auto&& _l_element : _a_elements)
            {
                _l_flw.write_line(
                    _m_element_rw_info.printer().run_printer(_l_element)
                );
            }
        }
        reset_file();
    }
    template <typename R>
    __constexpr
    file_data_t(
        const general_data_with_rw_info_t<T>& _a_gdf,
        R&&        _a_elements
    )
        : _m_path(_a_gdf._m_path)
    {
        using namespace abc::utility::io;
        using namespace std::filesystem;
        if (not exists(_m_path))
        {
            file_line_writer_t _l_flw(_m_path);
            _l_flw.write_comment(typeid(T).name());
            for (auto&& _l_element : _a_elements)
            {
                _l_flw.write_line(
                    _m_element_rw_info.printer().run_printer(_l_element)
                );
            }
        }
        reset_file();
    }
    __constexpr void
        reset()
    {
        _m_elements_read = 0;
        reset_file();
    }
    __constexpr void
        reset_file()
    {
        using namespace abc::utility::io;
        _m_line_reader = file_line_reader_t(_m_path);
        if (_m_line_reader.has_current_line())
        {
            ++_m_elements_read;
            _m_element = _m_element_rw_info.parser().run_parser_with_exception(
                _m_line_reader.current_line()
            );
        }
    }

    __constexpr void
        skip_lines(
            const std::size_t _a_idx
        )
    {
        while (_m_elements_read > _a_idx)
        {
            _m_line_reader.get_next_line();
            ++_m_elements_read;
        }
    }

    __constexpr bool
        has_current_element() const
    {
        return _m_line_reader.has_current_line();
    }

    __constexpr bool
        generate_next()
    {
        if (_m_line_reader.get_next_line())
        {
            ++_m_elements_read;
            _m_element = _m_element_rw_info.parser().run_parser_with_exception(
                _m_line_reader.current_line()
            );
            return true;
        }
        else
        {
            return false;
        }
    }

    __constexpr const T&
        current_element() const
    {
        return _m_element;
    }
private:
    std::filesystem::path                     _m_path;
    std::size_t                               _m_elements_read{0};
    abc::utility::io::file_line_reader_t      _m_line_reader;
    abc::utility::str::rw_info_t<T>           _m_element_rw_info;
    abc::utility::str::rw_info_t<std::size_t> _m_index_rw_info;
    T                                         _m_element;
};

_END_ABC_DG_NS

_BEGIN_ABC_NS
template <typename T, typename R = std::initializer_list<T>>
    requires std::same_as<std::ranges::range_value_t<R>, T>
__constexpr_imp
_ABC_NS_DG::data_generator_collection_t<std::ranges::range_value_t<R>, true>
read_data_from_file(
    const general_data_t& _a_general_data_file,
    R&& _a_init_list = R{}
)
{
    using namespace std;
    using namespace _ABC_NS_DG;
    return unary_collection<T>(make_shared<data_generator_single_t<file_data_t<T>, false>>(
        file_data_t<T>(_a_general_data_file, _a_init_list)
    ));
}
template <typename T, typename R = std::initializer_list<T>>
requires std::same_as<std::ranges::range_value_t<R>, T>
__constexpr_imp
    _ABC_NS_DG::data_generator_collection_t<std::ranges::range_value_t<R>, true>
    read_data_from_file(
        const general_data_with_rw_info_t<T>& _a_general_data_file,
        R&&        _a_init_list = R{}
    )
{
    using namespace std;
    using namespace _ABC_NS_DG;
    return unary_collection<T>(make_shared<data_generator_single_t<file_data_t<T>, false>>(
        file_data_t<T>(_a_general_data_file, _a_init_list)
    ));
}

_END_ABC_NS

#if 0
_BEGIN_ABC_DG_NS
using file_type_rep_data_t = bool;

/*!
 * Object describing data taken from a file
 */
template <typename T>
struct file_data_t
    : public data_generator_with_file_support_and_element_storage_t<
          T,
          file_type_rep_data_t,true,false>
{
public:
    /*!
     * Default file data object. No default object
     */
    __constexpr
    file_data_t() noexcept
        = delete;
    /*!
     * Construct with initial values. Takes file name without extension and a
     * set of initial values to write to the file if it doesn't previously
     * exist.
     */
    template <typename R = std::vector<T>>
    __constexpr
    file_data_t(
        const utility::io::file_rw_info_t<T>& _a_templated_file_rw,
        R&&                                   _a_initial_values = {}
    );

    /*!
     * Determines whether the object has a current element. As this is a file
     * streaming object, there are no additional modes assocated with it.
     * therefore, it always returns false.
     */
    __constexpr virtual bool
        subclass_has_current_element() const noexcept;
    /*!
     * Generates the next element. Will alwyas return false.
     */
    __constexpr virtual bool
        subclass_generate_next();
    __constexpr virtual void
        subclass_set_data_using_mode_and_repetition_data(
            const std::size_t          _a_mode,
            const file_type_rep_data_t _a_rep_type
        );
    __constexpr virtual bool
        are_failed_values_written_to_files() const noexcept;

    __constexpr virtual file_type_rep_data_t
        subclass_get_repetition_data() const noexcept
    {
        return file_type_rep_data_t();
    }
};
_END_ABC_DG_NS
_BEGIN_ABC_NS
template <typename T, typename R = std::initializer_list<T>>
__constexpr _ABC_NS_DG::data_generator_collection_t<T,true>
read_data_from_file(
                const utility::io::file_name_t& _a_filename,
    R&& _a_init_elements = R{}
            );
_END_ABC_NS

_BEGIN_ABC_DG_NS
template <typename T>
template <typename R>
__constexpr_imp
    file_data_t<T>::file_data_t(
        const utility::io::file_rw_info_t<T>& _a_templated_file_rw,
        R&&                                   _a_initial_values
    )
    : data_generator_with_file_support_and_element_storage_t<
          T,
          file_type_rep_data_t,true,false>(
          0,
          _a_templated_file_rw,
          std::forward<R>(_a_initial_values),
          "",
          utility::io::opt_file_rw_info_t<file_type_rep_data_t>{},
          T{},
          &_ABC_NS_GLOBAL::get_global_test_options()
      )
{}

template <typename T>
__constexpr_imp bool
    file_data_t<T>::subclass_has_current_element() const noexcept
{
    return false;
}

template <typename T>
__constexpr_imp bool
    file_data_t<T>::subclass_generate_next()
{
    return false;
}

template <typename T>
__constexpr_imp void
    file_data_t<T>::subclass_set_data_using_mode_and_repetition_data(
        const std::size_t          _a_mode,
        const file_type_rep_data_t _a_rep_type
    )
{}

template <typename T>
__constexpr_imp bool
    file_data_t<T>::are_failed_values_written_to_files() const noexcept
{
    return false;
}
_END_ABC_DG_NS
_BEGIN_ABC_NS
template <typename T, typename R>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T, true>
read_data_from_file(
                    const utility::io::file_name_t& _a_filename,
                    R&&                             _a_init_elements
                )
{
    using namespace std;
    using namespace _ABC_NS_DG;
    return unary_collection<T>(make_shared<file_data_t<T>>(
        utility::io::file_rw_info_t<T>(_a_filename),
        forward<R>(_a_init_elements)
    ));
}

_END_ABC_NS
#endif