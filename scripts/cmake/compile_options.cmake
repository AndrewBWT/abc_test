function(set_common_compile_options TARGET)
  target_compile_features(${TARGET} PUBLIC cxx_std_23)
  
  target_compile_options(${TARGET} PRIVATE
    "$<$<CXX_COMPILER_ID:MSVC>:/bigobj>"
    "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
  )
  if(MSVC)
#    target_compile_options(${TARGET} PRIVATE /W4 /permissive- /bigobj /utf-8)
  else()
#    target_compile_options(${TARGET} PRIVATE -Wall -Wextra -pedantic)
  endif()
  set_target_properties(${TARGET} PROPERTIES CXX_EXTENSIONS OFF)
endfunction()