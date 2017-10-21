cmake_minimum_required(VERSION 3.9)
project(ompu VERSION 1.0.0 LANGUAGES C CXX)

set(OMPU_ROOT "${PROJECT_SOURCE_DIR}")
set(OMPU_MODULES_ROOT "${OMPU_ROOT}/modules")
set(SAYA_ROOT "${OMPU_MODULES_ROOT}/saya")


set(
  CMAKE_MODULE_PATH
  "${SAYA_ROOT}/cmake"
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
  ${CMAKE_MODULE_PATH}
)

if(NOT BOOST_ROOT)
  message(FATAL_ERROR "please specify BOOST_ROOT")
endif()


find_package(Threads REQUIRED)


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
message(STATUS "ompu configuration: ${CMAKE_BUILD_TYPE}")

# ------------------------------------------------

# Global build config
if(MSVC)
  set(CMAKE_CXX_FLAGS "/std:c++latest /W4 /WX /MP /EHsc /DNOMINMAX /DBOOST_SCOPE_EXIT_CONFIG_USE_LAMBDAS=1 /D_HAS_AUTO_PTR_ETC=1")
  set(CMAKE_CXX_FLAGS_DEBUG "/MTd /Zi")
  set(CMAKE_CXX_FLAGS_RELEASE "/MT /DNDEBUG")

  add_definitions(-DUNICODE)
  add_definitions(-D_UNICODE)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)
else()
  set(CMAKE_CXX_FLAGS "-std=c++1y -Wall -Wextra -pedantic-errors")
  set(CMAKE_CXX_FLAGS_DEBUG "-g -O0") # -fno-inline -pg
  set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")


# source config ---------------------------------------
set(
  OMPU_SYSTEM_INCLUDE_DIRS
  "${BOOST_ROOT}/include"
  "${OMPU_MODULES_ROOT}/sprout"
  "${SAYA_ROOT}/include"
)
set(
  OMPU_LIBS
  
  # TODO: saya

  "${CMAKE_THREAD_LIBS_INIT}"
)

if(NOT MSVC)
  list(
    APPEND OMPU_SYSTEM_INCLUDE_DIRS
    "${Boost_INCLUDE_DIRS}"
	"${ICU_INCLUDE_DIRS}"
    "${OPENSSL_INCLUDE_DIRS}"
  )
  list(
	APPEND OMPU_LIBS
	${Boost_LIBRARIES}
	${ICU_LIBRARIES}
	${OPENSSL_LIBRARIES}
  )
endif()

set(
  OMPU_INCLUDE_DIRS
  "${OMPU_ROOT}/include"
)

set(CMAKE_DEBUG_POSTFIX d)
add_subdirectory(src)

# tests ---------------------------------------
include(CTest)
add_subdirectory(test)
