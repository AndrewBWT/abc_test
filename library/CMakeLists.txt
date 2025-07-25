# Guard against rebuilding by accident.
if(NOT DEFINED ABC_TEST_LIBRARY_INCLUDED)
  set(ABC_TEST_LIBRARY_INCLUDED TRUE)
  # Set library name so that, if we change it, we only have to change this line
  set(ABC_LIBRARY_NAME abc_test)

  # Set the required libs for the library.
  set(ABC_LIBRARY_REQUIRED_LIBS
    fmt::fmt
  )

  # Set the header files variable. This is a list of all the header files.
  set(ABC_INTERNAL_HEADERS
    # Global file
    include/abc_test/core/global.hpp
	include/abc_test/core/global/test_framework_global_variable_set.hpp
    # The core subfolder; contains the core logic for how the library works.
    include/abc_test/core.hpp
    include/abc_test/included_instances.hpp
	include/abc_test/core/test_harness.hpp
    include/abc_test/core/test_main.hpp
    include/abc_test/core/options/test_options_base.hpp
    include/abc_test/core/test_evaluator.hpp
    # Logging messages, associated with tests
    include/abc_test/core/logging/macros.hpp 
    include/abc_test/core/logging/log_msg.hpp
    include/abc_test/core/test_assertions/test_block.hpp
    include/abc_test/core/test_assertions/macro_objects.hpp
    include/abc_test/core/test_assertions/macros.hpp
    include/abc_test/core/test_assertions/functions.hpp
    # Subfolder contains objects and functions pertaining to how repetition data is stored.
    include/abc_test/core/ds/data_generator_memoization/indexed_data_generator_collection_memoized_element.hpp
    include/abc_test/core/ds/data_generator_memoization/typeless_data_generator_collection_stack_trie.hpp
    include/abc_test/core/ds/data_generator_memoization/typeless_data_generator_collection_stack.hpp
    include/abc_test/core/ds/data_generator_memoization/data_generator_collection_memoized_element.hpp 
    include/abc_test/core/ds/data_generator_memoization/data_generator_memoized_element.hpp
    include/abc_test/core/ds/data_generator_memoization/map_unique_id_to_tdg_collection_stack_trie.hpp
    # Contains code for test data - e.g. name, location, repetition data.
    include/abc_test/core/ds/test_data/invoked_test_data.hpp
    include/abc_test/core/ds/test_data/registered_test_data.hpp
    include/abc_test/core/ds/test_data/post_setup_test_data.hpp
    include/abc_test/core/ds/test_data/user_defined_test_data.hpp
    include/abc_test/core/ds/test_data/checked_user_defined_test_data.hpp
    include/abc_test/core/ds/test_data/test_function_object.hpp
    include/abc_test/core/ds/test_data/test_function_callable_object.hpp
    include/abc_test/core/ds/test_data/enum_test_status.hpp
    include/abc_test/core/ds/test_data/macros.hpp
    include/abc_test/core/ds/test_data/test_set_data.hpp
    include/abc_test/core/ds/test_data/finalised_test_set_data.hpp
    include/abc_test/core/ds/test_data/pre_test_run_report.hpp
    # Bespoke data structures for the library for storing tests in various forms.
    include/abc_test/core/ds/test_collections/test_collection.hpp
    include/abc_test/core/ds/test_collections/test_tree.hpp
    include/abc_test/core/ds/test_collections/test_tree_sentinel.hpp
    include/abc_test/core/ds/test_collections/test_tree_iterator.hpp
    include/abc_test/core/ds/source/single_source.hpp
    include/abc_test/core/ds/source/source_pair.hpp
    include/abc_test/core/ds/type_synonyms.hpp
    # Contains objects and functions pertaining to recoverable and unrecoverable errors,
    # as well as errors encountered when evaluating tests.
    include/abc_test/core/errors/test_library_exception.hpp
    include/abc_test/core/errors/test_library_setup_error.hpp
    include/abc_test/core/errors/test_assertion_exception.hpp
    # Contains code for reporting errors and test results to the user.
    include/abc_test/core/reporters/error_reporter.hpp
    include/abc_test/core/reporters/error_reporter_controller.hpp
    include/abc_test/core/reporters/test_reporter.hpp
    include/abc_test/core/reporters/test_reporter_controller.hpp
    # Contains data structures representing things reported from a test. e.g. assertions, unexpected exceptions
    include/abc_test/core/test_reports/user_initialised_report.hpp
    include/abc_test/core/test_reports/assertion.hpp
    include/abc_test/core/test_reports/assertion_base.hpp
    include/abc_test/core/test_reports/matcher_based_assertion_single_line.hpp
    include/abc_test/core/test_reports/multi_element_assertion_block.hpp
    include/abc_test/core/test_reports/static_assertion.hpp
    include/abc_test/core/test_reports/unexpected_report.hpp
    include/abc_test/core/test_reports/basic_text_warning.hpp
    include/abc_test/core/test_reports/unexpected_thrown_non_descript_entity.hpp
    include/abc_test/core/test_reports/unexpected_thrown_exception.hpp
    include/abc_test/core/test_reports/assertion_status/assertion_status_base.hpp
    include/abc_test/core/test_reports/assertion_status/dynamic_status.hpp
    include/abc_test/core/test_reports/assertion_status/static_status.hpp
    include/abc_test/core/test_reports/assertion_status/pass.hpp
    include/abc_test/core/test_reports/assertion_status/fail.hpp
    include/abc_test/core/test_reports/assertion_status/terminate.hpp
    include/abc_test/core/test_reports/assertion_status/pass_or_fail.hpp
    include/abc_test/core/test_reports/assertion_status/pass_or_terminate.hpp
    include/abc_test/core/test_reports/matcher_based_assertion_block.hpp
    # The core logic for the data generators. There are no objects here, only abstract objects and iterators.
    include/abc_test/core/data_generator/data_generator.hpp
    include/abc_test/core/data_generator/data_generator_with_file_support_concept.hpp
    include/abc_test/core/data_generator/data_generator_with_file_support.hpp
    include/abc_test/core/data_generator/data_generator_collection.hpp
    include/abc_test/core/data_generator/data_generator_collection_iterator.hpp
    include/abc_test/core/data_generator/typeless_data_generator_collection_iterator.hpp
    include/abc_test/core/data_generator/data_generator_collection_sentinel.hpp
    include/abc_test/core/data_generator/data_generator_file_reader_and_writer.hpp
    # Core matcher files
    include/abc_test/core/matchers/matcher_wrapper.hpp
    include/abc_test/core/matchers/simulated_logic_expr.hpp
    include/abc_test/core/matchers/bool_matcher.hpp
    include/abc_test/core/matchers/static_matcher.hpp
    include/abc_test/core/matchers/comparison/macros.hpp
    include/abc_test/core/matchers/matcher_result.hpp
    include/abc_test/core/matchers/comparison/comparison_functions.hpp
    include/abc_test/core/matchers/comparison/comparison_enum.hpp
    include/abc_test/core/matchers/comparison/operators.hpp
    include/abc_test/core/matchers/comparison/comparison_placeholder.hpp
    include/abc_test/core/matchers/comparison/comparison_wrapper.hpp
    include/abc_test/core/matchers/logic_enum.hpp
    include/abc_test/core/matchers/source_map.hpp
    # Matcher subfolder. Code contains logic for assertions written using the stl.
    include/abc_test/included_instances/matchers/ranges.hpp
    include/abc_test/included_instances/matchers/str.hpp
    include/abc_test/included_instances/matchers/source.hpp
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
    include/abc_test/included_instances/data_generator/random_generator/enumeration_based_random_generator_object.hpp
    # Static data files
    include/abc_test/included_instances/data_generator/static_data_generator.hpp
    # Enumeration files
    include/abc_test/included_instances/data_generator/enumeration_data_generator.hpp
    include/abc_test/included_instances/data_generator/enumeration/enumeration_base.hpp
    include/abc_test/included_instances/data_generator/enumeration/default_enumeration.hpp
    include/abc_test/included_instances/data_generator/enumeration/enumeration_from_functions.hpp
    include/abc_test/included_instances/data_generator/enumeration/enumeration_from_list.hpp
    include/abc_test/included_instances/data_generator/enumeration/enumeration_schema_base.hpp
    include/abc_test/included_instances/data_generator/enumeration/enumeration_schema/from_m_to_n_inner.hpp
    include/abc_test/included_instances/data_generator/enumeration/enumeration_schema/from_m_to_n.hpp
    include/abc_test/included_instances/data_generator/enumeration/enumeration_schema/from_m_move_k_elements.hpp
	include/abc_test/included_instances/test_harness/simple_reporter.hpp
	include/abc_test/included_instances/test_harness/simple_console_reporter.hpp
	include/abc_test/included_instances/test_harness/simple_text_reporter.hpp
    # utility subfolder. Contains axillary helper classes and functions not necessary
    # related to the core testing framework.
    include/abc_test/utility/do_not_optimise.hpp
    include/abc_test/utility/rng/inner_rng.hpp
    include/abc_test/utility/rng/inner_rng_mt19937_64.hpp
    include/abc_test/utility/rng/rng_utilities.hpp
    include/abc_test/utility/rng.hpp
    include/abc_test/utility/types.hpp
    include/abc_test/utility/type_level.hpp
    # Internal subfolder. Contains functions used for either internal debugging or
    # internal macros. These shouldn't really be used by the user, used for development.
    include/abc_test/utility/internal/log/enum.hpp
    include/abc_test/utility/internal/log/params.hpp
    include/abc_test/utility/internal/log.hpp
    include/abc_test/utility/internal/macros.hpp
    include/abc_test/utility/internal/unreachable.hpp
    # Utility io functions and objects.
    include/abc_test/utility/io/threaded_text_output_reporter.hpp
    include/abc_test/utility/io/threaded_ostream_output_reporter.hpp
    # Utility file functions and objects.
    include/abc_test/utility/io/file/file_name.hpp
    include/abc_test/utility/io/file/file_line_reader.hpp
    include/abc_test/utility/io/file/file_line_writer.hpp
    include/abc_test/utility/io/file/general_data_with_rw_info.hpp
    include/abc_test/utility/io/file/general_data.hpp
    include/abc_test/utility/io/file/tertiary_data.hpp
    include/abc_test/utility/io/file/file_line_writer_with_optional_rw_info.hpp
    include/abc_test/utility/io/file/file_based_map.hpp
    include/abc_test/utility/io/io_utilities.hpp
    # Utility functions and objects for strings including parsing and printing.
    include/abc_test/utility/parsers/parser_input.hpp
    include/abc_test/utility/parsers/parser_exceptions.hpp
    include/abc_test/utility/parsers/function_parser.hpp
    include/abc_test/utility/parsers/parser_base.hpp
    include/abc_test/utility/parsers/default_parser.hpp
    include/abc_test/utility/printers/function_printer.hpp
    include/abc_test/utility/printers/printer_base.hpp
    include/abc_test/utility/printers/default_printer.hpp
    include/abc_test/utility/object_printer_parser.hpp
    include/abc_test/utility/str/print_utils.hpp
    include/abc_test/utility/str/concepts.hpp
    include/abc_test/utility/str/unicode.hpp
    include/abc_test/utility/str/string_utils.hpp
    include/abc_test/utility/str/string_cast.hpp
    include/abc_test/utility/ranges.hpp
    include/abc_test/utility/str/rw_info.hpp
    include/abc_test/utility/enum.hpp
    include/abc_test/utility/bounds.hpp
    include/abc_test/utility/concepts.hpp
    include/abc_test/utility/limits/min_value_object.hpp
    include/abc_test/utility/limits/max_value_object.hpp
    include/abc_test/utility/limits/min_value_concept.hpp
    include/abc_test/utility/limits/max_value_concept.hpp
    # Command line parser
    include/abc_test/utility/cli.hpp
    include/abc_test/utility/cli/cli_info.hpp
    include/abc_test/utility/cli/cli_help.hpp
    include/abc_test/utility/cli/cli_help.hpp
    include/abc_test/utility/cli/cli_option_config.hpp
    include/abc_test/utility/cli/cli_output.hpp
    include/abc_test/utility/cli/cli_auto_configuration.hpp
    # Included instances utility files    include/abc_test/utility/limits/min_value_object.hpp
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
    include/abc_test/included_instances/reporters/text_test_reporter/enum_fields/pre_test_set_data_report.hpp
    include/abc_test/included_instances/reporters/text_test_reporter/list_formatter/pre_test_set_data_report.hpp
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
    include/abc_test/included_instances/options/option_config.hpp
  )

  # Set the cpp files variable. This is a list of all the cpp files.
  set(ABC_INTERNAL_SOURCES
        src/abc_test/core/global.cpp
        src/abc_test/core/test_evaluator.cpp
        src/abc_test/core/ds/test_data/invoked_test_data.cpp
        src/abc_test/core/ds/test_data/test_set_data.cpp
  	    src/abc_test/core/ds/test_collections/test_tree.cpp
        src/abc_test/core/matchers/static_matcher.cpp
        src/abc_test/core/matchers/source_map.cpp
        src/abc_test/utility/cli/cli_info.cpp
        src/abc_test/utility/cli/cli_help.cpp
        src/abc_test/utility/cli/cli_auto_configuration.cpp
        src/abc_test/included_instances/reporters/text_test_reporter/list_formatter/pre_test_set_data_report.cpp
  )

  # Set the external header files variable. This is a list of all the external header files. They are libraries not built by abc_test.
  set(ABC_EXTERNAL_HEADER_FILES
    # Global file
    #Externally included libraries (we include the files rather than linking to the git repo).
    include/abc_test/external/bigint/BigInteger.hpp
    include/abc_test/external/bigint/BigIntegerAlgorithms.hpp
    include/abc_test/external/bigint/BigIntegerLibrary.hpp
    include/abc_test/external/bigint/BigIntegerUtils.hpp
    include/abc_test/external/bigint/BigUnsigned.hpp
    include/abc_test/external/bigint/BigUnsignedInABase.hpp
    include/abc_test/external/bigint/NumberlikeArray.hpp
  )

  # Set the external cpp files variable. This is a list of all the external cpp files. Not made by abc_test.
  set(ABC_EXTERNAL_SOURCE_FILES
        src/abc_test/external/bigint/BigInteger.cpp
        src/abc_test/external/bigint/BigIntegerAlgorithms.cpp
        src/abc_test/external/bigint/BigIntegerUtils.cpp
        src/abc_test/external/bigint/BigUnsigned.cpp
        src/abc_test/external/bigint/BigUnsignedInABase.cpp
  )

  # Sets how the IDE organises the files.
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/.. FILES
    ${ABC_INTERNAL_SOURCES}
    ${ABC_INTERNAL_HEADERS}
    ${ABC_EXTERNAL_SOURCE_FILES}
    ${ABC_EXTERNAL_HEADER_FILES}
  )

  # Adds the library itself.
  add_library(${ABC_LIBRARY_NAME})

  # Sets the sources of the library.
  target_sources(${ABC_LIBRARY_NAME}
    PRIVATE
      ${ABC_INTERNAL_SOURCES}
      ${ABC_EXTERNAL_SOURCE_FILES}
    PUBLIC
      FILE_SET public_headers TYPE HEADERS
      BASE_DIRS include
      FILES ${ABC_INTERNAL_HEADERS}
    PRIVATE
      FILE_SET private_external_headers TYPE HEADERS
      BASE_DIRS include
      FILES ${ABC_EXTERNAL_HEADER_FILES}
  )

  set_common_compile_options(${ABC_LIBRARY_NAME})

  # Add the include directories to the target.
  target_include_directories(${ABC_LIBRARY_NAME}
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>
  )

  target_include_directories(${ABC_LIBRARY_NAME}
    SYSTEM PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/abc_test/external>
  )
  # Link the ABC_LIBRARY_REQUIRED_LIBS to the ABC_LIBRARY_NAME
  target_link_libraries(${ABC_LIBRARY_NAME} PUBLIC ${ABC_LIBRARY_REQUIRED_LIBS})

  # Required for gcc toolchain and the Clang toolchain under linux.
  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_SYSTEM_NAME STREQUAL "Linux"))
    target_link_libraries(${ABC_LIBRARY_NAME} PUBLIC stdc++_libbacktrace)
  endif()

  # Set metadata folder
  set(METADATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/metadata")
  # Ensure metadata folder exists
  file(MAKE_DIRECTORY "${METADATA_DIR}")
  set(LIST_OF_ABC_TEST_FILES "${METADATA_DIR}/list_of_all_files_in_abc_test.txt")

  # Write to the LIST_OF_ABC_TEST_FILES.
  file(WRITE ${LIST_OF_ABC_TEST_FILES} "")  

  # Generate a file at configure time
  set(ALL_FILES "")
  foreach(FILE_PATH IN LISTS ABC_INTERNAL_HEADERS ABC_INTERNAL_SOURCES)
    get_filename_component(ABSOLUTE_PATH ${FILE_PATH} ABSOLUTE BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    string(APPEND ALL_FILES "${ABSOLUTE_PATH}\n")
  endforeach()

  file(WRITE "${CMAKE_BINARY_DIR}/tmp_list.txt" "${ALL_FILES}")
  configure_file("${CMAKE_BINARY_DIR}/tmp_list.txt" "${LIST_OF_ABC_TEST_FILES}" COPYONLY)
endif()
