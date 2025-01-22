#pragma once

#include "abc_test/internal/utility/internal/macros.hpp"

#include <optional>
#include <string>

_BEGIN_ABC_UTILITY_NS

struct object_printer_parser_t
{
    char begin_char                             = '{';
    char end_char                               = '}';
    char delimiter_between_field_name_and_field = '=';
    char delimiter_between_fields               = ',';
    bool space_after_object_name = true;
    bool space_before_field_name_and_field_separator = true;
    bool space_after_field_name_and_field_separator = true;
    bool space_after_field_delimieter = true;
    bool space_before_field_delimiter = false;
};

_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_NS

_END_ABC_UTILITY_NS