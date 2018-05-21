cmake_minimum_required(VERSION 3.1)

enable_testing()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

#-----------------------------------------------------------------------------
# Project Setup
#-----------------------------------------------------------------------------

# default to debug when building CMake
if( NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES )
  set(CMAKE_BUILD_TYPE DEBUG)
endif()

include(EnableCCache)

# enable cacche to speed up compilations
enable_ccache()

include(CMakeDependentOption)

option(BIT_CONCURRENCY_COMPILE_HEADER_SELF_CONTAINMENT_TESTS "Include each header independently in a .cpp file to determine header independence" on)
option(BIT_CONCURRENCY_COMPILE_UNIT_TESTS "Compile and run the unit tests for this library" on)
option(BIT_CONCURRENCY_GENERATE_DOCUMENTATION "Generates doxygen documentation" off)
option(BIT_CONCURRENCY_VERBOSE_CONFIGURE "Verbosely configures this library project" off)

project("BitConcurrency")

#-----------------------------------------------------------------------------
# Version Info
#-----------------------------------------------------------------------------

set(BIT_CONCURRENCY_VERSION_MAJOR 0 CACHE STRING "major version of bit::concurrency" FORCE)
set(BIT_CONCURRENCY_VERSION_MINOR 1 CACHE STRING "minor version of bit::concurrency" FORCE)
set(BIT_CONCURRENCY_VERSION_PATCH 0 CACHE STRING "patch version of bit::concurrency" FORCE)

set(BIT_CONCURRENCY_VERSION "${BIT_CONCURRENCY_VERSION_MAJOR}.${BIT_CONCURRENCY_VERSION_MINOR}.${BIT_CONCURRENCY_VERSION_PATCH}" CACHE STRING "version of bit::concurrency" FORCE)

if( BIT_CONCURRENCY_VERBOSE_CONFIGURE )
  message(STATUS "bit::concurrency ${BIT_CONCURRENCY_VERSION}")
endif()

#-----------------------------------------------------------------------------
# bit::concurrency
#-----------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)

include(MakeVersionHeader)

make_version_header("${CMAKE_CURRENT_BINARY_DIR}/include/bit/concurrency/version.hpp"
  MAJOR ${BIT_CONCURRENCY_VERSION_MAJOR}
  MINOR ${BIT_CONCURRENCY_VERSION_MINOR}
  PATCH ${BIT_CONCURRENCY_VERSION_PATCH}
  PREFIX BIT_CONCURRENCY
  SUFFIX "a"
)

set(inline_headers
  # Utilities
  include/bit/concurrency/utilities/detail/unlock_guard.inl

  # Locks
  include/bit/concurrency/locks/detail/null_mutex.inl
  include/bit/concurrency/locks/detail/semaphore.inl
  include/bit/concurrency/locks/detail/spinning_semaphore.inl
  include/bit/concurrency/locks/detail/waitable_event.inl
)

set(headers
  # Utilites
  include/bit/concurrency/utilities/unlock_guard.hpp

  # Locks
  include/bit/concurrency/locks/null_mutex.hpp
  include/bit/concurrency/locks/semaphore.hpp
  include/bit/concurrency/locks/shared_mutex.hpp
  include/bit/concurrency/locks/spin_lock.hpp
  include/bit/concurrency/locks/spinning_semaphore.hpp
  include/bit/concurrency/locks/waitable_event.hpp
)

if( WIN32 )
  set(platform_source_files
    src/bit/concurrency/locks/win32/semaphore.cpp
  )
elseif( UNIX )
  set(platform_source_files
    src/bit/concurrency/locks/posix/semaphore.cpp
  )
elseif( APPLE )
  set(platform_source_files
    src/bit/concurrency/locks/mac/semaphore.cpp
  )
else()
  message(FATAL_ERROR "unknown or unsupported target concurrency")
endif()

set(source_files
  src/bit/concurrency/locks/spin_lock.cpp

  # concurrency-specific
  ${platform_source_files}
)

include(GroupSourceTree)

add_library(concurrency ${source_files} ${headers} ${inline_headers})
add_library(bit::concurrency ALIAS concurrency)
group_source_tree("${CMAKE_CURRENT_LIST_DIR}" ${source_files} ${headers} ${inline_headers})

target_include_directories(concurrency PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
)

#-----------------------------------------------------------------------------
# bit::concurrency : Header self-containment Tests
#-----------------------------------------------------------------------------

include(AddHeaderSelfContainmentTest)

if( BIT_CONCURRENCY_COMPILE_HEADER_SELF_CONTAINMENT_TESTS )

  add_header_self_containment_test(bit_concurrency_header_self_containment_test ${headers})
  target_include_directories(bit_concurrency_header_self_containment_test PRIVATE
    $<TARGET_PROPERTY:concurrency,INCLUDE_DIRECTORIES>
  )

  target_sources(bit_concurrency_header_self_containment_test PRIVATE ${inline_headers})
  add_library(bit::concurrency::header_self_containment_test ALIAS bit_concurrency_header_self_containment_test)

  if( "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" OR
      "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
    target_compile_options(bit_concurrency_header_self_containment_test PRIVATE -Wall -Werror -pedantic)
  elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
    # TODO: Determine MSVC necessary compiler flags
  endif()

endif()

#-----------------------------------------------------------------------------
# bit::concurrency : Unit Tests
#-----------------------------------------------------------------------------

if( BIT_CONCURRENCY_COMPILE_UNIT_TESTS )
  add_subdirectory(test)
endif()

#-----------------------------------------------------------------------------
# bit::concurrency : Documentation
#-----------------------------------------------------------------------------

if( BIT_CONCURRENCY_GENERATE_DOCUMENTATION )
  add_subdirectory(doc)
endif()


#-----------------------------------------------------------------------------
# bit::concurrency : Export
#-----------------------------------------------------------------------------

include(ExportLibrary)

if( EXISTS "$ENV{BIT_HOME}" )
  set(CMAKE_INSTALL_PREFIX "$ENV{BIT_HOME}")
endif()

export_library( TARGETS concurrency
                PACKAGE Concurrency
                VERSION ${BIT_CONCURRENCY_VERSION}
                MAJOR_VERSION ${BIT_CONCURRENCY_VERSION_MAJOR}
                MINOR_VERSION ${BIT_CONCURRENCY_VERSION_MINOR}
                INCLUDE_PATHS "${CMAKE_CURRENT_LIST_DIR}/include"
                              "${CMAKE_CURRENT_BINARY_DIR}/include"
                NAMESPACE "bit::"
                PREFIX_PACKAGE )
