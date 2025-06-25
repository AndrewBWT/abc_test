# Set the name of the example executable once
set(EXAMPLE_EXECUTABLE_NAME abc_test_examples)

# The set of all example files.
set(EXAMPLE_FILES
  include/abc_test_examples/example_001.hpp
      src/main.cpp
  include/abc_test_examples/enumeration.hpp
  include/abc_test_examples/enumeration/enum_enumeration_examples.hpp
  include/abc_test_examples/basic_examples.hpp
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

# These are the libs required for the examples executable.
set(REQUIRED_LIBS
  abc_test
)

# Add the set of files to executable.
add_executable(${EXAMPLE_EXECUTABLE_NAME} ${EXAMPLE_FILES})
  
# Add the include directory to the target.
target_include_directories(${EXAMPLE_EXECUTABLE_NAME} PUBLIC include)
# Link the REQUIRED_LIBS to the LIBRARY
target_link_libraries(${EXAMPLE_EXECUTABLE_NAME} PUBLIC ${REQUIRED_LIBS})
# Ensure that REQUIRED_LIBS are built before the executable.
add_dependencies(${EXAMPLE_EXECUTABLE_NAME} ${REQUIRED_LIBS})
# Ensure that the IDE sees the files in the file structure they are organised in.
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/.." FILES ${EXAMPLE_FILES})
# Unset variable defined in this file.
unset(LIBRARY_NAME)
unset(REQUIRED_LIBS)
unset(LIBRARY_FILES)