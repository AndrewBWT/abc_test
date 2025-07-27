# Guard against rebuilding by accident.
if(NOT DEFINED ABC_TEST_TESTS_INCLUDED)
  set(ABC_TEST_TESTS_INCLUDED TRUE)
  # Set library name so that, if we change it, we only have to change this line
  # Set the name of the example executable once
  set(ABC_TESTLIB_EXECUTABLE_NAME abc_test_test)
  
  set(ABC_TEST_TEST_RUN_ARGS
    "${CMAKE_CURRENT_SOURCE_DIR}/data/abc_test_test_config"
    CACHE STRING "Optional runtime args for abc_test_test executable")

  # The set of all test files. The idea behind these is they mirror the files in the library.
  # However we may change this as we move forward.
  
  # These are the libs required for the testing suite.
  set(ABC_TEST_TESTS_REQUIRED_LIBS
    abc_test 
    fmt::fmt
  )

  #There are some additional entries at the end which are not mirrored by files in the library.
  set(ABC_TEST_TESTS_FILES
    # Files which test their abc_test counterparts.
    include/abc_test_test/utility.hpp
    include/abc_test_test/utility/cli.hpp
    include/abc_test_test/utility/bounds.hpp
    include/abc_test_test/utility/str.hpp
    include/abc_test_test/utility/str/unicode.hpp
    include/abc_test_test/utility/str/concepts.hpp
    include/abc_test_test/utility/str/string_cast.hpp
    include/abc_test_test/utility/str/print_utils.hpp
    include/abc_test_test/utility/rng.hpp
    include/abc_test_test/utility/enum.hpp
    include/abc_test_test/utility/ranges.hpp
    include/abc_test_test/utility/concepts.hpp
    include/abc_test_test/utility/rng/inner_rng_mt19937_64.hpp
    include/abc_test_test/utility/rng/rng_utilities.hpp
    include/abc_test_test/included_instances.hpp
    include/abc_test_test/included_instances/data_generator/enumeration/default_enumeration.hpp
    include/abc_test_test/included_instances/data_generator/random_generator/default_random_generator.hpp
    include/abc_test_test/included_instances/data_generator/random_generator/default_random_generator/detail.hpp
    include/abc_test_test/included_instances/data_generator/random_generator/default_random_generator/basic_string.hpp
    include/abc_test_test/core/ds/test_data/user_defined_test_data.hpp
        src/main.cpp
    include/abc_test_test/core.hpp
    include/abc_test_test/core/errors/test_library_exception.hpp
    # Contains simple random generator for testing randomly generated data.
    include/abc_test_test/testing_utilities/simple_random_generator.hpp
    include/abc_test_test/testing_utilities/functions.hpp
	include/abc_test_test/testing_utilities/bootstrapped_testing_utilities.hpp
	
	include/abc_test_test/component_tests.hpp
    include/abc_test_test/component_tests/data_generators.hpp
    include/abc_test_test/component_tests/data_generators/static_data_generator.hpp
	include/abc_test_test/component_tests/data_generators/file_data_generator.hpp
  )
  
  # Sets how the IDE organises the files.
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/.. FILES
    ${ABC_TEST_TESTS_FILES}
  )

  # Add the set of files to executable.
  add_executable(${ABC_TESTLIB_EXECUTABLE_NAME} ${ABC_TEST_TESTS_FILES})
  
  set_common_compile_options(${ABC_TESTLIB_EXECUTABLE_NAME})
  
  # Add the include directory to the target.
  target_include_directories(${ABC_TESTLIB_EXECUTABLE_NAME} PRIVATE include)
  # Link the REQUIRED_LIBS to the LIBRARY
  target_link_libraries(${ABC_TESTLIB_EXECUTABLE_NAME} PUBLIC ${ABC_TEST_TESTS_REQUIRED_LIBS})
  # Ensure that the IDE sees the files in the file structure they are organised in.
  
  # Adds the test as a command
  add_test(NAME "Run abc_tests test suite executable"
           COMMAND ${ABC_TESTLIB_EXECUTABLE_NAME} "--config" ${ABC_TEST_TEST_RUN_ARGS})

  # Interpreted scripts
  find_package(Python3 REQUIRED COMPONENTS Interpreter)
  add_test(NAME "Check abc_test can be used as external testing library"
           COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/script_to_use_library.py)
endif()