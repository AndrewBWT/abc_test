#pragma once

#include "abc_test/core.hpp"
#include "abc_test/internal/data_generator/data_generator_collection.hpp"
#include "abc_test/internal/global.hpp"
#include "abc_test/internal/test_runner.hpp"
#include "abc_test/internal/utility/io/file/file_line_writer.hpp"
#include "abc_test/internal/utility/io/file/file_name.hpp"

#include <filesystem>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <scn/ranges.h>
#include <scn/scan.h>
#include <vector>

_BEGIN_ABC_DG_NS

template <typename T>
class file_data_generator_t
{
public:
    using generator_type = T;
    using tertiary_type  = std::size_t;
    __constexpr const tertiary_type&
        tertiary_data() const;
    __constexpr void
        set_generator_using_tertiary_data(const tertiary_type& _a_tertiary_data
        );
    template <typename R>
    __constexpr
    file_data_generator_t(
        const utility::io::general_data_t& _a_gdf,
        R&&                                _a_elements
    );
    template <typename R>
    __constexpr
    file_data_generator_t(
        const utility::io::general_data_with_rw_info_t<T>& _a_gdf,
        R&&                                                _a_elements
    );
    __constexpr void
        reset();
    __constexpr void
        reset_file();
    __constexpr void
        skip_lines(const std::size_t _a_idx);
    __constexpr bool
        has_current_element() const;
    __constexpr bool
        generate_next();
    __constexpr const T&
        current_element() const;
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
__constexpr
    _ABC_NS_DG::data_generator_collection_t<std::ranges::range_value_t<R>>
    read_data_from_file(
        const std::string_view& _a_data_file_name,
        R&&                     _a_init_list = R{}
    );
template <typename T, typename R = std::initializer_list<T>>
requires std::same_as<std::ranges::range_value_t<R>, T>
__constexpr
    _ABC_NS_DG::data_generator_collection_t<std::ranges::range_value_t<R>>
    read_data_from_file(
        const utility::io::general_data_t& _a_general_data_file,
        R&&                                _a_init_list = R{}
    );
template <typename T, typename R = std::initializer_list<T>>
requires std::same_as<std::ranges::range_value_t<R>, T>
__constexpr
    _ABC_NS_DG::data_generator_collection_t<std::ranges::range_value_t<R>>
    read_data_from_file(
        const utility::io::general_data_with_rw_info_t<T>& _a_general_data_file,
        R&&                                                _a_init_list = R{}
    );

_END_ABC_NS

_BEGIN_ABC_DG_NS
template <typename T>
__constexpr_imp const file_data_generator_t<T>::tertiary_type&
                      file_data_generator_t<T>::tertiary_data() const
{
    return _m_elements_read;
}

template <typename T>
__constexpr_imp void
    file_data_generator_t<T>::set_generator_using_tertiary_data(
        const file_data_generator_t<T>::tertiary_type& _a_tertiary_data
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
        _m_element       = abc::utility::parser::run_parser_with_exception(
            _m_line_reader.current_line(), _m_element_rw_info.parser()
        );
    }
}

template <typename T>
template <typename R>
__constexpr_imp
    file_data_generator_t<T>::file_data_generator_t(
        const utility::io::general_data_t& _a_gdf,
        R&&                                _a_elements
    )
    : _m_path(_a_gdf.path())
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
                _m_element_rw_info.printer()->run_printer(_l_element)
            );
        }
    }
    reset_file();
}

template <typename T>
template <typename R>
__constexpr_imp
    file_data_generator_t<T>::file_data_generator_t(
        const utility::io::general_data_with_rw_info_t<T>& _a_gdf,
        R&&                                                _a_elements
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

template <typename T>
__constexpr_imp void
    file_data_generator_t<T>::reset()
{
    _m_elements_read = 0;
    reset_file();
}

template <typename T>
__constexpr_imp void
    file_data_generator_t<T>::reset_file()
{
    using namespace abc::utility::io;
    _m_line_reader = file_line_reader_t(_m_path);
    if (_m_line_reader.has_current_line())
    {
        ++_m_elements_read;
        _m_element = abc::utility::parser::run_parser_with_exception(
            _m_line_reader.current_line(), _m_element_rw_info.parser()
        );
    }
}

template <typename T>
__constexpr_imp void
    file_data_generator_t<T>::skip_lines(
        const std::size_t _a_idx
    )
{
    while (_m_elements_read > _a_idx)
    {
        _m_line_reader.get_next_line();
        ++_m_elements_read;
    }
}

template <typename T>
__constexpr_imp bool
    file_data_generator_t<T>::has_current_element() const
{
    return _m_line_reader.has_current_line();
}

template <typename T>
__constexpr_imp bool
    file_data_generator_t<T>::generate_next()
{
    if (_m_line_reader.get_next_line())
    {
        ++_m_elements_read;
        _m_element = abc::utility::parser::run_parser_with_exception(
            _m_line_reader.current_line(),
            _m_element_rw_info.parser()
        );
        return true;
    }
    else
    {
        return false;
    }
}

template <typename T>
__constexpr_imp const T&
    file_data_generator_t<T>::current_element() const
{
    return _m_element;
}

_END_ABC_DG_NS

_BEGIN_ABC_NS
template <typename T, typename R>
requires std::same_as<std::ranges::range_value_t<R>, T>
__constexpr_imp
    _ABC_NS_DG::data_generator_collection_t<std::ranges::range_value_t<R>>
    read_data_from_file(
        const std::string_view& _a_data_file_name,
        R&&                     _a_init_list
    )
{
    return read_data_from_file<T, R>(
        gdf(_a_data_file_name), std::forward<R>(_a_init_list)
    );
}

template <typename T, typename R>
requires std::same_as<std::ranges::range_value_t<R>, T>
__constexpr_imp
    _ABC_NS_DG::data_generator_collection_t<std::ranges::range_value_t<R>>
    read_data_from_file(
        const utility::io::general_data_t& _a_general_data_file,
        R&&                                _a_init_list
    )
{
    using namespace std;
    using namespace _ABC_NS_DG;
    return unary_collection<
        T>(make_shared<
           data_generator_with_file_support_t<file_data_generator_t<T>, false>>(
        file_data_generator_t<T>(_a_general_data_file, _a_init_list)
    ));
}

template <typename T, typename R>
requires std::same_as<std::ranges::range_value_t<R>, T>
__constexpr_imp
    _ABC_NS_DG::data_generator_collection_t<std::ranges::range_value_t<R>>
    read_data_from_file(
        const utility::io::general_data_with_rw_info_t<T>& _a_general_data_file,
        R&&                                                _a_init_list
    )
{
    using namespace std;
    using namespace _ABC_NS_DG;
    return unary_collection<
        T>(make_shared<
           data_generator_with_file_support_t<file_data_generator_t<T>, false>>(
        file_data_generator_t<T>(_a_general_data_file, _a_init_list)
    ));
}

_END_ABC_NS