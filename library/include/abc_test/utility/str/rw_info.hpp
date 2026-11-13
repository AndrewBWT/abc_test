#pragma once
#include <pnp/core.hpp>

_BEGIN_ABC_UTILITY_STR_NS

template <typename T>
struct rw_info_t
{
    pnp::runtime_printer_container<T> internal_printer;
    pnp::runtime_parser_container<T>  internal_parser;
};

_END_ABC_UTILITY_STR_NS