# Guard against rebuilding by accident.
if(NOT DEFINED ABC_TEST_EXAMPLES_INCLUDED)
  set(ABC_TEST_EXAMPLES_INCLUDED TRUE)
  # Set library name so that, if we change it, we only have to change this line
  # Set the name of the example executable once
  set(EXAMPLE_EXECUTABLE_NAME abc_test_examples)
	  
  set(ABC_TEST_EXAMPLE_RUN_ARGS
    "${CMAKE_CURRENT_SOURCE_DIR}/data/examples_config"
    CACHE STRING "Optional runtime args for abc_test_examples executable")

  # These are the libs required for the testing suite.
  set(ABC_EXAMPLES_REQUIRED_LIBS
    abc_test 
    fmt::fmt
  )

  # The set of all example files.
  set(ABC_TEST_EXAMPLE_FILES
    include/abc_test_examples/example_001.hpp
        src/main.cpp
    include/abc_test_examples/enumeration.hpp
    include/abc_test_examples/enumeration/enum_enumeration_examples.hpp
    include/abc_test_examples/basic_examples.hpp
    include/abc_test_examples/readme.hpp
    include/abc_test_examples/basic_examples/01_assertions.hpp
    include/abc_test_examples/basic_examples/02_basic_matchers.hpp
    include/abc_test_examples/basic_examples/03_matchers_and_logical_operators.hpp
    include/abc_test_examples/basic_examples/04_matchers_and_comparison_operators.hpp
    include/abc_test_examples/basic_examples/05_user_defined_matcher.hpp
    include/abc_test_examples/basic_examples/06_block_assertions_and_exception_handling.hpp
    include/abc_test_examples/basic_examples/exceptions.hpp
    include/abc_test_examples/data_generator_examples.hpp
    include/abc_test_examples/data_generator_examples/01_static_gen_data_examples.hpp
    include/abc_test_examples/data_generator_examples/02_generate_data_from_file_examples.hpp
    include/abc_test_examples/data_generator_examples/03_generate_data_randomly.hpp
    include/abc_test_examples/data_generator_examples/04_enumerating_data_examples.hpp
    include/abc_test_examples/data_generator_examples/05_manual_data_generator_examples.hpp
    include/abc_test_examples/data_generator_examples/06_combinatorics_data_generator_examples.hpp
    include/abc_test_examples/documentation/matchers.hpp
    include/abc_test_examples/documentation/test_assertions.hpp
    include/abc_test_examples/documentation/declaring_tests.hpp
    include/abc_test_examples/documentation.hpp
    include/abc_test_examples/documentation/data_generators.hpp
    include/abc_test_examples/documentation/readme_1.hpp
    include/abc_test_examples/documentation/readme_2.hpp
  )
  
  # Sets how the IDE organises the files.
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/.. FILES
    ${ABC_TEST_EXAMPLE_FILES}
  )

  # Add the set of files to executable.
  add_executable(${EXAMPLE_EXECUTABLE_NAME} ${ABC_TEST_EXAMPLE_FILES})
 
   set_common_compile_options(${EXAMPLE_EXECUTABLE_NAME})
  
  # Add the include directory to the target.
  target_include_directories(${EXAMPLE_EXECUTABLE_NAME} PRIVATE include)
  # Link the REQUIRED_LIBS to the LIBRARY
  target_link_libraries(${EXAMPLE_EXECUTABLE_NAME} PUBLIC ${ABC_EXAMPLES_REQUIRED_LIBS})
  # Ensure that the IDE sees the files in the file structure they are organised in.
		   
  # Adds the test as a command
  add_test(NAME "Run abc_test examples executable"
           COMMAND ${EXAMPLE_EXECUTABLE_NAME} "--config" ${ABC_TEST_EXAMPLE_RUN_ARGS})
endif()