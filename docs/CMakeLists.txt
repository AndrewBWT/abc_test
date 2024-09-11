# Get the include directory from the header file.
get_target_property(PROJECT_HEADER_FILES abc_test INTERFACE_INCLUDE_DIRECTORIES)

# Probably not a great idea. But unsure how else to get this information.
file(GLOB_RECURSE PROJECT_PUBLIC_HEADERS ${PROJECT_HEADER_FILES}/*.h)

# Set where output is written to.
set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs/doxygen)

# Set where the index file will be.
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)

#Doxyfile input file
set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
#Doxyfile output file
set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

#Write the Doxyfile. This could be documented better.
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

#Need to make the directory, as Doxygen won't make this automatically
file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR}) 

# Add custom command which runs Doxygen.
add_custom_command(
  OUTPUT ${DOXYGEN_INDEX_FILE}
  DEPENDS ${PROJECT_PUBLIC_HEADERS}
  COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
  MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
  COMMENT "Generating docs"
  VERBATIM
)

#Unsure about this one. Need to check documentation regarding this.
add_custom_target(Doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})