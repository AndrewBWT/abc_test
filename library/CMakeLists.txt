# Set library name so that, if we change it, we only have to change this line
set(LIBRARY_NAME abc_test)
# Set the required libs for the library.
set(REQUIRED_LIBS
  scn::scn
  fmt::fmt
)
# The set of all files included with the library.
set(LIBRARY_FILES
  # Global file
  include/abc_test/internal/global.h
      src/abc_test/internal/global.cpp
  # The core subfolder; contains the core logic for how the library works.
  include/abc_test/core.h
  include/abc_test/included_instances.h
  include/abc_test/internal/test_main.h
  include/abc_test/internal/options/test_options_base.h
  include/abc_test/internal/options/validated_test_options.h
  include/abc_test/internal/test_runner.h
      src/abc_test/internal/test_runner.cpp
  # Logging messages, associated with tests
  include/abc_test/internal/logging/macros.h 
  include/abc_test/internal/logging/log_msg.h
  include/abc_test/internal/test_assertions/test_block.h
  include/abc_test/internal/test_assertions/macros.h
  include/abc_test/internal/test_assertions/functions.h
  # Subfolder contains objects and functions pertaining to how repetition data is stored.
  include/abc_test/internal/ds/data_generator_memoization/indexed_data_generator_collection_memoized_element.h
  include/abc_test/internal/ds/data_generator_memoization/typeless_data_generator_collection_stack_trie.h
  include/abc_test/internal/ds/data_generator_memoization/typeless_data_generator_collection_stack.h
  include/abc_test/internal/ds/data_generator_memoization/data_generator_collection_memoized_element.h 
  include/abc_test/internal/ds/data_generator_memoization/data_generator_memoized_element.h
  include/abc_test/internal/ds/data_generator_memoization/map_unique_id_to_tdg_collection_stack_trie.h
  # Contains code for test data - e.g. name, location, repetition data.
  include/abc_test/internal/ds/test_data/invoked_test_data.h
  include/abc_test/internal/ds/test_data/registered_test_data.h
  include/abc_test/internal/ds/test_data/post_setup_test_data.h
  include/abc_test/internal/ds/test_data/user_defined_test_data.h
  include/abc_test/internal/ds/test_data/checked_user_defined_test_data.h
  include/abc_test/internal/ds/test_data/enum_test_status.h
  include/abc_test/internal/ds/test_data/macros.h
  include/abc_test/internal/ds/test_data/test_set_data.h
  include/abc_test/internal/ds/test_data/finalised_test_set_data.h
  # Bespoke data structures for the library for storing tests in various forms.
  include/abc_test/internal/ds/test_collections/test_collection.h
  include/abc_test/internal/ds/test_collections/test_tree.h
      src/abc_test/internal/ds/test_collections/test_tree.cpp
  include/abc_test/internal/ds/test_collections/test_tree_sentinel.h
  include/abc_test/internal/ds/test_collections/test_tree_iterator.h
  include/abc_test/internal/ds/source/single_source.h
  include/abc_test/internal/ds/source/source_pair.h
  include/abc_test/internal/ds/type_synonyms.h
  # Contains objects and functions pertaining to recoverable and unrecoverable errors,
  # as well as errors encountered when evaluating tests.
  include/abc_test/internal/errors/test_library_exception.h
  include/abc_test/internal/errors/test_library_setup_error.h
  include/abc_test/internal/errors/test_assertion_exception.h
  # Contains code for reporting errors and test results to the user.
  include/abc_test/internal/reporters/error_reporter.h
  include/abc_test/internal/reporters/error_reporter_controller.h
  include/abc_test/internal/reporters/test_reporter.h
  include/abc_test/internal/reporters/test_reporter_controller.h
  # Contains data structures representing things reported from a test. e.g. assertions, unexpected exceptions
  include/abc_test/internal/test_reports/user_initialised_report.h
  include/abc_test/internal/test_reports/assertion.h
  include/abc_test/internal/test_reports/assertion_base.h
  include/abc_test/internal/test_reports/matcher_based_assertion_single_line.h
  include/abc_test/internal/test_reports/static_assertion.h
  include/abc_test/internal/test_reports/unexpected_report.h
  include/abc_test/internal/test_reports/basic_text_warning.h
  include/abc_test/internal/test_reports/unexpected_thrown_non_descript_entity.h
  include/abc_test/internal/test_reports/unexpected_thrown_exception.h
  include/abc_test/internal/test_reports/assertion_status/assertion_status_base.h
  include/abc_test/internal/test_reports/assertion_status/dynamic_status.h
  include/abc_test/internal/test_reports/assertion_status/static_status.h
  include/abc_test/internal/test_reports/assertion_status/pass.h
  include/abc_test/internal/test_reports/assertion_status/fail.h
  include/abc_test/internal/test_reports/assertion_status/terminate.h
  include/abc_test/internal/test_reports/assertion_status/pass_or_fail.h
  include/abc_test/internal/test_reports/assertion_status/pass_or_terminate.h
  include/abc_test/internal/test_reports/matcher_based_assertion_block.h
  include/abc_test/internal/test_reports/matcher_based_assertion.h
  # The core logic for the data generators. There are no objects here, only abstract objects and iterators.
  include/abc_test/internal/data_generator/data_generator.h
  include/abc_test/internal/data_generator/data_generator_with_file_support_concept.h
  include/abc_test/internal/data_generator/data_generator_with_file_support.h
  include/abc_test/internal/data_generator/data_generator_collection.h
  include/abc_test/internal/data_generator/data_generator_collection_iterator.h
  include/abc_test/internal/data_generator/typeless_data_generator_collection_iterator.h
  include/abc_test/internal/data_generator/data_generator_collection_sentinel.h
  include/abc_test/internal/data_generator/data_generator_file_reader_and_writer.h
  # Core matcher files
  include/abc_test/internal/matchers/matcher_wrapper.h
  include/abc_test/internal/matchers/bool_matcher.h
  include/abc_test/internal/matchers/static_matcher.h
      src/abc_test/internal/matchers/static_matcher.cpp
  include/abc_test/internal/matchers/matcher_base.h
  include/abc_test/internal/matchers/operator_based_matcher.h
  include/abc_test/internal/matchers/comparison.h
  include/abc_test/internal/matchers/comparison/macros.h
  include/abc_test/internal/matchers/matcher_result.h
  include/abc_test/internal/matchers/comparison/comparison_matcher.h
  include/abc_test/internal/matchers/comparison/comparison_enum.h
  include/abc_test/internal/matchers/comparison/operators.h
  include/abc_test/internal/matchers/comparison/comparison_placeholder.h
  include/abc_test/internal/matchers/comparison/comparison_wrapper.h
  include/abc_test/internal/matchers/logic/logic_matcher.h
  include/abc_test/internal/matchers/logic/logic_enum.h
  include/abc_test/internal/matchers/source_map.h
      src/abc_test/internal/matchers/source_map.cpp
  # Matcher subfolder. Code contains logic for writing individual assertions.
  include/abc_test/included_instances/matchers/ranges.h
  include/abc_test/included_instances/matchers/function_wrapper.h
  # gen_data subfolder. Contains the logic that allow the gen_data for loops to work.
  include/abc_test/included_instances/data_generator/manual_data_generator.h
  include/abc_test/included_instances/data_generator/file_data_generator.h
  include/abc_test/included_instances/data_generator/random_data_generator.h
  # Subfolder contains logic specifically to do with random data generation.
  include/abc_test/included_instances/data_generator/random_generator_object.h
  include/abc_test/included_instances/data_generator/random_generator_object/specializations.h
  # Folder containing random data specializations for std types.
  include/abc_test/included_instances/data_generator/random_generator_object/specializations/fundamental_types.h
  include/abc_test/included_instances/data_generator/random_generator_object/specializations/enum.h
  include/abc_test/included_instances/data_generator/random_generator_object/specializations/array.h
  include/abc_test/included_instances/data_generator/random_generator_object/specializations/vector.h
  # Static data files
  include/abc_test/included_instances/data_generator/static_data_generator.h
  # Enumeration files
  include/abc_test/included_instances/data_generator/enumeration_schema.h
  include/abc_test/included_instances/data_generator/enumeration_schema/from_m_to_n.h
  include/abc_test/included_instances/data_generator/enumeration_schema/from_m_move_k_elements.h
  include/abc_test/included_instances/data_generator/enumeration_data_generator.h
  include/abc_test/included_instances/data_generator/enumeration_data_object.h
  include/abc_test/included_instances/data_generator/enumeration_data_object/min_value_object.h
  include/abc_test/included_instances/data_generator/enumeration_data_object/max_value_object.h
  include/abc_test/included_instances/data_generator/enumeration_data_object/min_value_concept.h
  include/abc_test/included_instances/data_generator/enumeration_data_object/max_value_concept.h
  include/abc_test/included_instances/data_generator/enumeration_data_object/specializations.h
  # Folder containing random data specializations for std types.
  include/abc_test/included_instances/data_generator/enumeration_data_object/specializations/fundamental_types.h
  include/abc_test/included_instances/data_generator/enumeration_data_object/specializations/enum.h
  include/abc_test/included_instances/data_generator/enumeration_data_object/specializations/array.h
  include/abc_test/included_instances/data_generator/enumeration_data_object/specializations/vector.h
  # utility subfolder. Contains axillary helper classes and functions not necessary
  # related to the core testing framework.
  include/abc_test/internal/utility/ptr_utility.h
  include/abc_test/internal/utility/rng.h
  # BigInteger files. Currently we do not use these. We should determine whether we will
  # need these and, if not, remove them from the library.
  # Combinatorics files. Currently these are unused, however we plan to re-engineer them
  # and make them compatible with gen_data.
  include/abc_test/internal/utility/combinatorics.h
  # Internal subfolder. Contains functions used for either internal debugging or
  # internal macros. These shouldn't really be used by the user, used for development.
  include/abc_test/internal/utility/internal/log/enum.h
  include/abc_test/internal/utility/internal/log/params.h
  include/abc_test/internal/utility/internal/log.h
  include/abc_test/internal/utility/internal/macros.h
  # Utility io functions and objects.
  include/abc_test/internal/utility/io/threaded_text_output_reporter.h
  include/abc_test/internal/utility/io/threaded_ostream_output_reporter.h
  # Utility file functions and objects.
  include/abc_test/internal/utility/io/file/file_name.h
  include/abc_test/internal/utility/io/file/file_line_reader.h
  include/abc_test/internal/utility/io/file/file_line_writer.h
  include/abc_test/internal/utility/io/file/general_data_with_rw_info.h
  include/abc_test/internal/utility/io/file/general_data.h
  include/abc_test/internal/utility/io/file/tertiary_data.h
  include/abc_test/internal/utility/io/file/file_line_writer_with_optional_rw_info.h
  # Utility functions and objects for strings including parsing and printing.
  include/abc_test/internal/utility/str/parser.h
  include/abc_test/internal/utility/str/printer.h
  include/abc_test/internal/utility/str/parser_utility.h
  include/abc_test/internal/utility/str/string_utility.h
  include/abc_test/internal/utility/str/string_table.h
  include/abc_test/internal/utility/str.h
  include/abc_test/internal/utility/str/rw_info.h
  # Included instances
  # Text test reporter
  include/abc_test/included_instances/reporters/text_test_reporter.h
  include/abc_test/included_instances/reporters/text_test_reporter/list_formatter.h
  include/abc_test/included_instances/reporters/text_test_reporter/print_config.h
  include/abc_test/included_instances/reporters/text_test_reporter/enum_print_data.h
  include/abc_test/included_instances/reporters/text_test_reporter/list_formatter/user_initialised_report.h
  include/abc_test/included_instances/reporters/text_test_reporter/enum_fields/user_initialised_report.h
  include/abc_test/included_instances/reporters/text_test_reporter/list_formatter/assertion.h
  include/abc_test/included_instances/reporters/text_test_reporter/enum_fields/assertion.h
  include/abc_test/included_instances/reporters/text_test_reporter/list_formatter/matcher_based_assertion_single_line.h
  include/abc_test/included_instances/reporters/text_test_reporter/enum_fields/matcher_based_assertion_single_line.h
  include/abc_test/included_instances/reporters/text_test_reporter/list_formatter/matcher_based_assertion.h
  include/abc_test/included_instances/reporters/text_test_reporter/enum_fields/matcher_based_assertion.h
  include/abc_test/included_instances/reporters/text_test_reporter/list_formatter/matcher_based_assertion_block.h
  include/abc_test/included_instances/reporters/text_test_reporter/enum_fields/matcher_based_assertion_block.h
  include/abc_test/included_instances/reporters/text_test_reporter/list_formatter/static_assertion.h
  include/abc_test/included_instances/reporters/text_test_reporter/enum_fields/static_assertion.h
  include/abc_test/included_instances/reporters/text_test_reporter/list_formatter/after_execution_test_report.h
  include/abc_test/included_instances/reporters/text_test_reporter/enum_fields/after_execution_test_report.h
  include/abc_test/included_instances/reporters/text_test_reporter/list_formatter/finalised_test_set_data_report.h
  include/abc_test/included_instances/reporters/text_test_reporter/enum_fields/finalised_test_set_data_report.h
  include/abc_test/included_instances/reporters/text_test_reporter/list_formatter/unexpected_report.h
  include/abc_test/included_instances/reporters/text_test_reporter/enum_fields/unexpected_report.h
  include/abc_test/included_instances/reporters/text_test_reporter/list_formatter/unexpected_thrown_non_descript_entity.h
  include/abc_test/included_instances/reporters/text_test_reporter/enum_fields/unexpected_thrown_non_descript_entity.h
  include/abc_test/included_instances/reporters/text_test_reporter/list_formatter/unexpected_thrown_exception.h
  include/abc_test/included_instances/reporters/text_test_reporter/enum_fields/unexpected_thrown_exception.h
  # Error reporter
  include/abc_test/included_instances/reporters/text_error_reporter.h
  include/abc_test/included_instances/test_assertions/macros.h
  # Command line parsing
  include/abc_test/included_instances/options/command_line_parser.h
  include/abc_test/included_instances/options/included_instances_test_options.h
  #Externally included libraries (we include the files rather than linking to the git repo).
  include/abc_test/external/CLI11/CLI11.hpp
  include/abc_test/external/bigint/BigInteger.h
      src/abc_test/external/bigint/BigInteger.cpp
  include/abc_test/external/bigint/BigIntegerAlgorithms.h
      src/abc_test/external/bigint/BigIntegerAlgorithms.cpp
  include/abc_test/external/bigint/BigIntegerLibrary.h
  include/abc_test/external/bigint/BigIntegerUtils.h
      src/abc_test/external/bigint/BigIntegerUtils.cpp
  include/abc_test/external/bigint/BigUnsigned.h
      src/abc_test/external/bigint/BigUnsigned.cpp
  include/abc_test/external/bigint/BigUnsignedInABase.h
      src/abc_test/external/bigint/BigUnsignedInABase.cpp
  include/abc_test/external/bigint/NumberlikeArray.h
)


# Add the set of files to the library.
add_library(${LIBRARY_NAME} ${LIBRARY_FILES})
  
# Add the include directory to the target.
target_include_directories(${LIBRARY_NAME} PUBLIC include)
# Link the REQUIRED_LIBS to the LIBRARY
target_link_libraries(${LIBRARY_NAME} PUBLIC ${REQUIRED_LIBS})
# Ensure that REQUIRED_LIBS are built before the library.
add_dependencies(${LIBRARY_NAME} ${REQUIRED_LIBS})

# Ensure that the IDE sees the files in the file structure they are organised in.
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/.." FILES ${LIBRARY_FILES})
# Unset variable defined in this file.
unset(LIBRARY_NAME)
unset(REQUIRED_LIBS)
unset(LIBRARY_FILES)
