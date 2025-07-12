
# Guard against rebuilding by accident.
if(NOT DEFINED ABC_TEST_DOCS_INCLUDED)
  set(ABC_TEST_DOCS_INCLUDED TRUE)
  set(ADDITIONAL_FILES
    # README.md
    ${PROJECT_SOURCE_DIR}/docs/user_written_docs/documentation_overview.md
    ${PROJECT_SOURCE_DIR}/docs/user_written_docs/overview_of_framework.md
    ${PROJECT_SOURCE_DIR}/docs/user_written_docs/declaring_tests.md
    ${PROJECT_SOURCE_DIR}/docs/user_written_docs/test_assertions.md
  )

  set(DOXYGEN_HTML_OUTPUT            ${PROJECT_BINARY_DIR}/docs/html)
  set(DOXYGEN_JAVADOC_AUTOBRIEF      YES)
  set(DOXYGEN_GENERATE_HTML          YES)
  set(DOXYGEN_HAVE_DOT               NO)
  set(DOXYGEN_USE_MDFILE_AS_MAINPAGE documentation_overview.md)
  set(DOXYGEN_GENERATE_TREEVIEW               YES)
  set(DOXYGEN_DISABLE_INDEX               NO)
  set(DOXYGEN_FULL_SIDEBAR               NO)
  set(DOXYGEN_HTML_EXTRA_STYLESHEET               doxygen-awesome-css/doxygen-awesome.css doxygen-awesome-css/doxygen-awesome-sidebar-only.css)
  set(DOXYGEN_HTML_COLORSTYLE               LIGHT)

# other DOXYGEN_... options

  doxygen_add_docs(docs_target
    ${ABC_INTERNAL_HEADERS}
    ${ADDITIONAL_FILES}
    ALL  # does not seem to work
    COMMENT "Generate HTML documentation"

  )
endif()