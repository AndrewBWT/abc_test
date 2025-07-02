#pragma once

#include "abc_test/core.hpp"
#include "abc_test/core/data_generator/data_generator_collection.hpp"
#include "abc_test/core/global.hpp"
#include "abc_test/core/test_runner.hpp"
#include "abc_test/utility/io/file/file_line_writer.hpp"
#include "abc_test/utility/io/file/file_name.hpp"

#include <filesystem>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <vector>

_BEGIN_ABC_DG_NS

struct position_data_t
{
    std::size_t file_index{0};
    std::size_t element_index{0};
    __constexpr bool
        operator==(const position_data_t&) const noexcept
        = default;
};

_END_ABC_DG_NS
_BEGIN_ABC_UTILITY_PRINTER_NS

template <>
struct default_printer_t<data_gen::position_data_t>
    : public printer_base_t<data_gen::position_data_t>
{
public:
    static constexpr bool is_specialized{true};

    __constexpr           std::u8string
                          run_printer(
                              const data_gen::position_data_t& _a_object
                          ) const
    {
        using namespace std;
        return default_printer_t<pair<size_t, size_t>>{}.run_printer(
            make_pair(_a_object.file_index, _a_object.element_index)
        );
    }
};

_END_ABC_UTILITY_PRINTER_NS
_BEGIN_ABC_UTILITY_PARSER_NS

template <>
struct default_parser_t<data_gen::position_data_t>
    : public parser_base_t<data_gen::position_data_t>
{
public:
    __constexpr virtual result_t<data_gen::position_data_t>
        run_parser(
            parser_input_t& _a_parse_input
        ) const
    {
        using namespace std;
        return default_parser_t<pair<size_t, size_t>>()
            .run_parser(_a_parse_input)
            .transform(
                [](const pair<size_t, size_t>& _a_pair)
                {
                    return data_gen::position_data_t{
                        _a_pair.first, _a_pair.second
                    };
                }
            );
    }
};

_END_ABC_UTILITY_PARSER_NS
_BEGIN_ABC_DG_NS

template <typename T>
class file_data_generator_t
{
public:
    using file_data_t = std::list<utility::io::general_data_with_rw_info_t<T>>;
    using generator_type = T;
    using tertiary_type  = position_data_t;
    __constexpr const tertiary_type&
        tertiary_data() const;
    __constexpr void
        set_generator_using_tertiary_data(const tertiary_type& _a_tertiary_data
        );

    /*template <typename R>
    __constexpr
    file_data_generator_t(
        const utility::io::general_data_t& _a_gdf,
        R&&                                _a_elements

        template <typename T, typename... Args>
__constexpr _ABC_NS_DG::data_generator_collection_t<T>
            enumerate_data(
                const _ABC_NS_DG::enumeration_schema_t<T>& _a_es,
                Args... _a_file_reader_writers
            );
    );*/
    template <typename R>
    __constexpr
    file_data_generator_t(
        R&& _a_init_elements,
        std::list<utility::io::general_data_with_rw_info_t<T>>&&
            _a_file_data_names
    );

    ~file_data_generator_t() // I. destructor
    {}

    __constexpr
    file_data_generator_t(
        const file_data_generator_t& _a_arg
    ) // II. copy constructor
        : _m_files(_a_arg._m_files)
        , _m_position_data(_a_arg._m_position_data)
        , _m_index_rw_info(_a_arg._m_index_rw_info)
    {
        using namespace _ABC_NS_UTILITY_STR;
        _m_current_file_data = begin(_m_files);
        advance(_m_current_file_data, _m_position_data.file_index);
        if (_m_current_file_data != std::end(_m_files))
        {
            _m_line_reader = utility::io::file_line_reader_t(
                (*_m_current_file_data).general_data_file().path()
            );
            if (_m_position_data.element_index > 0)
            {
                skip_lines(_m_position_data.element_index - 1);
            }
            const result_t<T> _l_parsed_result{abc::utility::parser::parse(
                _m_line_reader.current_line(),
                (*_m_current_file_data).rw_info().internal_parser
            )};
            if (_l_parsed_result.has_value())
            {
                _m_element = _l_parsed_result.value();
            }
            else
            {
                throw errors::test_library_exception_t(fmt::format(
                    u8"Error encountered in file_data_generator_t<{0}> "
                    u8"when reading file \"{1}\" at line {2}. Parser "
                    u8"reported "
                    u8"error \"{3}\".",
                    type_id<T>(),
                    (*_m_current_file_data)
                        .general_data_file()
                        .path()
                        .u8string(),
                    _m_line_reader.line_number(),
                    _l_parsed_result.error()
                ));
            }
        }
    }

    __constexpr file_data_generator_t&
        operator=(
            const file_data_generator_t& _a_arg
        ) // III. copy assignment
    {
        _m_files             = _a_arg._m_files;
        _m_position_data     = _a_arg._m_position_data;
        _m_index_rw_info     = _a_arg._m_index_rw_info;
        _m_current_file_data = begin(_m_files);
        advance(_m_current_file_data, _m_position_data.file_index);
        _m_line_reader = utility::io::file_line_reader_t(
            (*_m_current_file_data).general_data_file().path()
        );
        skip_lines(_m_position_data.element_index);
        return *this;
    }

    /*template <typename R>
    __constexpr
    file_data_generator_t(
        const utility::io::general_data_with_rw_info_t<T>& _a_gdf,
        R&&                                                _a_elements
    );*/
    __constexpr void
        reset();
    __constexpr void
        reset_file(const std::size_t _a_file_index = 0);
    __constexpr void
        skip_lines(const std::size_t _a_idx);
    __constexpr bool
        has_current_element() const;
    __constexpr bool
        generate_next();
    __constexpr const T&
        current_element() const;
private:
    file_data_t                                   _m_files;
    file_data_t::iterator                         _m_current_file_data;
    position_data_t                               _m_position_data;
    abc::utility::io::file_line_reader_t          _m_line_reader;
    abc::utility::str::rw_info_t<position_data_t> _m_index_rw_info;
    T                                             _m_element;
};

namespace detail
{
template <typename Parse_Type, typename T>
concept general_data_with_rw_info_creatable_c
    = std::convertible_to<
          T,
          abc::utility::io::general_data_with_rw_info_t<Parse_Type>>
      || std::convertible_to<T, std::string>
      || std::convertible_to<T, abc::utility::io::general_data_t>
      || std::convertible_to<T, std::string_view>;
} // namespace detail

_END_ABC_DG_NS

_BEGIN_ABC_NS
/*template <typename T, typename R = std::initializer_list<T>>
requires std::same_as<std::ranges::range_value_t<R>, T>
__constexpr
    _ABC_NS_DG::data_generator_collection_t<std::ranges::range_value_t<R>>
    read_data_from_file(
        const std::string_view& _a_data_file_name,
        R&&                     _a_init_list = R{}
    );*/

template <typename T, typename R, typename... Args>
requires std::same_as<std::ranges::range_value_t<R>, T>
         && (abc::data_gen::detail::
                 general_data_with_rw_info_creatable_c<T, Args>
             && ...)
__constexpr _ABC_NS_DG::data_generator_collection_t<T> read_data_from_file(
    R&& _a_init_data,
    Args&&... _a_file_elements
);

template <typename T, typename... Args>
requires (
    abc::data_gen::detail::general_data_with_rw_info_creatable_c<T, Args> && ...
)
__constexpr _ABC_NS_DG::data_generator_collection_t<T>
            read_data_from_file(Args&&... _a_file_elements);

/*template <typename T, typename R = std::initializer_list<T>>
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
    */
_END_ABC_NS

_BEGIN_ABC_DG_NS
template <typename T>
__constexpr_imp const file_data_generator_t<T>::tertiary_type&
                      file_data_generator_t<T>::tertiary_data() const
{
    return _m_position_data;
}

template <typename T>
__constexpr_imp void
    file_data_generator_t<T>::set_generator_using_tertiary_data(
        const file_data_generator_t<T>::tertiary_type& _a_tertiary_data
    )
{
    if (_a_tertiary_data.file_index != _m_position_data.file_index)
    {
        reset_file(_a_tertiary_data.file_index);
    }
    _m_position_data.element_index = 0;
    if (_a_tertiary_data.element_index != _m_position_data.element_index)
    {
        skip_lines(
            _a_tertiary_data.element_index - _m_position_data.element_index
        );
    }
    if (_a_tertiary_data == _m_position_data)
    {
        _m_position_data = _a_tertiary_data;
        _m_element       = abc::utility::parser::parse_with_exception(
            _m_line_reader.current_line(),
            (*_m_current_file_data).rw_info().internal_parser
        );
    }
}

/*template <typename T>
template <typename R>
__constexpr_imp
    file_data_generator_t<T>::file_data_generator_t(
        const utility::io::general_data_t& _a_gdf,
        R&&                                _a_elements
    )
    : _m_path(abc::utility::io::normalise_for_file_use(_a_gdf.path().u8string())
      )
{
    using namespace abc::utility::io;
    using namespace std::filesystem;
    std::error_code ec;
    if (not std::filesystem::exists(_m_path, ec))
    {
        file_line_writer_t _l_flw(_m_path);
        _l_flw.write_comment(type_id<T>());
        for (auto&& _l_element : _a_elements)
        {
            _l_flw.write_line(
                _m_element_rw_info.internal_printer->run_printer(_l_element)
            );
        }
    }
    reset_file();
}*/
template <typename T>
template <typename R>
__constexpr_imp
    file_data_generator_t<T>::file_data_generator_t(
        R&& _a_init_elements,
        std::list<utility::io::general_data_with_rw_info_t<T>>&&
            _a_file_data_names
    )
    : _m_files(_a_file_data_names)
{
    using namespace abc::utility::io;
    using namespace std::filesystem;
    using namespace _ABC_NS_UTILITY_STR;
    for (auto& _l_element : _m_files)
    {
        auto& _l_path{_l_element.general_data_file().path()};
        if (not exists(_l_path))
        {
            file_line_writer_t _l_flw(_l_path);
            _l_flw.write_comment(type_id<T>());
            for (auto&& _l_init_element : _a_init_elements)
            {
                _l_flw.write_line(
                    _l_element.rw_info().internal_printer->run_printer(
                        _l_init_element
                    )
                );
            }
        }
    }
    reset_file();
}

/*template <typename T>
template <typename R>
__constexpr_imp
    file_data_generator_t<T>::file_data_generator_t(
        const utility::io::general_data_with_rw_info_t<T>& _a_gdf,
        R&&                                                _a_elements
    )
    : _m_files(_a_gdf)
{
    using namespace abc::utility::io;
    using namespace std::filesystem;
    for (auto& _l_element : _m_files)
    {
        auto& _l_path{ _l_element.general_data_file() };
        if (not exists(_l_path))
        {
            file_line_writer_t _l_flw(_l_path);
            _l_flw.write_comment(type_id<T>());
            for (auto&& _l_element : _a_elements)
            {
                _l_flw.write_line(
                    _m_element_rw_info.internal_printer->run_printer(_l_element)
                );
            }
        }
    }
    reset_file();
}*/

template <typename T>
__constexpr_imp void
    file_data_generator_t<T>::reset()
{
    _m_position_data = position_data_t();
    reset_file();
}

template <typename T>
__constexpr_imp void
    file_data_generator_t<T>::reset_file(
        const std::size_t _a_file_index
    )
{
    using namespace std;
    using namespace abc::utility::io;
    using namespace _ABC_NS_UTILITY_STR;
    _m_current_file_data = begin(_m_files);
    std::advance(_m_current_file_data, _a_file_index);
    _m_position_data.file_index    = _a_file_index;
    _m_position_data.element_index = 0;
    bool _l_found_element{false};
    do
    {
        auto _l_path{(*_m_current_file_data).general_data_file().path()};
        _m_line_reader = file_line_reader_t(_l_path);
        if (_m_line_reader.has_current_line())
        {
            ++_m_position_data.element_index;
            const result_t<T> _l_parsed_result{abc::utility::parser::parse(
                _m_line_reader.current_line(),
                (*_m_current_file_data).rw_info().internal_parser
            )};
            if (_l_parsed_result.has_value())
            {
                _m_element       = _l_parsed_result.value();
                _l_found_element = true;
            }
            else
            {
                throw errors::test_library_exception_t(fmt::format(
                    u8"Error encountered in file_data_generator_t<{0}> "
                    u8"when reading file \"{1}\" at line {2}. Parser reported "
                    u8"error \"{3}\".",
                    type_id<T>(),
                    _l_path.u8string(),
                    _m_line_reader.line_number(),
                    _l_parsed_result.error()
                ));
            }
        }
        else
        {
            _m_position_data.element_index = 0;
            _m_position_data.file_index++;
            ++_m_current_file_data;
            if (_m_current_file_data == end(_m_files))
            {
                return;
            }
        }
    }
    while (_l_found_element == false);
}

template <typename T>
__constexpr_imp void
    file_data_generator_t<T>::skip_lines(
        const std::size_t _a_idx
    )
{
    for (size_t _l_idx{0}; _l_idx < _a_idx; ++_l_idx)
    {
        _m_line_reader.get_next_line();
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
    using namespace _ABC_NS_UTILITY_STR;
    bool _l_element_found{false};
    do
    {
        if (_m_line_reader.get_next_line())
        {
            ++_m_position_data.element_index;
            const result_t<T> _l_parsed_result{abc::utility::parser::parse(
                _m_line_reader.current_line(),
                (*_m_current_file_data).rw_info().internal_parser
            )};
            if (_l_parsed_result.has_value())
            {
                _m_element       = _l_parsed_result.value();
                _l_element_found = true;
            }
            else
            {
                throw errors::test_library_exception_t(fmt::format(
                    u8"Error encountered in file_data_generator_t<{0}> "
                    u8"when reading file \"{1}\" at line {2}. Parser reported "
                    u8"error \"{3}\".",
                    type_id<T>(),
                    (*_m_current_file_data)
                        .general_data_file()
                        .path()
                        .u8string(),
                    _m_line_reader.line_number(),
                    _l_parsed_result.error()
                ));
            }
        }
        else
        {
            /*
    abc::utility::io::file_line_reader_t          _m_line_reader;
    T                                             _m_element;
            */
            ++_m_current_file_data;
            _m_position_data.file_index++;
            _m_position_data.element_index = 0;
            if (_m_current_file_data == _m_files.end())
            {
                return false;
            }
            _m_line_reader = utility::io::file_line_reader_t(
                (*_m_current_file_data).general_data_file().path()
            );
            if (_m_line_reader.has_current_line())
            {
                const result_t<T> _l_parsed_result{abc::utility::parser::parse(
                    _m_line_reader.current_line(),
                    (*_m_current_file_data).rw_info().internal_parser
                )};
                if (_l_parsed_result.has_value())
                {
                    _m_element = _l_parsed_result.value();
                    _l_element_found = true;
                }
                else
                {
                    throw errors::test_library_exception_t(fmt::format(
                        u8"Error encountered in file_data_generator_t<{0}> "
                        u8"when reading file \"{1}\" at line {2}. Parser "
                        u8"reported "
                        u8"error \"{3}\".",
                        type_id<T>(),
                        (*_m_current_file_data)
                            .general_data_file()
                            .path()
                            .u8string(),
                        _m_line_reader.line_number(),
                        _l_parsed_result.error()
                    ));
                }
            }
        }
    }
    while (_l_element_found == false);
    return true;
}

template <typename T>
__constexpr_imp const T&
    file_data_generator_t<T>::current_element() const
{
    return _m_element;
}

_END_ABC_DG_NS

_BEGIN_ABC_NS

/*template <typename T, typename R>
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
}*/

template <typename T, typename R, typename... Args>
requires std::same_as<std::ranges::range_value_t<R>, T>
         && (
             abc::data_gen::detail::
                 general_data_with_rw_info_creatable_c<T, Args>
             && ...
         )
__constexpr _ABC_NS_DG::data_generator_collection_t<T> read_data_from_file(
    R&& _a_init_data,
    Args&&... _a_file_elements
)
{
    using namespace std;
    using namespace _ABC_NS_DG;
    using namespace utility::io;
    list<general_data_with_rw_info_t<T>> _l_fdns;
    auto _l_process_func = [&]<typename Arg>(Arg&& _a_file_name...)
    {
        if constexpr (convertible_to<Arg, string_view>)
        {
            _l_fdns.push_back(general_data_with_rw_info_t<T>(gdf(_a_file_name))
            );
        }
        else if constexpr (convertible_to<Arg, general_data_with_rw_info_t<T>>)
        {
            _l_fdns.push_back(_a_file_name);
        }
        else if constexpr (convertible_to<Arg, general_data_t>)
        {
            _l_fdns.push_back(general_data_with_rw_info_t<T>(_a_file_name));
        }
        else
        {
            __STATIC_ASSERT(Arg, "Coulnd't work");
        }
    };
    (_l_process_func(std::forward<Args>(_a_file_elements)), ...);
    return unary_collection<
        T>(make_shared<
           data_generator_with_file_support_t<file_data_generator_t<T>, false>>(
        file_data_generator_t<T>(
            std::forward<R>(_a_init_data), std::forward<decltype(_l_fdns)>(_l_fdns)
        )
    ));
}

template <typename T, typename... Args>
requires (
    abc::data_gen::detail::general_data_with_rw_info_creatable_c<T, Args> && ...
)
__constexpr _ABC_NS_DG::data_generator_collection_t<T>
            read_data_from_file(
                Args&&... _a_file_elements
            )
{
    using namespace std;
    return read_data_from_file<T, initializer_list<T>>(
        initializer_list<T>{}, _a_file_elements...
    );
}

/*template <typename T, typename R>
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
    return read_data_from_file(
        utility::io::general_data_with_rw_info_t(
            _a_general_data_file, _ABC_NS_UTILITY::str::rw_info_t<T>()
        ),
        _a_init_list
    );
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
}*/

_END_ABC_NS