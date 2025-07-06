# Set the name of the test lib once.
set(TEST_LIB_NAME testlib)


# The set of all test files. The idea behind these is they mirror the files in the library.
# However we may change this as we move forward.

#There are some additional entries at the end which are not mirrored by files in the library.
set(TEST_FILES
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
)

# These are the libs required for the testing suite.
set(REQUIRED_LIBS
  abc_test 
  fmt::fmt
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