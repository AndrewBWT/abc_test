

# Get the include directory from the header file.
get_target_property(PROJECT_HEADER_FILES abc_test INTERFACE_INCLUDE_DIRECTORIES)

# Probably not a great idea. But unsure how else to get this information.
file(GLOB_RECURSE PROJECT_PUBLIC_HEADERS ${PROJECT_HEADER_FILES}/*.hpp)

set(ADDITIONAL_FILES
  # README.md
  ${PROJECT_SOURCE_DIR}/docs/user_written_docs/documentation_overview.md
  ${PROJECT_SOURCE_DIR}/docs/user_written_docs/overview_of_framework.md
  ${PROJECT_SOURCE_DIR}/docs/user_written_docs/declaring_tests.md
  ${PROJECT_SOURCE_DIR}/docs/user_written_docs/test_assertions.md
)

#list(APPEND PROJECT_PUBLIC_HEADERS ${PROJECT_SOURCE_DIR}/docs/additional_docs/DECLARING_TESTS.md)
# Set where output is written to.
#set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs/doxygen)

# Set where the index file will be.
#set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)

#Doxyfile input file
#set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
#Doxyfile output file
#set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

#Write the Doxyfile. This could be documented better.
#configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

#Need to make the directory, as Doxygen won't make this automatically
#file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR}) 

# Add custom command which runs Doxygen.
#add_custom_command(
#  OUTPUT ${DOXYGEN_INDEX_FILE}
#  DEPENDS ${PROJECT_PUBLIC_HEADERS}
#  COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
#  MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
#  COMMENT "Generating docs"
#  VERBATIM
#)

#Unsure about this one. Need to check documentation regarding this.
#add_custom_target(Doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})


#set(DOXYGEN_INPUT                  ${PROJECT_SOURCE_DIR}/docs/additional_docs)
set(DOXYGEN_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/docs/additional_docs)
set(DOXYGEN_HTML_OUTPUT            ${PROJECT_BINARY_DIR}/docs/html)
set(DOXYGEN_JAVADOC_AUTOBRIEF      YES)
set(DOXYGEN_GENERATE_HTML          YES)
set(DOXYGEN_HAVE_DOT               NO)
set(DOXYGEN_USE_MDFILE_AS_MAINPAGE Documentation_Overview.md)  # requires a README.md in /myproject/
set(DOXYGEN_GENERATE_TREEVIEW               YES)
set(DOXYGEN_DISABLE_INDEX               NO)
set(DOXYGEN_FULL_SIDEBAR               NO)
set(DOXYGEN_HTML_EXTRA_STYLESHEET               doxygen-awesome-css/doxygen-awesome.css doxygen-awesome-css/doxygen-awesome-sidebar-only.css)
set(DOXYGEN_HTML_COLORSTYLE               LIGHT)

# other DOXYGEN_... options

doxygen_add_docs(docs_target
  ${PROJECT_PUBLIC_HEADERS}
  ${ADDITIONAL_FILES}
  ALL  # does not seem to work
  COMMENT "Generate HTML documentation"
#  CONFIG_FILE "Doxy_Config.txt"
#  CONFIG_FILE "G:/MyProjects/cpp/git_projects/abc_test/docs/Doxy_Config.txt"
#  CONFIG_FILE "../docs/Doxy_Config.txt"
)