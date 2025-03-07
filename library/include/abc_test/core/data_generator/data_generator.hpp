#pragma once
#include "abc_test/core/ds/data_generator_memoization/data_generator_memoized_element.hpp"
#include "abc_test/core/ds/data_generator_memoization/indexed_data_generator_collection_memoized_element.hpp"
#include "abc_test/core/ds/type_synonyms.hpp"
#include "abc_test/core/global.hpp"
#include "abc_test/utility/io/file/file_name.hpp"
#include "abc_test/utility/io/file/file_line_reader.hpp"
#include "abc_test/core/data_generator/data_generator_file_reader_and_writer.hpp"
#include "abc_test/utility/io/file/file_line_writer.hpp"
#include "abc_test/utility/io/file/file_line_writer_with_optional_rw_info.hpp"

#include <expected>
#include <functional>
#include <memory>
#include <optional>
#include <scn/scan.h>
#include <string>
#include <vector>

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
_END_ABC_DG_NS