#pragma once
#include "abc_test/internal/utility/internal/macros.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_print_data.hpp"
_BEGIN_ABC_REPORTERS_NS
enum class enum_pre_test_set_data_fields_t
{
	USE_GLOBAL_TEST_LIST,
    WRITE_DATA_TO_FILES,
    PATH_DELIMITER,
    ROOT_PATH,
    THREADS,
    COMMENT_STR,
    GENERAL_DATA_FILE_EXTENSION,
    N_INTEGERS_USED_FOR_CREATION_OF_RNG,
    REPETITION_CONFIG_USED,
    GLOBAL_SEED,
    FORCE_RUN_ALL_TESTS,
    TEST_PATHS_TO_RUN
};
__constexpr
enum_print_pair_collection_t< enum_pre_test_set_data_fields_t>
default_pre_test_set_data_fields(
) noexcept;
_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__constexpr_imp
enum_print_pair_collection_t< enum_pre_test_set_data_fields_t>
default_pre_test_set_data_fields(
) noexcept
{
	using enum enum_pre_test_set_data_fields_t;
	using enum reporters::enum_print_data_t;
	return
	{
		{USE_GLOBAL_TEST_LIST, PRINT},
        {WRITE_DATA_TO_FILES, PRINT},
        {PATH_DELIMITER, PRINT},
        {ROOT_PATH, PRINT},
        {THREADS, PRINT},
        {COMMENT_STR, PRINT},
        {GENERAL_DATA_FILE_EXTENSION, PRINT},
        {N_INTEGERS_USED_FOR_CREATION_OF_RNG, PRINT},
        {REPETITION_CONFIG_USED, PRINT},
        {GLOBAL_SEED, PRINT},
        {FORCE_RUN_ALL_TESTS, PRINT},
        {TEST_PATHS_TO_RUN, PRINT},
	};
}
_END_ABC_REPORTERS_NS