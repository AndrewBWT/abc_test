#pragma once
#include "abc_test/internal/utility/parsers/default_parser.hpp"
#include "abc_test/internal/utility/printers/default_printer.hpp"

_BEGIN_ABC_UTILITY_STR_NS

template <typename T>
struct rw_info_t
{
    parser::parser_t<T>   internal_parser  = parser::default_parser<T>();
    printer::printer_t<T> internal_printer = printer::default_printer<T>();
};

_END_ABC_UTILITY_STR_NS