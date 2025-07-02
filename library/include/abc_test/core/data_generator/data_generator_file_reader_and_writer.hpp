#pragma once

#include "abc_test/core/data_generator/data_generator_with_file_support_concept.hpp"
#include "abc_test/utility/internal/macros.hpp"
#include "abc_test/utility/io/file/file_line_reader.hpp"
#include "abc_test/utility/io/file/file_line_writer.hpp"

_BEGIN_ABC_DG_NS

template <typename T>
requires data_gen::concept_for_data_generator_with_file_support<T>
class data_generator_file_reader_and_writer_t
{
public:
    __constexpr
    data_generator_file_reader_and_writer_t(
        const T&                                                    _a_object,
        const utility::io::file_name_t<typename T::generator_type>& _a_frw,
        const std::u8string_view _a_comment_str
    );
    __constexpr std::size_t
                write_data_to_file(const T& _a_element);
    __constexpr std::size_t
                get_file_position() const noexcept;
    __constexpr void
        set_data_generator_using_additional_data(const std::size_t _a_idx
        ) noexcept;
    __constexpr void
        set_to_write();
    __constexpr const typename T::generator_type&
        current_element() const;
    __constexpr bool
        has_current_element();
    __constexpr bool
        generate_next();
private:
    std::optional<utility::str::rw_info_t<typename T::generator_type>>
                                    _m_opt_rw_info;
    utility::io::file_line_reader_t _m_line_reader;
    utility::io::file_line_writer_t _m_line_writer;
    std::filesystem::path           _m_path;
    T                               _m_object;
    std::size_t                     _m_elements_read_or_written{0};
    typename T::generator_type      _m_element;

    __constexpr void
        set_element();
};

_END_ABC_DG_NS

_BEGIN_ABC_DG_NS
template <typename T>
requires data_gen::concept_for_data_generator_with_file_support<T>
__constexpr
data_generator_file_reader_and_writer_t<T>::
    data_generator_file_reader_and_writer_t(
        const T&                                                    _a_object,
        const utility::io::file_name_t<typename T::generator_type>& _a_frw,
        const std::u8string_view _a_comment_str
    )
    : _m_opt_rw_info(opt_rw_info(_a_frw))
    , _m_path(path(_a_frw))
    , _m_object(_a_object)
{
    using namespace std::filesystem;
    using namespace abc::utility::io;
    if (not exists(_m_path))
    {
        file_line_writer_t _l_flw(_m_path);
        _l_flw.write_comment(_a_comment_str);
    }
    _m_line_reader = file_line_reader_t(_m_path);
    if (_m_line_reader.has_current_line())
    {
        set_element();
    }
    else
    {
        set_to_write();
    }
}

template <typename T>
requires data_gen::concept_for_data_generator_with_file_support<T>
__constexpr std::size_t
            data_generator_file_reader_and_writer_t<T>::write_data_to_file(
        const T& _a_element
    )
{
    using namespace abc::utility::str;
    using namespace abc::utility::printer;
    if (_m_opt_rw_info.has_value())
    {
        _m_line_writer.write_line(
            _m_opt_rw_info.value().internal_printer->run_printer(
                _a_element.current_element()
            )
        );
    }
    else
    {
        _m_line_writer.write_line(default_printer<typename T::tertiary_type>()
                                      ->run_printer(_a_element.tertiary_data())
        );
    }

    return ++_m_elements_read_or_written;
}

template <typename T>
requires data_gen::concept_for_data_generator_with_file_support<T>
__constexpr std::size_t
            data_generator_file_reader_and_writer_t<T>::get_file_position(
    ) const noexcept
{
    return _m_elements_read_or_written;
}

template <typename T>
requires data_gen::concept_for_data_generator_with_file_support<T>
__constexpr void
    data_generator_file_reader_and_writer_t<T>::
        set_data_generator_using_additional_data(
            const std::size_t _a_idx
        ) noexcept
{
    while (_m_elements_read_or_written < _a_idx)
    {
        const bool _l_has_next_line{generate_next()};
        if (not _l_has_next_line)
        {
        }
    }
}

template <typename T>
requires data_gen::concept_for_data_generator_with_file_support<T>
__constexpr void
    data_generator_file_reader_and_writer_t<T>::set_to_write()
{
    using namespace abc::utility::io;
    _m_line_reader = file_line_reader_t();
    _m_line_writer = file_line_writer_t(_m_path);
}

template <typename T>
requires data_gen::concept_for_data_generator_with_file_support<T>
__constexpr const typename T::generator_type&
    data_generator_file_reader_and_writer_t<T>::current_element() const
{
    if (_m_opt_rw_info.has_value())
    {
        return _m_element;
    }
    else
    {
        return _m_object.current_element();
    }
}

template <typename T>
requires data_gen::concept_for_data_generator_with_file_support<T>
__constexpr bool
    data_generator_file_reader_and_writer_t<T>::has_current_element()
{
    return _m_line_reader.has_current_line();
}

template <typename T>
requires data_gen::concept_for_data_generator_with_file_support<T>
__constexpr bool
    data_generator_file_reader_and_writer_t<T>::generate_next()
{
    const bool _l_has_next_line{_m_line_reader.get_next_line()};
    if (_l_has_next_line)
    {
        _m_elements_read_or_written++;
        set_element();
    }
    return _l_has_next_line;
}

template <typename T>
requires data_gen::concept_for_data_generator_with_file_support<T>
__constexpr void
    data_generator_file_reader_and_writer_t<T>::set_element()
{
    using namespace abc::utility::parser;
    if (_m_opt_rw_info.has_value())
    {
        _m_element = parse_with_exception(
            _m_line_reader.current_line(),
            _m_opt_rw_info.value().internal_parser
        );
    }
    else
    {
        _m_object.set_generator_using_tertiary_data(parse_with_exception(
            _m_line_reader.current_line(),
            default_parser<typename T::tertiary_type>()
        ));
    }
}

_END_ABC_DG_NS