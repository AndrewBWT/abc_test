#pragma once
#include "abc_test/internal/ds/data_generator_memoization/data_generator_memoized_element.h"
#include "abc_test/internal/ds/data_generator_memoization/indexed_data_generator_collection_memoized_element.h"
#include "abc_test/internal/ds/type_synonyms.h"
#include "abc_test/internal/global.h"
#include "abc_test/internal/utility/io/file/file_name.h"
#include "abc_test/internal/utility/io/file/file_reader.h"
#include "abc_test/internal/utility/io/file/file_rw.h"
#include "abc_test/internal/utility/io/file/file_writer.h"
#include "abc_test/internal/utility/io/file/opt_file_rw.h"

#include <expected>
#include <functional>
#include <memory>
#include <optional>
#include <scn/scan.h>
#include <string>
#include <vector>

_BEGIN_ABC_DG_NS

template <typename I>
struct data_generator_traits_t
{
    using tertiary_type = I::tertiary_type;
    using value_type    = I::value_type;
};

/*template <typename T>
class basic_data_generator_t
{
public:
    typedef tertiary_type = 0;
    __constexpr virtual void
        reset()
        = 0;
    __constexpr virtual void
        set_data_generator_using_additional_data(const std::string_view _a_str)
        = 0;
    __constexpr virtual bool
        has_current_element()
        = 0;
    __constexpr virtual bool
        generate_next()
        = 0;
    __constexpr virtual const T&
        current_element()
        = 0;
    __constexpr virtual const std::string
        get_additional_data() const
        = 0;
    __constexpr virtual tertiary_type
        tertiary_data()
        = 0;
};*/

// template<typename T>
// using mf_t = std::string (basic_data_generator_t<T>::*)();
_END_ABC_DG_NS

_BEGIN_ABC_NS

// template <typename T>
// using mf_t
//    = const std::string (_ABC_NS_DG::basic_data_generator_t<T>::*)() const;

class file_type_t
{
public:
    inline virtual ~file_type_t()
    {}

    const std::filesystem::path&
        path() const noexcept
    {
        return _m_path;
    }
protected:
    std::filesystem::path _m_path;

    file_type_t(
        const std::filesystem::path& _a_str
    )
    {
        _m_path = _a_str;
    }
};

template <typename T>
class GDF : public file_type_t
{
public:
    abc::utility::str::rw_info_t<T> _m_rw_info;

    __constexpr                     std::string
                                    to_string(
                                        const T& _a_element
                                    )
    {
        return _m_rw_info.printer().run_printer(_a_element);
    }

    GDF(
        const std::filesystem::path&           _a_str,
        const abc::utility::str::rw_info_t<T>& _a_rw_info
    )
        : file_type_t(_a_str), _m_rw_info(_a_rw_info)
    {}

    // __constexpr mf_t<T> virtual get_write_element_func() const
    // {
    //     return
    //     &abc::data_gen::basic_data_generator_t<T>::get_additional_data;
    // }
};

class TDF : public file_type_t
{
public:
    TDF(
        const std::string_view _a_str
    )
        : file_type_t(_a_str)
    {}
};

template <typename T>
struct general_data_with_rw_info_t
{
public:
    __constexpr
    general_data_with_rw_info_t()
        = delete;
    std::filesystem::path           _m_path;
    abc::utility::str::rw_info_t<T> _m_rw_info;
};

struct general_data_t
{
public:
    std::filesystem::path _m_path;
};

struct tertiary_data_t
{
public:
    std::filesystem::path _m_path;
};

template <typename T>
using file_name_t = std::
    variant<general_data_with_rw_info_t<T>, general_data_t, tertiary_data_t>;

template <typename T>
__constexpr const std::filesystem::path&
                  path(
                      const file_name_t<T>& _a_file_name
                  )
{
    using namespace std;
    if (auto _l_ptr{get_if<general_data_with_rw_info_t<T>>(&_a_file_name)};
        _l_ptr != nullptr)
    {
        return _l_ptr->_m_path;
    }
    else if (auto _l_ptr{get_if<general_data_t>(&_a_file_name)};
             _l_ptr != nullptr)
    {
        return _l_ptr->_m_path;
    }
    else if (auto _l_ptr{get_if<tertiary_data_t>(&_a_file_name)};
             _l_ptr != nullptr)
    {
        return _l_ptr->_m_path;
    }
    else
    {
        throw errors::unaccounted_for_variant_exception(_a_file_name);
    }
}

template <typename T>
__constexpr std::optional<abc::utility::str::rw_info_t<T>>
            opt_rw_info(
                const file_name_t<T>& _a_file_name
            )
{
    using namespace std;
    using namespace abc::utility::str;
    if (auto _l_ptr{get_if<general_data_with_rw_info_t<T>>(&_a_file_name)};
        _l_ptr != nullptr)
    {
        return optional<rw_info_t<T>>(_l_ptr->_m_rw_info);
    }
    else if (auto _l_ptr{get_if<general_data_t>(&_a_file_name)};
             _l_ptr != nullptr)
    {
        return optional<rw_info_t<T>>(rw_info_t<T>{});
    }
    else if (auto _l_ptr{get_if<tertiary_data_t>(&_a_file_name)};
             _l_ptr != nullptr)
    {
        return optional<rw_info_t<T>>();
    }
    else
    {
        throw errors::unaccounted_for_variant_exception(_a_file_name);
    }
}

template <typename T>
using file_names_t = std::vector<file_name_t<T>>;

template <typename T>
using gdf_ptr_t = std::shared_ptr<GDF<T>>;

__no_constexpr_imp general_data_t
    general_data_file(
        const std::string_view _a_str
    )
{
    using namespace std;
    return general_data_t{_a_str};
}

template <typename T>
__no_constexpr_imp general_data_with_rw_info_t<T>
                   general_data_file(
                       const std::string_view                 _a_str,
                       const abc::utility::str::rw_info_t<T>& _a_rw_info
                   )
{
    using namespace std;
    return general_data_with_rw_info_t<T>{_a_str, _a_rw_info};
}

__no_constexpr_imp tertiary_data_t
    tertiary_data_file(
        const std::string_view _a_str
    )
{
    using namespace std;
    return tertiary_data_t{_a_str};
    // return make_shared<TDF>(_a_str);
}

_END_ABC_NS

_BEGIN_ABC_DG_NS

template <typename T>
// requires (Has_Core_Generator == true || N > 0)
class data_generator_t
{
public:
    __constexpr virtual void
        set_data_generator_using_data_generator_memoized_element(
            const ds::dg_memoized_element_t& _a_dgme
        )
        = 0;
    __constexpr virtual bool
        has_current_element()
        = 0;
    __constexpr virtual const T&
        current_element() const
        = 0;
    __constexpr virtual bool
        generate_next()
        = 0;
    __constexpr virtual ds::dg_memoized_element_t
        get_data_generator_memoized_element(
            const bool _a_get_original_dg_memoized_element_data
        )
        = 0;
};

template <typename T>
using data_generator_ptr_t = std::shared_ptr<data_generator_t<T>>;
template <typename T>
using data_generator_ptr_collection_t = std::vector<data_generator_ptr_t<T>>;

template <typename T>
class file_writer_t
{
private:
    std::optional<abc::utility::str::rw_info_t<T>> _m_opt_rw_info;
public:
    __constexpr bool
        has_printer_parser() const noexcept
    {
        return _m_opt_rw_info.has_value();
    }

    __no_constexpr_imp void
        write_line(
            const std::string_view _a_str
        ) noexcept
    {
        _m_line_writer.write_line(_a_str);
    }

    __no_constexpr_imp void
        write_line(
            const T& _a_element
        )
    {
        if (_m_opt_rw_info.has_value())
        {
            return write_line(
                _m_opt_rw_info.value().printer().run_printer(_a_element)
            );
        }
    }

    __no_constexpr
        file_writer_t(
            const file_name_t<T>& _a_frw
        )
        : _m_line_writer(abc::utility::io::file_line_writer_t(path(_a_frw)))
        , _m_opt_rw_info(opt_rw_info(_a_frw))
    {}

    abc::utility::io::file_line_writer_t _m_line_writer;
};

template <typename T>
requires basic_data_generator_c<T>
class file_read_writer_t
{
public:
    __constexpr ~file_read_writer_t(

    )
    {}

    __constexpr std::size_t
                write_data_to_file(
                    const T& _a_element
                )
    {
        using namespace abc::utility::str;
        if (_m_opt_rw_info.has_value())
        {
            _m_line_writer.write_line(
                _m_opt_rw_info.value().printer().run_printer(
                    _a_element.current_element()
                )
            );
        }
        else
        {
            _m_line_writer.write_line(
                printer_t<typename T::tertiary_type>().run_printer(
                    _a_element.tertiary_data()
                )
            );
        }

        return ++_m_elements_read_or_written;
    }

    __constexpr std::size_t
                get_file_position() const noexcept
    {
        return _m_elements_read_or_written;
    }

    T           _m_object;
    std::size_t _m_elements_read_or_written{0};

    __constexpr
    file_read_writer_t(
        const T&                                       _a_object,
        const file_name_t<typename T::generator_type>& _a_frw
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
            using namespace abc::utility::io;
            if (_m_opt_rw_info.has_value())
            {
                _m_element
                    = _m_opt_rw_info.value().parser().run_parser_with_exception(
                        _m_line_reader.current_line()
                    );
            }
            else
            {
                _m_object.set_generator_using_tertiary_data(
                    abc::utility::str::parser_t<typename T::tertiary_type>()
                        .run_parser_with_exception(_m_line_reader.current_line()
                        )
                );
            }
        }
        else
        {
            using namespace utility::io;
            _m_line_reader = file_line_reader_t();
            _m_line_writer = file_line_writer_t(_m_path);
        }
    }

    __constexpr void
        set_data_generator_using_additional_data(
            const std::string_view _a_str
        ) noexcept
    {}

    typename T::generator_type _m_element;

    __constexpr void
        set_to_write()
    {
        using namespace abc::utility::io;
        _m_line_reader              = file_line_reader_t();
        _m_line_writer              = file_line_writer_t(_m_path);
    }

    __constexpr const typename T::generator_type&
        current_element() const
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

    __constexpr bool
        has_current_element()
    {
        return _m_line_reader.has_current_line();
    }

    __constexpr bool
        generate_next()
    {
        const bool _l_has_next_line{_m_line_reader.get_next_line()};
        if (_l_has_next_line)
        {
            _m_elements_read_or_written++;
            if (_m_opt_rw_info.has_value())
            {
                _m_element
                    = _m_opt_rw_info.value().parser().run_parser_with_exception(
                        _m_line_reader.current_line()
                    );
            }
            else
            {
                _m_object.set_generator_using_tertiary_data(
                    abc::utility::str::parser_t<typename T::tertiary_type>()
                        .run_parser_with_exception(_m_line_reader.current_line()
                        )
                );
            }
        }
        return _l_has_next_line;
    }

    // __constexpr std::string
    //             get_additional_data() noexcept
    // {
    //     return "hello";
    // }

    // __constexpr std::string
    //            get_data_generator_string(
    //               const typename T::generator_type& _a_element
    //           ) noexcept
    // {
    //     return "hello2";
    // }
private:
    std::optional<abc::utility::str::rw_info_t<typename T::generator_type>>
                                         _m_opt_rw_info;
    abc::utility::io::file_line_reader_t _m_line_reader;
    abc::utility::io::file_line_writer_t _m_line_writer;
    std::filesystem::path                _m_path;
};

template <typename T>
concept basic_data_generator_c = requires (
    const T&                _a_t,
    T&                      _a_t_non_const,
    const T::tertiary_type& _a_tertiary_data
) {
    { _a_t.has_current_element() } -> std::same_as<bool>;
    { _a_t_non_const.generate_next() } -> std::same_as<bool>;
    {
        _a_t.current_element()
    } -> std::same_as<const typename T::generator_type&>;
    { _a_t.tertiary_data() } -> std::same_as<const typename T::tertiary_type&>;
    { _a_t_non_const.set_generator_using_tertiary_data(_a_tertiary_data) };
    { _a_t_non_const.reset() };
};

template <typename T, bool Has_File_Read_Writer>
requires basic_data_generator_c<T>
// requires (Has_Core_Generator == true || N > 0)
class data_generator_single_t
    : public data_generator_t<typename T::generator_type>
{
private:
    T _m_object;
    std::conditional_t<
        Has_File_Read_Writer,
        file_read_writer_t<T>,
        std::monostate>
                                                            _m_file_read_writer;
    std::size_t                                             _m_mode{0};
    std::vector<file_writer_t<typename T::generator_type>>  _m_file_writers;
    abc::utility::str::rw_info_t<typename T::tertiary_type> _m_tertiary_rw_info;
public:
    __constexpr ~data_generator_single_t()
    {}

    __constexpr
    data_generator_single_t(
        const T& _a_element
    )
        : _m_object(_a_element)
    {}

    __constexpr
    data_generator_single_t(
        const T&                                       _a_element,
        const file_name_t<typename T::generator_type>& _a_file_read_writer
    )
        : _m_object(_a_element)
        , _m_file_read_writer(
              file_read_writer_t<T>(_a_element, _a_file_read_writer)
          )
    {
        if (not _m_file_read_writer.has_current_element())
        {
            _m_mode = 1;
        }
    }

    __constexpr
    data_generator_single_t(
        const T&                                       _a_element,
        const file_name_t<typename T::generator_type>& _a_file_read_writer,
        const std::pair<
            typename file_names_t<typename T::generator_type>::const_iterator,
            typename file_names_t<typename T::generator_type>::const_iterator>&
            _a_pair_iterators
    )
        : _m_object(_a_element)
        , _m_file_read_writer(
              file_read_writer_t<T>(_a_element, _a_file_read_writer)
          )
    {
        if (not _m_file_read_writer.has_current_element())
        {
            _m_mode = 1;
        }
        for (typename file_names_t<typename T::generator_type>::const_iterator
                 _l_itt{_a_pair_iterators.first};
             _l_itt != _a_pair_iterators.second;
             ++_l_itt)
        {
            _m_file_writers.push_back(
                file_writer_t<typename T::generator_type>(*_l_itt)
            );
        }
    }

    __constexpr void
        switch_mode(
            const std::size_t _a_new_mode
        )
    {
        if (_m_mode != _a_new_mode)
        {
            if (_m_mode < _a_new_mode)
            {
                if constexpr (Has_File_Read_Writer)
                {
                    switch (_m_mode)
                    {
                    case 0:
                        // Currently 0, becoming 1. Set writer to writing.
                        _m_file_read_writer.set_to_write();
                        break;
                    default:
                        throw std::exception("couldn't");
                    }
                    _m_mode = _a_new_mode;
                }
            }
        }
        else
        {
            throw std::exception("coudln't");
        }
    }

    __constexpr virtual void
        set_data_generator_using_data_generator_memoized_element(
            const ds::dg_memoized_element_t& _a_dgme
        )
    {
        if constexpr (Has_File_Read_Writer)
        {
            switch_mode(_a_dgme.mode);
            switch (_a_dgme.mode)
            {
            case 0:
                _m_file_read_writer.set_data_generator_using_additional_data(
                    _a_dgme.additional_data
                );
                break;
            case 1:
                _m_object.set_generator_using_tertiary_data(
                    _m_tertiary_rw_info.parser().run_parser_with_exception(
                        _a_dgme.additional_data
                    )
                );
                break;
            default:
                throw std::exception("Couldn't work");
            }
        }
        else
        {
            switch (_a_dgme.mode)
            {
            case 0:
                _m_object.set_generator_using_tertiary_data(
                    _m_tertiary_rw_info.parser().run_parser_with_exception(
                        _a_dgme.additional_data
                    )
                );
                break;
            default:
                throw std::exception("Couldn't work");
            }
        }
    }

    __constexpr virtual bool
        has_current_element()
    {
        if constexpr (Has_File_Read_Writer)
        {
            switch (_m_mode)
            {
            case 0:
                return _m_file_read_writer.has_current_element();
            case 1:
                return _m_object.has_current_element();
            default:
                throw std::exception("Couldn't work");
            }
        }
        else
        {
            switch (_m_mode)
            {
            case 0:
                return _m_object.has_current_element();
            default:
                throw std::exception("Couldn't work");
            }
        }
    }

    __constexpr virtual const T::generator_type&
        current_element() const
    {
        if constexpr (Has_File_Read_Writer)
        {
            switch (_m_mode)
            {
            case 0:
                return _m_file_read_writer.current_element();
            case 1:
                return _m_object.current_element();
            default:
                throw std::exception("Couldn't work");
            }
        }
        else
        {
            switch (_m_mode)
            {
            case 0:
                return _m_object.current_element();
            default:
                throw std::exception("Couldn't work");
            }
        }
    }

    __constexpr virtual bool
        generate_next()
    {
        if constexpr (Has_File_Read_Writer)
        {
            switch (_m_mode)
            {
            case 0:
                if (not _m_file_read_writer.generate_next())
                {
                    _m_mode = 1;
                    _m_file_read_writer.set_to_write();
                    return true;
                }
                else
                {
                    return true;
                }
            case 1:
                return _m_object.generate_next();
            default:
                throw std::exception("Couldn't work");
            }
        }
        else
        {
            switch (_m_mode)
            {
            case 0:
                return _m_object.generate_next();
            default:
                throw std::exception("Couldn't work");
            }
        }
    }

    __constexpr virtual ds::dg_memoized_element_t
        get_data_generator_memoized_element(
            const bool _a_get_original_dg_memoized_element_data
        )
    {
        using namespace _ABC_NS_DS;
        using namespace std;
        if constexpr (Has_File_Read_Writer)
        {
            if (_a_get_original_dg_memoized_element_data)
            {
                string _l_rv;
                switch (_m_mode)
                {
                case 0:
                    _l_rv = abc::utility::str::printer_t<std::size_t>()
                                .run_printer(
                                    _m_file_read_writer.get_file_position()
                                );
                    break;
                case 1:
                    _l_rv = _m_object.get_additional_data();
                    break;
                default:
                    throw std::exception("Couldn't work");
                }
                return dg_memoized_element_t{_m_mode, _l_rv};
            }
            else
            {
                string _l_rv;
                size_t _l_mode = _m_mode;
                switch (_m_mode)
                {
                case 0:
                    _l_rv = abc::utility::str::printer_t<std::size_t>()
                                .run_printer(
                                    _m_file_read_writer.get_file_position()
                                );
                    break;
                case 1:
                    _l_mode = 0;
                    _l_rv
                        = abc::utility::str::printer_t<std::size_t>()
                              .run_printer(_m_file_read_writer
                                               .write_data_to_file(_m_object));
                    // file_writer_t<generator_type>
                    //_m_object. T.
                    // File writiers take a T. Turn it into a string?
                    for (file_writer_t<typename T::generator_type>& _l_writer :
                         _m_file_writers)
                    {
                        if (_l_writer.has_printer_parser())
                        {
                            _l_writer.write_line(_m_object.current_element());
                        }
                        else
                        {
                            _l_writer.write_line(
                                _m_tertiary_rw_info.printer().run_printer(
                                    _m_object.tertiary_data()
                                )
                            );
                        }
                    }
                    break;
                default:
                    throw std::exception("Couldn't work");
                }
                return dg_memoized_element_t{_l_mode, _l_rv};
            }
        }
        else
        {
            auto ki = _m_tertiary_rw_info.printer().run_printer(
                _m_object.tertiary_data()
            );
            return dg_memoized_element_t{
                _m_mode,
                _m_tertiary_rw_info.printer().run_printer(
                    _m_object.tertiary_data()
                )
            };
        }
    }
};

template <typename T>
class data_generator_single_with_file_list_t : public data_generator_t<T>
{};

_END_ABC_DG_NS
#if 0
/*!
 * @brief This abstract class represents the data generator; a mechanism used in
 * abc_test to generate data in manner which is useful when writing tests.
 *
 * data_generator_t allows the user to write loops which output data. By using
 * this abstract class, the user can easily add the ability for the generators
 * to read and write data to a file, and when the user wants to repeat tests but
 * only generate data which has resulted in a failure previously, this class can
 * be used to achieve that aim with little to no additional input from the user.
 *
 * @tparam T The output type from the generator.
 */
template <typename T>
struct data_generator_t
{
public:
    /*!
     * @brief Default constructor.
     */
    __constexpr
    data_generator_t() noexcept
        = default;
    __constexpr
    data_generator_t(const std::size_t _a_mode) noexcept;
    /*!
     * @brief Gets the mode of the object.
     * @return The mode.
     */
    __constexpr std::size_t
                mode() const noexcept;
    /*!
     * @brief Tells the caller whether the object has a current element.
     *
     * By current element, we mean that calling current_element() will return an
     * element which is well-formed.
     * @return True if there is a current element. False otherwise.
     */
    __constexpr virtual bool
        has_current_element() const
        = 0;
    /*!
     * @brief Returns a cref to the current element's caller.
     * @return A cref to the object's current element.
     */
    __constexpr virtual const T&
        current_element() const
        = 0;
    /*!
     * @brief Generates the next element in the generator.
     * @return True if the data_generator_t was able to generate a next entity;
     * false otherwise.
     */
    __constexpr virtual bool
        generate_next()
        = 0;
    /*!
     * @brief Sets the data_generator using the
     * data_generator_memoized_element_t argument.
     *
     * This function should set the state of the object to the exact state the
     * data_generator_t would be if it had got to this state organicly.
     *
     * @param _a_dgme The data_generator_memoized_element_t to set the object
     * to.
     */
    __constexpr virtual void
        set_data_generator_using_data_generator_memoized_element(
            const ds::dg_memoized_element_t& _a_dgme
        )
        = 0;
    /*!
     * @brief This function returns the dg_memoized_element_t to the caller
     * which represents this object.
     *
     * Care must be taken when defining this object as it has two modes.
     *
     * When _a_get_data_for_repeating is true, this function gets the
     * ds::dg_memoized_element_t with its data from memoized sources if
     * available.
     *
     * What this means is that, if we have generated some data A which, on
     * failure, is written to a file F. When
     * get_data_generator_memoized_element(true) is called, this function should
     * return the dg_memoized_element_t with data from the file.
     *
     * When _a_get_data_for_repeating is false, return the data used as if it
     * were generated. This may still be from a file, but it was originally from
     * a file (atleast on this run).
     *
     * @param _a_get_data_for_repeating Denotes whether to get the data from any
     * alternative source or the original source.
     * @return a ds::dg_memoized_element_t object representing the data
     * generated by this data_generator_t.
     */
    __constexpr virtual ds::dg_memoized_element_t
        get_data_generator_memoized_element(const bool _a_get_data_for_repeating
        ) noexcept
        = 0;
    /*!
     * @brief Gets the caller a string representation of the data used to create
     * the instance current_element() points at.
     * @return String representative of the internal data of this object.
     */
    __constexpr virtual std::string
        get_additional_data() const
        = 0;
protected:
    std::size_t                _m_mode = 0;
    utility::str::rw_info_t<T> _m_rw_info_t;
};

/*!
 * @brief Type synonym for a pointer to a data_generator_t element.
 * @tparam T The type param of the data_generator_t element.
 */
template <typename T>
using data_generator_ptr_t = std::shared_ptr<data_generator_t<T>>;

/*!
 * @brief Functino used to uniform errors::test_library_exception_t error for
 * when a mode is not supproted in a data_generator_t object.
 * @param _a_type_info The type_info of the element which called this function.
 * @param _a_mode The mode of the object which called this function.
 * @param _a_supported_elements The supported modes of the object which called
 * this function.
 * @return A errors::test_library_exception_t error object.
 */
__no_constexpr errors::test_library_exception_t
               unsupported_mode_exception(
                   const std::type_info&                     _a_type_info,
                   const std::size_t                         _a_mode,
                   const std::initializer_list<std::size_t>& _a_supported_elements
               );
_END_ABC_DG_NS

_BEGIN_ABC_DG_NS

template <typename T>
__constexpr_imp
    data_generator_t<T>::data_generator_t(
        const std::size_t _a_mode
    ) noexcept
    : _m_mode(_a_mode)
{}

template <typename T>
__constexpr_imp std::size_t
                data_generator_t<T>::mode() const noexcept
{
    return _m_mode;
}

__no_constexpr_imp errors::test_library_exception_t
                   unsupported_mode_exception(
                       const std::type_info&                     _a_type_info,
                       const std::size_t                         _a_mode,
                       const std::initializer_list<std::size_t>& _a_supported_elements
                   )
{
    using namespace errors;
    return test_library_exception_t(fmt::format(
        "{0} does not support mode {1}. {0} only supports the following modes: "
        "{2}",
        _a_type_info.name(),
        _a_mode,
        _a_supported_elements
    ));
}

_END_ABC_DG_NS
#endif