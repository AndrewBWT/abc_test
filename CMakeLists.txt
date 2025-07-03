# Current CMake version. We have not tested with other versions.
cmake_minimum_required(VERSION 3.28)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/bigobj>")
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
#FetchContent needed to get fmt.
include(FetchContent)
#Fetch and make fmt available
FetchContent_Declare(
  fmt
  GIT_REPOSITORY 	https://github.com/fmtlib/fmt.git
  GIT_TAG 			master
)
FetchContent_MakeAvailable(fmt)

# Set up the project information
project(
  abc_test
  VERSION 			0.1.0.5
  DESCRIPTION 		"A C++ testing library."
  LANGUAGES 		CXX
)

# Boolean variable which states whether this is the top-level project being built.
set (TOP_LEVEL_PROJECT (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME))

# Boolean variable which states whether tests are to be ran or not. Testa are only ran
# when build testing is set to true, and when the TOP_LEVEL_PROJECT variable is set to true.
set(RUNNING_TESTS (TOP_LEVEL_PROJECT AND BUILD_TESTING))

if(TOP_LEVEL_PROJECT)

  #Set C++ standard
  set(CMAKE_CXX_STANDARD 23)
  # Set the linker language to C++.
  set(LINKER_LANGUAGE CXX)
  
  # Don't allow extensions
  set(CMAKE_CXX_EXTENSIONS OFF)

  # Ensure that the folders work nicely with IDE's.
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  # Enable testing.
  include(CTest)
endif()
  
# Add the library folder.
add_subdirectory(library)

# If the tests are being ran.
if(RUNNING_TESTS)
  # Add the test folder.
  add_subdirectory(tests)
endif()

#If top level directory, we can generate the documentation.
if(TOP_LEVEL_PROJECT)
  # Find Doxygen on the system.
  find_package(Doxygen)
  # If Doxygen is on the system.
  if(Doxygen_FOUND)
    # Add documentation folder.
    add_subdirectory(docs)
  else()
    # Else, tell the developer that we could not load the documentation.
    message(STATUS "Doxygen not found, not building docs")
  endif()
endif()

# If top level project, build the examples.
if (TOP_LEVEL_PROJECT)
  add_subdirectory(examples)
endif()
