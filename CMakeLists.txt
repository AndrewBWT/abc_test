# Current CMake version. We have not tested with other versions.
cmake_minimum_required(VERSION 3.28)
include(scripts/cmake/compile_options.cmake)
# Set up the project information
project(
  abc_test
  VERSION 			0.0.1
  DESCRIPTION 		"A C++ testing library."
  LANGUAGES 		CXX
)

# Set up the variables TOP_LEVEL_PROJECT and RUNNING_TESTS

# ABC_TOP_LEVEL_PROJECT informs the user if this is the top level project.
# If it is the top level project, we assume that it is being built on its own.
set(ABC_TOP_LEVEL_PROJECT FALSE)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(ABC_TOP_LEVEL_PROJECT TRUE)
endif()

# We include this here so that the variable BUILD_TESTING is now defined as TRUE (due to its inclusion by CTest).
# Remember, the user can set BUILD_TESTING themselves if they so wish, include(CTest) won't override that value.
if(ABC_TOP_LEVEL_PROJECT)
  include(CTest)
endif()

# Ensures that BUILD_TESTING is defined as FALSE. If ABC_TOP_LEVEL_PROJECT == FALSE, it would be an undefined value.
if(NOT DEFINED BUILD_TESTING)
  set(BUILD_TESTING OFF)
endif()

# Sets up the ABC_RUNNING_TESTS variable.
set(ABC_RUNNING_TESTS FALSE)
if(ABC_TOP_LEVEL_PROJECT AND BUILD_TESTING)
  set(ABC_RUNNING_TESTS TRUE)
endif()

# Set up the options for the user.
option(ABC_TEST_ENABLE_DOCS "Build abc_test documentation" ${ABC_TOP_LEVEL_PROJECT})
option(ABC_TEST_ENABLE_TESTS "Build abc_test tests" ${ABC_RUNNING_TESTS})
option(ABC_TEST_ENABLE_EXAMPLES "Build abc_test examples" ${ABC_TOP_LEVEL_PROJECT})

# Set these if its the top level directory. Kept for when building the project itself, as I have found issues when not setting these in some older versions of visual studio.
# We have commented this out for now, as we want to test with older compilers to check whether this is actually an issue, or whether this is something that was caused by incorrect configuration by us.
#if(ABC_TOP_LEVEL_PROJECT)
#  set(CMAKE_CXX_STANDARD 23)
#  set(CMAKE_CXX_STANDARD_REQUIRED ON)
#  set(CMAKE_CXX_EXTENSIONS OFF)
#endif()

# Fetch the fmt library if its the top level project
# Only use FetchContent if fmt::fmt is not already provided
if(NOT TARGET fmt::fmt)
  include(FetchContent)

  if(ABC_TOP_LEVEL_PROJECT)
    FetchContent_Declare(
      fmt
      GIT_REPOSITORY https://github.com/fmtlib/fmt.git
      GIT_TAG 11.2.0
    )
    FetchContent_MakeAvailable(fmt)
  else()
    FetchContent_GetProperties(fmt)
    if(NOT fmt_POPULATED)
      message(FATAL_ERROR "Required target fmt::fmt not found.
Please make sure:
  - fmt::fmt is available (e.g., via find_package(fmt CONFIG REQUIRED)),
  - or build abc_test as a top-level project.")
    endif()
  endif()
endif()

# Show the user the status of the core variables.
message(STATUS "Top level project: ${ABC_TOP_LEVEL_PROJECT}")
message(STATUS "Building tests: ${ABC_TEST_ENABLE_TESTS}")
message(STATUS "Building docs: ${ABC_TEST_ENABLE_DOCS}")
message(STATUS "Building examples: ${ABC_TEST_ENABLE_EXAMPLES}")
  
# Add the library folder.
add_subdirectory(library)

# Enable documentation if required.
if(ABC_TEST_ENABLE_DOCS)
  find_package(Doxygen)
  if(Doxygen_FOUND)
    add_subdirectory(docs)
  else()
    message(STATUS "Doxygen not found, skipping documentation creation.")
  endif()
endif()

# Enable tests if required
if(ABC_TEST_ENABLE_TESTS)
  add_subdirectory(tests)
endif()

# Enable examples if required
if(ABC_TEST_ENABLE_EXAMPLES)
  add_subdirectory(examples)
endif()
