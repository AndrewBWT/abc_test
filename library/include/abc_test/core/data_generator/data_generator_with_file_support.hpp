#pragma once
#include "abc_test/core/data_generator/data_generator_file_reader_and_writer.hpp"
#include "abc_test/core/data_generator/data_generator_with_file_support_concept.hpp"
#include "abc_test/utility/io/file/file_line_writer_with_optional_rw_info.hpp"

#include <type_traits>
#include <vector>

_BEGIN_ABC_DG_NS

template <typename T, bool Has_File_Read_Writer>
requires concept_for_data_generator_with_file_support<T>
class data_generator_with_file_support_t
    : public data_generator_t<typename T::generator_type>
{
public:
    __constexpr virtual ~data_generator_with_file_support_t() = default;
    __constexpr
    data_generator_with_file_support_t(const T& _a_element);
    __constexpr
    data_generator_with_file_support_t(
        const T& _a_element,
        const utility::io::file_name_t<typename T::generator_type>&
            _a_file_read_writer
    );
    __constexpr
    data_generator_with_file_support_t(
        const T& _a_element,
        const utility::io::file_name_t<typename T::generator_type>&
            _a_file_read_writer,
        const std::pair<
            typename utility::io::file_names_t<
                typename T::generator_type>::const_iterator,
            typename utility::io::file_names_t<
                typename T::generator_type>::const_iterator>& _a_pair_iterators
    );

    __constexpr virtual void
        set_data_generator_using_data_generator_memoized_element(
            const ds::dg_memoized_element_t& _a_dgme
        );
    __constexpr virtual bool
        has_current_element();
    __constexpr virtual const T::generator_type&
        current_element() const;
    __constexpr virtual bool
        generate_next();
    __constexpr virtual ds::dg_memoized_element_t
        get_data_generator_memoized_element(
            const bool _a_get_original_dg_memoized_element_data
        );
private:
    __constexpr void
        switch_mode(const std::size_t _a_new_mode);
    T   _m_object;
    std::conditional_t<
        Has_File_Read_Writer,
        data_generator_file_reader_and_writer_t<T>,
        std::monostate>
                _m_file_read_writer;
    std::size_t _m_mode{0};
    std::vector<utility::io::file_line_writer_with_optional_rw_info_t<
        typename T::generator_type>>
                                                            _m_file_writers;
    abc::utility::str::rw_info_t<typename T::tertiary_type> _m_tertiary_rw_info;
};

template <typename T, typename... Args>
requires concept_for_data_generator_with_file_support<T>
__constexpr _ABC_NS_DG::data_generator_collection_t<typename T::generator_type>
            make_data_generator_with_file_support(
                const T& _a_generator,
                Args... _a_file_reader_writers
            ) noexcept;

template <typename T>
requires concept_for_data_generator_with_file_support<T>
__constexpr _ABC_NS_DG::data_generator_collection_t<typename T::generator_type>
    make_data_generator_with_file_support(const T& _a_generator) noexcept;

namespace
{
template <typename T>
__constexpr_imp void
    process_args_internal(utility::io::file_names_t<T>& _a_fns) noexcept;
template <typename T, typename... Args>
__constexpr_imp void
    process_args_internal(
        utility::io::file_names_t<T>&      _a_fns,
        const utility::io::file_name_t<T>& _a_fn_no_type,
        Args... _a_elements
    ) noexcept;
template <typename T>
__constexpr_imp void
    process_args_internal(
        utility::io::file_names_t<T>&      _a_fns,
        const utility::io::file_name_t<T>& _a_fn_no_type
    ) noexcept;

template <typename T, typename... Args>
__constexpr utility::io::file_names_t<T>
            process_args(Args... _a_elements) noexcept;
} // namespace

_END_ABC_DG_NS

_BEGIN_ABC_DG_NS
template <typename T, bool Has_File_Read_Writer>
requires concept_for_data_generator_with_file_support<T>
__constexpr
data_generator_with_file_support_t<T, Has_File_Read_Writer>::
    data_generator_with_file_support_t(
        const T& _a_element
    )
    : _m_object(_a_element)
{}

template <typename T, bool Has_File_Read_Writer>
requires concept_for_data_generator_with_file_support<T>
__constexpr
data_generator_with_file_support_t<T, Has_File_Read_Writer>::
    data_generator_with_file_support_t(
        const T& _a_element,
        const utility::io::file_name_t<typename T::generator_type>&
            _a_file_read_writer
    )
    : _m_object(_a_element)
    , _m_file_read_writer(data_generator_file_reader_and_writer_t<T>(
          _a_element,
          _a_file_read_writer,
          generate_comment_str(
              _a_file_read_writer,
              typeid(typename T::tertiary_type)
          )
      ))
{
    if (not _m_file_read_writer.has_current_element())
    {
        _m_mode = 1;
    }
}

template <typename T, bool Has_File_Read_Writer>
requires concept_for_data_generator_with_file_support<T>
__constexpr
data_generator_with_file_support_t<T, Has_File_Read_Writer>::
    data_generator_with_file_support_t(
        const T& _a_element,
        const utility::io::file_name_t<typename T::generator_type>&
            _a_file_read_writer,
        const std::pair<
            typename utility::io::file_names_t<
                typename T::generator_type>::const_iterator,
            typename utility::io::file_names_t<
                typename T::generator_type>::const_iterator>& _a_pair_iterators
    )
    : data_generator_with_file_support_t<T, Has_File_Read_Writer>(
          _a_element,
          _a_file_read_writer
      )
{
    using namespace utility::io;
    for (typename file_names_t<typename T::generator_type>::const_iterator
             _l_itt{_a_pair_iterators.first};
         _l_itt != _a_pair_iterators.second;
         ++_l_itt)
    {
        _m_file_writers.push_back(file_line_writer_with_optional_rw_info_t<
                                  typename T::generator_type>(
            *_l_itt,
            generate_comment_str(*_l_itt, typeid(typename T::tertiary_type))
        ));
    }
}

template <typename T, bool Has_File_Read_Writer>
requires concept_for_data_generator_with_file_support<T>
__constexpr void
    data_generator_with_file_support_t<T, Has_File_Read_Writer>::
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
                abc::utility::parser::parse_with_exception<std::size_t>(
                    _a_dgme.additional_data
                )
            );
            break;
        case 1:
            _m_object.set_generator_using_tertiary_data(
                abc::utility::parser::parse_with_exception(
                    _a_dgme.additional_data, _m_tertiary_rw_info.internal_parser
                )
            );
            break;
        default:
            throw std::runtime_error("Couldn't work");
        }
    }
    else
    {
        switch (_a_dgme.mode)
        {
        case 0:
            _m_object.set_generator_using_tertiary_data(
                abc::utility::parser::parse_with_exception(
                    _a_dgme.additional_data, _m_tertiary_rw_info.internal_parser
                )
            );
            break;
        default:
            throw std::runtime_error("Couldn't work");
        }
    }
}

template <typename T, bool Has_File_Read_Writer>
requires concept_for_data_generator_with_file_support<T>
__constexpr bool
    data_generator_with_file_support_t<T, Has_File_Read_Writer>::
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
            throw std::runtime_error("Couldn't work");
        }
    }
    else
    {
        switch (_m_mode)
        {
        case 0:
            return _m_object.has_current_element();
        default:
            throw std::runtime_error("Couldn't work");
        }
    }
}

template <typename T, bool Has_File_Read_Writer>
requires concept_for_data_generator_with_file_support<T>
__constexpr const T::generator_type&
                  data_generator_with_file_support_t<T, Has_File_Read_Writer>::
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
            throw std::runtime_error("Couldn't work");
        }
    }
    else
    {
        switch (_m_mode)
        {
        case 0:
            return _m_object.current_element();
        default:
            throw std::runtime_error("Couldn't work");
        }
    }
}

template <typename T, bool Has_File_Read_Writer>
requires concept_for_data_generator_with_file_support<T>
__constexpr bool
    data_generator_with_file_support_t<T, Has_File_Read_Writer>::generate_next()
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
            throw std::runtime_error("Couldn't work");
        }
    }
    else
    {
        switch (_m_mode)
        {
        case 0:
            return _m_object.generate_next();
        default:
            throw std::runtime_error("Couldn't work");
        }
    }
}

template <typename T, bool Has_File_Read_Writer>
requires concept_for_data_generator_with_file_support<T>
__constexpr ds::dg_memoized_element_t
            data_generator_with_file_support_t<T, Has_File_Read_Writer>::
        get_data_generator_memoized_element(
            const bool _a_get_original_dg_memoized_element_data
        )
{
    using namespace _ABC_NS_DS;
    using namespace std;
    using namespace abc::utility::io;
    if constexpr (Has_File_Read_Writer)
    {
        if (_a_get_original_dg_memoized_element_data)
        {
            u8string _l_rv;
            switch (_m_mode)
            {
            case 0:
                _l_rv = utility::printer::print<size_t>(
                    _m_file_read_writer.get_file_position()
                );
                break;
            case 1:
                _l_rv = _m_tertiary_rw_info.internal_printer->run_printer(
                    _m_object.tertiary_data()
                );
                break;
            default:
                throw std::runtime_error("Couldn't work");
            }
            return dg_memoized_element_t{_m_mode, _l_rv};
        }
        else
        {
            u8string _l_rv;
            size_t   _l_mode = _m_mode;
            switch (_m_mode)
            {
            case 0:
                _l_rv = utility::printer::print<size_t>(
                    _m_file_read_writer.get_file_position()
                );
                break;
            case 1:
                if (global::get_this_threads_test_options().write_data_to_files)
                {
                    _l_mode = 0;
                    _l_rv   = utility::printer::print<size_t>(
                        _m_file_read_writer.write_data_to_file(_m_object)
                    );
                }
                else
                {
                    _l_rv = _m_tertiary_rw_info.internal_printer->run_printer(
                        _m_object.tertiary_data()
                    );
                }
                // file_writer_t<generator_type>
                //_m_object. T.
                // File writiers take a T. Turn it into a string?
                for (file_line_writer_with_optional_rw_info_t<
                         typename T::generator_type>& _l_writer :
                     _m_file_writers)
                {
                    if (_l_writer.has_printer_parser())
                    {
                        _l_writer.write_line_using_rw_info(
                            _m_object.current_element()
                        );
                    }
                    else
                    {
                        _l_writer.write_line(
                            _m_tertiary_rw_info.internal_printer->run_printer(
                                _m_object.tertiary_data()
                            )
                        );
                    }
                }
                break;
            default:
                throw std::runtime_error("Couldn't work");
            }
            return dg_memoized_element_t{_l_mode, _l_rv};
        }
    }
    else
    {
        return dg_memoized_element_t{
            _m_mode,
            _m_tertiary_rw_info.internal_printer->run_printer(
                _m_object.tertiary_data()
            )
        };
    }
}

template <typename T, bool Has_File_Read_Writer>
requires concept_for_data_generator_with_file_support<T>
__constexpr void
    data_generator_with_file_support_t<T, Has_File_Read_Writer>::switch_mode(
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
                    throw std::runtime_error("couldn't");
                }
                _m_mode = _a_new_mode;
            }
        }
        else
        {
            throw std::runtime_error("coudln't");
        }
    }
}

template <typename T, typename... Args>
requires concept_for_data_generator_with_file_support<T>
__constexpr_imp
    _ABC_NS_DG::data_generator_collection_t<typename T::generator_type>
    make_data_generator_with_file_support(
        const T& _a_generator,
        Args... _a_file_reader_writers
    ) noexcept
{
    using namespace _ABC_NS_DG;
    using namespace std;
    using namespace utility::io;
    const file_names_t<typename T::generator_type>& _l_file_reader_writers{
        process_args<typename T::generator_type>(_a_file_reader_writers...)
    };
    if (_l_file_reader_writers.size() == 0)
    {
        return unary_collection<typename T::generator_type>(
            make_shared<data_generator_with_file_support_t<T, false>>(
                _a_generator
            )
        );
    }
    else if (_l_file_reader_writers.size() == 1)
    {
        return unary_collection<typename T::generator_type>(
            make_shared<data_generator_with_file_support_t<T, true>>(
                _a_generator, _l_file_reader_writers[0]
            )
        );
    }
    else
    {
        return unary_collection<typename T::generator_type>(
            make_shared<data_generator_with_file_support_t<T, true>>(
                _a_generator,
                _l_file_reader_writers[0],
                pair<
                    typename file_names_t<
                        typename T::generator_type>::const_iterator,
                    typename file_names_t<
                        typename T::generator_type>::const_iterator>(
                    _l_file_reader_writers.begin() + 1,
                    _l_file_reader_writers.end()
                )
            )
        );
    }
}

template <typename T>
requires concept_for_data_generator_with_file_support<T>
__constexpr _ABC_NS_DG::data_generator_collection_t<typename T::generator_type>
            make_data_generator_with_file_support(
                const T& _a_generator
            ) noexcept
{
    using namespace _ABC_NS_DG;
    using namespace std;
    using namespace utility::io;
    return unary_collection<typename T::generator_type>(
        make_shared<data_generator_with_file_support_t<T, false>>(_a_generator)
    );
}

namespace
{
template <typename T>
__constexpr_imp void
    process_args_internal(
        utility::io::file_names_t<T>& _a_fns
    ) noexcept
{}

template <typename T, typename... Args>
__constexpr_imp void
    process_args_internal(
        utility::io::file_names_t<T>&      _a_fns,
        const utility::io::file_name_t<T>& _a_fn_no_type,
        Args... _a_elements
    ) noexcept
{
    _a_fns.push_back(_a_fn_no_type);
    process_args_internal<T>(_a_fns, _a_elements...);
}

template <typename T>
__constexpr_imp void
    process_args_internal(
        utility::io::file_names_t<T>&      _a_fns,
        const utility::io::file_name_t<T>& _a_fn_no_type
    ) noexcept
{
    _a_fns.push_back(_a_fn_no_type);
}

template <typename T, typename... Args>
__constexpr_imp utility::io::file_names_t<T>
                process_args(
                    Args... _a_elements
                ) noexcept
{
    utility::io::file_names_t<T> _l_rv{};
    process_args_internal<T>(_l_rv, _a_elements...);
    return _l_rv;
}
} // namespace

_END_ABC_DG_NS
