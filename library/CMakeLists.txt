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
  include/abc_test/internal/global.hpp
      src/abc_test/internal/global.cpp
  # The core subfolder; contains the core logic for how the library works.
  include/abc_test/core.hpp
  include/abc_test/included_instances.hpp
  include/abc_test/internal/test_main.hpp
  include/abc_test/internal/options/test_options_base.hpp
  include/abc_test/internal/options/validated_test_options.hpp
  include/abc_test/internal/test_runner.hpp
      src/abc_test/internal/test_runner.cpp
  # Logging messages, associated with tests
  include/abc_test/internal/logging/macros.hpp 
  include/abc_test/internal/logging/log_msg.hpp
  include/abc_test/internal/test_assertions/test_block.hpp
  include/abc_test/internal/test_assertions/macros.hpp
  include/abc_test/internal/test_assertions/functions.hpp
  # Subfolder contains objects and functions pertaining to how repetition data is stored.
  include/abc_test/internal/ds/data_generator_memoization/indexed_data_generator_collection_memoized_element.hpp
  include/abc_test/internal/ds/data_generator_memoization/typeless_data_generator_collection_stack_trie.hpp
  include/abc_test/internal/ds/data_generator_memoization/typeless_data_generator_collection_stack.hpp
  include/abc_test/internal/ds/data_generator_memoization/data_generator_collection_memoized_element.hpp 
  include/abc_test/internal/ds/data_generator_memoization/data_generator_memoized_element.hpp
  include/abc_test/internal/ds/data_generator_memoization/map_unique_id_to_tdg_collection_stack_trie.hpp
  # Contains code for test data - e.g. name, location, repetition data.
  include/abc_test/internal/ds/test_data/invoked_test_data.hpp
  include/abc_test/internal/ds/test_data/registered_test_data.hpp
  include/abc_test/internal/ds/test_data/post_setup_test_data.hpp
  include/abc_test/internal/ds/test_data/user_defined_test_data.hpp
  include/abc_test/internal/ds/test_data/checked_user_defined_test_data.hpp
  include/abc_test/internal/ds/test_data/test_function_object.hpp
  include/abc_test/internal/ds/test_data/test_function_callable_object.hpp
  include/abc_test/internal/ds/test_data/enum_test_status.hpp
  include/abc_test/internal/ds/test_data/macros.hpp
  include/abc_test/internal/ds/test_data/test_set_data.hpp
      src/abc_test/internal/ds/test_data/test_set_data.cpp
  include/abc_test/internal/ds/test_data/finalised_test_set_data.hpp
  # Bespoke data structures for the library for storing tests in various forms.
  include/abc_test/internal/ds/test_collections/test_collection.hpp
  include/abc_test/internal/ds/test_collections/test_tree.hpp
      src/abc_test/internal/ds/test_collections/test_tree.cpp
  include/abc_test/internal/ds/test_collections/test_tree_sentinel.hpp
  include/abc_test/internal/ds/test_collections/test_tree_iterator.hpp
  include/abc_test/internal/ds/source/single_source.hpp
  include/abc_test/internal/ds/source/source_pair.hpp
  include/abc_test/internal/ds/type_synonyms.hpp
  # Contains objects and functions pertaining to recoverable and unrecoverable errors,
  # as well as errors encountered when evaluating tests.
  include/abc_test/internal/errors/test_library_exception.hpp
  include/abc_test/internal/errors/test_library_setup_error.hpp
  include/abc_test/internal/errors/test_assertion_exception.hpp
  # Contains code for reporting errors and test results to the user.
  include/abc_test/internal/reporters/error_reporter.hpp
  include/abc_test/internal/reporters/error_reporter_controller.hpp
  include/abc_test/internal/reporters/test_reporter.hpp
  include/abc_test/internal/reporters/test_reporter_controller.hpp
  # Contains data structures representing things reported from a test. e.g. assertions, unexpected exceptions
  include/abc_test/internal/test_reports/user_initialised_report.hpp
  include/abc_test/internal/test_reports/assertion.hpp
  include/abc_test/internal/test_reports/assertion_base.hpp
  include/abc_test/internal/test_reports/matcher_based_assertion_single_line.hpp
  include/abc_test/internal/test_reports/multi_element_assertion_block.hpp
  include/abc_test/internal/test_reports/static_assertion.hpp
  include/abc_test/internal/test_reports/unexpected_report.hpp
  include/abc_test/internal/test_reports/basic_text_warning.hpp
  include/abc_test/internal/test_reports/unexpected_thrown_non_descript_entity.hpp
  include/abc_test/internal/test_reports/unexpected_thrown_exception.hpp
  include/abc_test/internal/test_reports/assertion_status/assertion_status_base.hpp
  include/abc_test/internal/test_reports/assertion_status/dynamic_status.hpp
  include/abc_test/internal/test_reports/assertion_status/static_status.hpp
  include/abc_test/internal/test_reports/assertion_status/pass.hpp
  include/abc_test/internal/test_reports/assertion_status/fail.hpp
  include/abc_test/internal/test_reports/assertion_status/terminate.hpp
  include/abc_test/internal/test_reports/assertion_status/pass_or_fail.hpp
  include/abc_test/internal/test_reports/assertion_status/pass_or_terminate.hpp
  include/abc_test/internal/test_reports/matcher_based_assertion_block.hpp
  # The core logic for the data generators. There are no objects here, only abstract objects and iterators.
  include/abc_test/internal/data_generator/data_generator.hpp
  include/abc_test/internal/data_generator/data_generator_with_file_support_concept.hpp
  include/abc_test/internal/data_generator/data_generator_with_file_support.hpp
  include/abc_test/internal/data_generator/data_generator_collection.hpp
  include/abc_test/internal/data_generator/data_generator_collection_iterator.hpp
  include/abc_test/internal/data_generator/typeless_data_generator_collection_iterator.hpp
  include/abc_test/internal/data_generator/data_generator_collection_sentinel.hpp
  include/abc_test/internal/data_generator/data_generator_file_reader_and_writer.hpp
  # Core matcher files
  include/abc_test/internal/matchers/matcher_wrapper.hpp
  include/abc_test/internal/matchers/simulated_logic_expr.hpp
  include/abc_test/internal/matchers/bool_matcher.hpp
  include/abc_test/internal/matchers/static_matcher.hpp
      src/abc_test/internal/matchers/static_matcher.cpp
  include/abc_test/internal/matchers/comparison/macros.hpp
  include/abc_test/internal/matchers/matcher_result.hpp
  include/abc_test/internal/matchers/comparison/comparison_functions.hpp
  include/abc_test/internal/matchers/comparison/comparison_enum.hpp
  include/abc_test/internal/matchers/comparison/operators.hpp
  include/abc_test/internal/matchers/comparison/comparison_placeholder.hpp
  include/abc_test/internal/matchers/comparison/comparison_wrapper.hpp
  include/abc_test/internal/matchers/logic_enum.hpp
  include/abc_test/internal/matchers/source_map.hpp
      src/abc_test/internal/matchers/source_map.cpp
  # Matcher subfolder. Code contains logic for writing individual assertions.
  include/abc_test/included_instances/matchers/ranges.hpp
  # gen_data subfolder. Contains the logic that allow the gen_data for loops to work.
  include/abc_test/included_instances/data_generator/manual_data_generator.hpp
  include/abc_test/included_instances/data_generator/file_data_generator.hpp
  include/abc_test/included_instances/data_generator/random_data_generator.hpp
  include/abc_test/included_instances/data_generator/combinatorics_data_generator.hpp
  include/abc_test/included_instances/data_generator/combinatorics_data_generator/permutations_generator.hpp
  # Subfolder contains logic specifically to do with random data generation.
  include/abc_test/included_instances/data_generator/random_generator/random_generator_base.hpp
  include/abc_test/included_instances/data_generator/random_generator/default_random_generator.hpp
  include/abc_test/included_instances/data_generator/random_generator/random_generator_function.hpp
  
  include/abc_test/included_instances/data_generator/random_generator/specializations.hpp
  include/abc_test/included_instances/data_generator/random_generator/enumeration_based_random_generator_object.hpp
  # Folder containing random data specializations for std types.
  include/abc_test/included_instances/data_generator/random_generator/specializations/stl_11.hpp
  include/abc_test/included_instances/data_generator/random_generator/specializations/stl_14.hpp
  include/abc_test/included_instances/data_generator/random_generator/specializations/stl_17.hpp
  include/abc_test/included_instances/data_generator/random_generator/specializations/stl_98.hpp
  include/abc_test/included_instances/data_generator/random_generator/specializations/fundamental_types.hpp
  # Static data files
  include/abc_test/included_instances/data_generator/static_data_generator.hpp
  # Enumeration files
  include/abc_test/included_instances/data_generator/enumeration_data_generator.hpp
  include/abc_test/included_instances/data_generator/enumeration/enumeration_base.hpp
  include/abc_test/included_instances/data_generator/enumeration/enumeration_from_functions.hpp
  include/abc_test/included_instances/data_generator/enumeration/enumeration_schema_base.hpp
  include/abc_test/included_instances/data_generator/enumeration/enumeration_schema/from_m_to_n.hpp
  include/abc_test/included_instances/data_generator/enumeration/enumeration_schema/from_m_move_k_elements.hpp
  include/abc_test/included_instances/data_generator/enumeration/enumeration_data_object.hpp
  include/abc_test/included_instances/data_generator/enumeration/specializations.hpp
  # Folder containing enumeration data specializations for std types.
  include/abc_test/included_instances/data_generator/enumeration/specializations/fundamental_types.hpp
  include/abc_test/included_instances/data_generator/enumeration/specializations/enum.hpp
  include/abc_test/included_instances/data_generator/enumeration/specializations/array.hpp
  include/abc_test/included_instances/data_generator/enumeration/specializations/vector.hpp
  # utility subfolder. Contains axillary helper classes and functions not necessary
  # related to the core testing framework.
  include/abc_test/internal/utility/ptr_utility.hpp
  include/abc_test/internal/utility/do_not_optimise.hpp
  include/abc_test/internal/utility/rng.hpp
  # BigInteger files. Currently we do not use these. We should determine whether we will
  # need these and, if not, remove them from the library.
  # Combinatorics files. Currently these are unused, however we plan to re-engineer them
  # and make them compatible with gen_data.
  include/abc_test/internal/utility/combinatorics.hpp
  # Internal subfolder. Contains functions used for either internal debugging or
  # internal macros. These shouldn't really be used by the user, used for development.
  include/abc_test/internal/utility/internal/log/enum.hpp
  include/abc_test/internal/utility/internal/log/params.hpp
  include/abc_test/internal/utility/internal/log.hpp
  include/abc_test/internal/utility/internal/macros.hpp
  # Utility io functions and objects.
  include/abc_test/internal/utility/io/threaded_text_output_reporter.hpp
  include/abc_test/internal/utility/io/threaded_ostream_output_reporter.hpp
  # Utility file functions and objects.
  include/abc_test/internal/utility/io/file/file_name.hpp
  include/abc_test/internal/utility/io/file/file_line_reader.hpp
  include/abc_test/internal/utility/io/file/file_line_writer.hpp
  include/abc_test/internal/utility/io/file/general_data_with_rw_info.hpp
  include/abc_test/internal/utility/io/file/general_data.hpp
  include/abc_test/internal/utility/io/file/tertiary_data.hpp
  include/abc_test/internal/utility/io/file/file_line_writer_with_optional_rw_info.hpp
  # Utility functions and objects for strings including parsing and printing.
  include/abc_test/internal/utility/parsers/types.hpp
  include/abc_test/internal/utility/parsers/parser_input.hpp
  include/abc_test/internal/utility/parsers/parser_exceptions.hpp
  include/abc_test/internal/utility/parsers/function_parser.hpp
  include/abc_test/internal/utility/parsers/parser_base.hpp
  include/abc_test/internal/utility/parsers/default_parser.hpp
  include/abc_test/internal/utility/parsers/specializations/fundamental_types.hpp
  include/abc_test/internal/utility/parsers/specializations/stl_98.hpp
  include/abc_test/internal/utility/parsers/specializations/stl_11.hpp
  include/abc_test/internal/utility/printers/function_printer.hpp
  include/abc_test/internal/utility/printers/printer_base.hpp
  include/abc_test/internal/utility/printers/default_printer.hpp
  include/abc_test/internal/utility/printers/specializations/fundamental_types.hpp
  include/abc_test/internal/utility/printers/specializations/stl_98.hpp
  include/abc_test/internal/utility/printers/specializations/stl_11.hpp
  include/abc_test/internal/utility/object_printer_parser.hpp
  include/abc_test/internal/utility/str/parser_utility.hpp
  include/abc_test/internal/utility/str/string_utility.hpp
  include/abc_test/internal/utility/str/string_table.hpp
  include/abc_test/internal/utility/ranges.hpp
  include/abc_test/internal/utility/str/rw_info.hpp
  include/abc_test/internal/utility/enum.hpp
  include/abc_test/internal/utility/concepts.hpp
  include/abc_test/internal/utility/limits/min_value_object.hpp
  include/abc_test/internal/utility/limits/max_value_object.hpp
  include/abc_test/internal/utility/limits/min_value_concept.hpp
  include/abc_test/internal/utility/limits/max_value_concept.hpp
  #Included instances utility files  include/abc_test/internal/utility/limits/min_value_object.hpp
  include/abc_test/included_instances/utility/limits/array.hpp
  include/abc_test/included_instances/utility/limits/enum.hpp
  include/abc_test/included_instances/utility/limits/fundamental_types.hpp
  include/abc_test/included_instances/utility/limits/vector.hpp
  # Included instances
  # Text test reporter
  include/abc_test/included_instances/reporters/text_test_reporter.hpp
  include/abc_test/included_instances/reporters/text_test_reporter/list_formatter.hpp
  include/abc_test/included_instances/reporters/text_test_reporter/print_config.hpp
  include/abc_test/included_instances/reporters/text_test_reporter/enum_print_data.hpp
  include/abc_test/included_instances/reporters/text_test_reporter/list_formatter/user_initialised_report.hpp
  include/abc_test/included_instances/reporters/text_test_reporter/enum_fields/user_initialised_report.hpp
  include/abc_test/included_instances/reporters/text_test_reporter/list_formatter/assertion.hpp
  include/abc_test/included_instances/reporters/text_test_reporter/enum_fields/assertion.hpp
  include/abc_test/included_instances/reporters/text_test_reporter/list_formatter/matcher_based_assertion_single_line.hpp
  include/abc_test/included_instances/reporters/text_test_reporter/enum_fields/matcher_based_assertion_single_line.hpp
  include/abc_test/included_instances/reporters/text_test_reporter/list_formatter/multi_element_assertion_block.hpp
  include/abc_test/included_instances/reporters/text_test_reporter/enum_fields/multi_element_assertion_block.hpp
  include/abc_test/included_instances/reporters/text_test_reporter/list_formatter/matcher_based_assertion_block.hpp
  include/abc_test/included_instances/reporters/text_test_reporter/enum_fields/assertion_block_matcher_data.hpp
  include/abc_test/included_instances/reporters/text_test_reporter/list_formatter/assertion_block_matcher_data.hpp
  include/abc_test/included_instances/reporters/text_test_reporter/enum_fields/matcher_based_assertion_block.hpp
  include/abc_test/included_instances/reporters/text_test_reporter/list_formatter/static_assertion.hpp
  include/abc_test/included_instances/reporters/text_test_reporter/enum_fields/static_assertion.hpp
  include/abc_test/included_instances/reporters/text_test_reporter/list_formatter/after_execution_test_report.hpp
  include/abc_test/included_instances/reporters/text_test_reporter/enum_fields/after_execution_test_report.hpp
  include/abc_test/included_instances/reporters/text_test_reporter/list_formatter/finalised_test_set_data_report.hpp
  include/abc_test/included_instances/reporters/text_test_reporter/enum_fields/finalised_test_set_data_report.hpp
  include/abc_test/included_instances/reporters/text_test_reporter/list_formatter/unexpected_report.hpp
  include/abc_test/included_instances/reporters/text_test_reporter/enum_fields/unexpected_report.hpp
  include/abc_test/included_instances/reporters/text_test_reporter/list_formatter/unexpected_thrown_non_descript_entity.hpp
  include/abc_test/included_instances/reporters/text_test_reporter/enum_fields/unexpected_thrown_non_descript_entity.hpp
  include/abc_test/included_instances/reporters/text_test_reporter/list_formatter/unexpected_thrown_exception.hpp
  include/abc_test/included_instances/reporters/text_test_reporter/enum_fields/unexpected_thrown_exception.hpp
  # Error reporter
  include/abc_test/included_instances/reporters/text_error_reporter.hpp
  include/abc_test/included_instances/test_assertions/macros.hpp
  # Command line parsing
  include/abc_test/included_instances/options/command_line_parser.hpp
  include/abc_test/included_instances/options/included_instances_test_options.hpp
  #Externally included libraries (we include the files rather than linking to the git repo).
  include/abc_test/external/CLI11/CLI11.hpp
  include/abc_test/external/bigint/BigInteger.hpp
      src/abc_test/external/bigint/BigInteger.cpp
  include/abc_test/external/bigint/BigIntegerAlgorithms.hpp
      src/abc_test/external/bigint/BigIntegerAlgorithms.cpp
  include/abc_test/external/bigint/BigIntegerLibrary.hpp
  include/abc_test/external/bigint/BigIntegerUtils.hpp
      src/abc_test/external/bigint/BigIntegerUtils.cpp
  include/abc_test/external/bigint/BigUnsigned.hpp
      src/abc_test/external/bigint/BigUnsigned.cpp
  include/abc_test/external/bigint/BigUnsignedInABase.hpp
      src/abc_test/external/bigint/BigUnsignedInABase.cpp
  include/abc_test/external/bigint/NumberlikeArray.hpp
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
