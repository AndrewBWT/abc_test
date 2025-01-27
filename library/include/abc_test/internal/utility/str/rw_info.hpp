#pragma once
#include "abc_test/internal/utility/parsers/default_parser.hpp"
#include "abc_test/internal/utility/printers/default_printer.hpp"

_BEGIN_ABC_UTILITY_STR_NS

template <typename T>
class rw_info_t
{
public:
    __constexpr
    rw_info_t(
        const parser::parser_t<T>&   _a_parser = parser::default_parser<T>(),
        const printer::printer_t<T>& _a_printer
        = printer::mk_printer(printer::default_printer_t<T>())
    ) noexcept;
    __constexpr
    rw_info_t(parser::parser_t<T>& _a_parser) noexcept;
    __constexpr
    rw_info_t(const printer::printer_t<T>& _a_printer) noexcept;
    __constexpr const parser::parser_t<T>&
                      parser() const noexcept;
    __constexpr const printer::printer_t<T>&
                      printer() const noexcept;
private:
    parser::parser_t<T>   _m_parser;
    printer::printer_t<T> _m_printer;
};

_END_ABC_UTILITY_STR_NS

_BEGIN_ABC_UTILITY_STR_NS
template <typename T>
__constexpr_imp
    rw_info_t<T>::rw_info_t(
        const parser::parser_t<T>&   _a_parser,
        const printer::printer_t<T>& _a_printer
    ) noexcept
    : _m_parser(_a_parser), _m_printer(_a_printer)
{}

template <typename T>
__constexpr_imp
    rw_info_t<T>::rw_info_t(
        parser::parser_t<T>& _a_parser
    ) noexcept
    : rw_info_t<T>(_a_parser, printer::mk_printer(printer::default_printer_t<T>()))
{}

template <typename T>
__constexpr_imp
    rw_info_t<T>::rw_info_t(
        const printer::printer_t<T>& _a_printer
    ) noexcept
    : rw_info_t<T>(parser::default_parser_t<T>(), _a_printer)
{}

template <typename T>
__constexpr_imp const parser::parser_t<T>&
                      rw_info_t<T>::parser() const noexcept
{
    return _m_parser;
}

template <typename T>
__constexpr_imp const printer::printer_t<T>&
                      rw_info_t<T>::printer() const noexcept
{
    return _m_printer;
}

_END_ABC_UTILITY_STR_NS