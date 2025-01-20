#pragma once
#include "abc_test/internal/utility/parsers/default_parser.hpp"
#include "abc_test/internal/utility/str/printer.hpp"

_BEGIN_ABC_UTILITY_STR_NS

template <typename T>
class rw_info_t
{
public:
    __constexpr
    rw_info_t(
        const str::parser_t<T>&  _a_parser  = str::default_parser<T>(),
        const str::printer_t<T>& _a_printer = str::printer_t<T>()
    ) noexcept;
    __constexpr
    rw_info_t(str::parser_t<T>& _a_parser) noexcept;
    __constexpr
                      rw_info_t(const str::printer_t<T>& _a_printer) noexcept;
    __constexpr const str::parser_t<T>&
                      parser() const noexcept;
    __constexpr const str::printer_t<T>&
                      printer() const noexcept;
private:
    str::parser_t<T>  _m_parser;
    str::printer_t<T> _m_printer;
};

_END_ABC_UTILITY_STR_NS

_BEGIN_ABC_UTILITY_STR_NS
template <typename T>
__constexpr_imp
    rw_info_t<T>::rw_info_t(
        const str::parser_t<T>&  _a_parser,
        const str::printer_t<T>& _a_printer
    ) noexcept
    : _m_parser(_a_parser), _m_printer(_a_printer)
{}

template <typename T>
__constexpr_imp
    rw_info_t<T>::rw_info_t(
        str::parser_t<T>& _a_parser
    ) noexcept
    : rw_info_t<T>(_a_parser, str::printer_t<T>())
{}

template <typename T>
__constexpr_imp
    rw_info_t<T>::rw_info_t(
        const str::printer_t<T>& _a_printer
    ) noexcept
    : rw_info_t<T>(str::default_parser_t<T>(), _a_printer)
{}

template <typename T>
__constexpr_imp const str::parser_t<T>&
                      rw_info_t<T>::parser() const noexcept
{
    return _m_parser;
}

template <typename T>
__constexpr_imp const str::printer_t<T>&
                      rw_info_t<T>::printer() const noexcept
{
    return _m_printer;
}

_END_ABC_UTILITY_STR_NS