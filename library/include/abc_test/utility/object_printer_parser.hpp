#pragma once

#include "abc_test/utility/internal/macros.hpp"

#include <optional>
#include <string>

_BEGIN_ABC_UTILITY_NS

struct object_printer_parser_t
{
    char8_t begin_char                             = u8'{';
    char8_t end_char                               = u8'}';
    char8_t delimiter_between_field_name_and_field = u8'=';
    char8_t delimiter_between_fields               = u8',';
    bool space_after_object_name = true;
    bool space_before_field_name_and_field_separator = true;
    bool space_after_field_name_and_field_separator = true;
    bool space_after_field_delimieter = true;
    bool space_before_field_delimiter = false;
};

enum enum_pointer_print_parse_type_t
{
    JUST_DATA,
    AS_OBJECT
};

enum variant_print_parse_e
{
    no_indexes,
    use_indexes
};

_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_NS

_END_ABC_UTILITY_NS