#pragma once

#include "abc_test/internal/data_generator/data_generator_with_file_support_concept.h"
#include "abc_test/internal/utility/internal/macros.h"
#include "abc_test/internal/utility/io/file/file_reader.h"
#include "abc_test/internal/utility/io/file/file_writer.h"

_BEGIN_ABC_UTILITY_IO_NS

template <typename T>
requires data_gen::concept_for_data_generator_with_file_support<T>
class file_read_writer_t
{
public:
    __constexpr
    file_read_writer_t(
        const T&                                                    _a_object,
        const utility::io::file_name_t<typename T::generator_type>& _a_frw
    );
    __constexpr std::size_t
                write_data_to_file(const T& _a_element);
    __constexpr std::size_t
                get_file_position() const noexcept;
    __constexpr void
        set_data_generator_using_additional_data(const std::string_view _a_str
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
    std::optional<str::rw_info_t<typename T::generator_type>> _m_opt_rw_info;
    file_line_reader_t                                        _m_line_reader;
    file_line_writer_t                                        _m_line_writer;
    std::filesystem::path                                     _m_path;
    T                                                         _m_object;
    std::size_t                _m_elements_read_or_written{0};
    typename T::generator_type _m_element;

    __constexpr void
        set_element();
};

_END_ABC_UTILITY_IO_NS

_BEGIN_ABC_UTILITY_IO_NS
template <typename T>
requires data_gen::concept_for_data_generator_with_file_support<T>
__constexpr
file_read_writer_t<T>::file_read_writer_t(
    const T&                                                    _a_object,
    const utility::io::file_name_t<typename T::generator_type>& _a_frw
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
        _l_flw.write_comment(typeid(T).name());
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
            file_read_writer_t<T>::write_data_to_file(
        const T& _a_element
    )
{
    using namespace abc::utility::str;
    if (_m_opt_rw_info.has_value())
    {
        _m_line_writer.write_line(_m_opt_rw_info.value().printer().run_printer(
            _a_element.current_element()
        ));
    }
    else
    {
        _m_line_writer.write_line(printer_t<typename T::tertiary_type>()
                                      .run_printer(_a_element.tertiary_data()));
    }

    return ++_m_elements_read_or_written;
}

template <typename T>
requires data_gen::concept_for_data_generator_with_file_support<T>
__constexpr std::size_t
            file_read_writer_t<T>::get_file_position() const noexcept
{
    return _m_elements_read_or_written;
}

template <typename T>
requires data_gen::concept_for_data_generator_with_file_support<T>
__constexpr void
    file_read_writer_t<T>::set_data_generator_using_additional_data(
        const std::string_view _a_str
    ) noexcept
{}

template <typename T>
requires data_gen::concept_for_data_generator_with_file_support<T>
__constexpr void
    file_read_writer_t<T>::set_to_write()
{
    using namespace abc::utility::io;
    _m_line_reader = file_line_reader_t();
    _m_line_writer = file_line_writer_t(_m_path);
}

template <typename T>
requires data_gen::concept_for_data_generator_with_file_support<T>
__constexpr const typename T::generator_type&
    file_read_writer_t<T>::current_element() const
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
    file_read_writer_t<T>::has_current_element()
{
    return _m_line_reader.has_current_line();
}

template <typename T>
requires data_gen::concept_for_data_generator_with_file_support<T>
__constexpr bool
    file_read_writer_t<T>::generate_next()
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
    file_read_writer_t<T>::set_element()
{
    if (_m_opt_rw_info.has_value())
    {
        _m_element = _m_opt_rw_info.value().parser().run_parser_with_exception(
            _m_line_reader.current_line()
        );
    }
    else
    {
        _m_object.set_generator_using_tertiary_data(
            abc::utility::str::parser_t<typename T::tertiary_type>()
                .run_parser_with_exception(_m_line_reader.current_line())
        );
    }
}

_END_ABC_UTILITY_IO_NS