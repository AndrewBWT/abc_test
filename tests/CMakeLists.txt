# Set the name of the test lib once.
set(TEST_LIB_NAME testlib)
# Fetch catch2, as it is currently how we test the library.
FetchContent_Declare(
    catch
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0)
  FetchContent_MakeAvailable(catch)
  
# A Catch2 option. It adds StringMaker specialisations for pairs, tuples, variants and optional
add_compile_definitions(CATCH_CONFIG_ENABLE_ALL_STRINGMAKERS)

# The set of all test files. The idea behind these is they mirror the files in the library.
# However we may change this as we move forward.

#There are some additional entries at the end which are not mirrored by files in the library.
set(TEST_FILES
  # include/abc_test_test/internal/test_assertions.h
  # include/abc_test_test/internal/test_main.h
  # include/abc_test_test/internal/test_options.h
  # include/abc_test_test/internal/test_order_enum.h
  # include/abc_test_test/internal/test_runner.h
  # include/abc_test_test/internal/log_test_msg.h
  # include/abc_test_test/internal/ds/repetitions/repetition_data.h
  # include/abc_test_test/internal/ds/repetitions/repetition_tree.h
  # include/abc_test_test/internal/ds/repetitions/repetition_tree_for_loop_iteration_data.h
  # include/abc_test_test/internal/ds/repetitions/repetition_tree_for_loop_index_data.h
  # include/abc_test_test/internal/ds/repetitions/for_loop_data.h
  # include/abc_test_test/internal/ds/repetitions/for_loop_data_collection.h
  # include/abc_test_test/internal/ds/test_data/invoked_test_data.h
  # include/abc_test_test/internal/ds/test_data/registered_test_data.h
  # include/abc_test_test/internal/ds/test_data/post_setup_test_data.h
  # include/abc_test_test/internal/ds/test_collections/test_list.h
  # include/abc_test_test/internal/ds/test_collections/test_collection.h
  # include/abc_test_test/internal/ds/test_collections/finalised_test_collection.h
  # include/abc_test_test/internal/ds/test_collections/test_tree.h
  # include/abc_test_test/internal/ds/test_collections/test_tree_iterator.h
  # include/abc_test_test/internal/errors/test_library_exception.h
  # include/abc_test_test/internal/errors/test_library_setup_error.h
  # include/abc_test_test/internal/errors/test_failure_info.h
  # include/abc_test_test/internal/errors/test_failures_info.h
  # include/abc_test_test/internal/errors/test_failure_info_enum.h
  # include/abc_test_test/internal/errors/test_assertion_exception.h
  # include/abc_test_test/internal/reporters/error_reporter.h
  # include/abc_test_test/internal/reporters/error_reporter_controller.h
  # include/abc_test_test/internal/reporters/text_error_reporter.h
  # include/abc_test_test/internal/reporters/test_reporter.h
  # include/abc_test_test/internal/reporters/text_test_reporter.h
  # include/abc_test_test/internal/reporters/test_reporter_controller.h
  # include/abc_test_test/matchers/matcher.h
  # include/abc_test_test/matchers/generic_matcher.h
  # include/abc_test_test/matchers/operator_based_matcher.h
  # include/abc_test_test/matchers/comparison.h
  # include/abc_test_test/matchers/comparison/comparison_matcher.h
  # include/abc_test_test/matchers/comparison/comparison_enum.h
  # include/abc_test_test/matchers/comparison/constructors.h
  # include/abc_test_test/matchers/comparison/operators.h
  # include/abc_test_test/matchers/comparison/placeholder.h
  # include/abc_test_test/matchers/comparison/wrapper.h
  # include/abc_test_test/gen_data/gen_data_base.h
  # include/abc_test_test/gen_data/manual_for_loop.h
  # include/abc_test_test/gen_data/gen_data_with_file_writer.h
  # include/abc_test_test/gen_data/file.h
  # include/abc_test_test/gen_data/collection.h
  # include/abc_test_test/gen_data/collection_iterator.h
  # include/abc_test_test/gen_data/random_data.h
  # include/abc_test_test/gen_data/random_data_description.h
  # include/abc_test_test/gen_data/finite_enumeration.h
  # include/abc_test_test/gen_data/random/base.h
  # include/abc_test_test/gen_data/random/specialization_base.h
  # include/abc_test_test/gen_data/random/using_functions.h
  # include/abc_test_test/gen_data/random/specializations.h
  # include/abc_test_test/gen_data/random/specializations/fundamental_types.h
  # include/abc_test_test/gen_data/random/specializations/enum.h
  # include/abc_test_test/gen_data/random/specializations/array.h
  # include/abc_test_test/gen_data/random/specializations/vector.h
  # include/abc_test_test/gen_data/static_data.h
  # include/abc_test_test/gen_data/enumeration.h
  # include/abc_test_test/gen_data/enumerable.h
  # include/abc_test_test/gen_data/enumerable/min_value.h
  # include/abc_test_test/gen_data/enumerable/max_value.h
  # include/abc_test_test/gen_data/enumerable/specializations.h
  # include/abc_test_test/gen_data/enumerable/specializations/enum.h
  # include/abc_test_test/gen_data/enumerable/specializations/array.h
  # include/abc_test_test/gen_data/enumerable/specializations/vector.h
  # include/abc_test_test/gen_data/combinatorics.h
  # include/abc_test_test/utility/ptr_utility.h
  # include/abc_test_test/utility/rng.h
  # include/abc_test_test/utility/bigint/NumberlikeArray.h
  # include/abc_test_test/utility/combinatorics.h
  # include/abc_test_test/utility/combinatorics/iterator.h
  # include/abc_test_test/utility/combinatorics/permutations.h
  # include/abc_test_test/utility/combinatorics/combinations.h
  # include/abc_test_test/utility/internal/log/enum.h
  # include/abc_test_test/utility/internal/log/params.h
  # include/abc_test_test/utility/internal/log.h
  # include/abc_test_test/utility/internal/macros.h
  # include/abc_test_test/utility/io/threaded_text_output_reporter.h
  # include/abc_test_test/utility/io/threaded_ostream_output_reporter.h
  # include/abc_test_test/utility/io/path_utility.h
  # include/abc_test_test/utility/io/file/file_name.h
  # include/abc_test_test/utility/io/file/file_name_with_extension.h
  # include/abc_test_test/utility/io/file/file_reader.h
  # include/abc_test_test/utility/io/file/file_writer.h
  # include/abc_test_test/utility/io/file/file_rw.h
  # include/abc_test_test/utility/io/file/file_rw_info.h
  # include/abc_test_test/utility/str/parser.h
  # include/abc_test_test/utility/str/printer.h
  # include/abc_test_test/utility/str/parser_utility.h
  # include/abc_test_test/utility/str/string_utility.h
  # include/abc_test_test/utility/str.h
  # include/abc_test_test/utility/str/rw_info.h
  # include/abc_test_test/global.h
  # # Utility files
  # include/abc_test_test/test_utilities/pc_test_reporter.h
  # include/abc_test_test/test_utilities/register_test.h
  # include/abc_test_test/test_utilities/pc_setup_tests_reporter.h
  # include/abc_test_test/test_utilities/test_info.h
  # include/abc_test_test/test_utilities/general_includes.h
  # include/abc_test_test/test_utilities/example_matcher.h
  # include/abc_test_test/test_utilities/placeholder_combinatorics.h
  # include/abc_test_test/test_utilities/basic_example_test_script.h
      src/main.cpp
)

# These are the libs required for the testing suite.
set(REQUIRED_LIBS
  Catch2::Catch2
  abc_test
)

# Add the set of files to executable.
add_executable(${TEST_LIB_NAME} ${TEST_FILES})
  
# Add the include directory to the target.
target_include_directories(${TEST_LIB_NAME} PUBLIC include)
# Link the REQUIRED_LIBS to the LIBRARY
target_link_libraries(${TEST_LIB_NAME} PUBLIC ${REQUIRED_LIBS})
# Ensure that REQUIRED_LIBS are built before the executable.
add_dependencies(${TEST_LIB_NAME} ${REQUIRED_LIBS})

# Adds the test as a command
add_test(NAME ${TEST_LIB_NAME} COMMAND ${TEST_LIB_NAME})

# Ensure that the IDE sees the files in the file structure they are organised in.
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/.." FILES ${TEST_FILES})
# Unset variable defined in this file.
unset(LIBRARY_NAME)
unset(REQUIRED_LIBS)
unset(LIBRARY_FILES)