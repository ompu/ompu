include("${SAYA_ROOT}/cmake/saya-test-generator.cmake")

set(OMPU_GTEST_ROOT "${OMPU_MODULES_ROOT}/googletest")
set(test_flags "OMPU_TESTING;UNISTR_FROM_STRING_EXPLICIT=explicit")
set(test_include_dirs "${OMPU_INCLUDE_DIRS}")
set(test_system_include_dirs "${OMPU_SYSTEM_INCLUDE_DIRS}")

if(MSVC)
  link_directories("${BOOST_ROOT}/lib")
endif()

list(
  APPEND test_include_dirs
  "include"
)
list(
  APPEND test_system_include_dirs
  "${OMPU_GTEST_ROOT}/googletest/include"
)

saya_enable_gtest(
  "${OMPU_GTEST_ROOT}"
  gtest_main.cpp
  "${test_include_dirs}"
  "${test_system_include_dirs}"
  "${test_flags}"
)

# -------------------------------------------------------
macro(resolve_tests files)
  set(tests "")
  foreach(f IN ITEMS "${files}")
    list(APPEND tests "${f}_test.cpp")
  endforeach()
endmacro()

resolve_tests(
  "io_reader_chordpro"
  "io_writer_chordpro"
)

saya_generate_tests("${OMPU_GTEST_ROOT}" "${tests}" "${test_include_dirs}" "${test_system_include_dirs}" "${OMPU_LIBS}" "${test_flags}")
