cmake_minimum_required(VERSION 3.1)

#-----------------------------------------------------------------------------
# Test Options
#-----------------------------------------------------------------------------

option(BIT_CONCURRENCY_COMPILE_ASAN "Compile and run the address sanetizer" off)
option(BIT_CONCURRENCY_COMPILE_MSAN "Compile and run the memory sanetizer" off)
option(BIT_CONCURRENCY_COMPILE_USAN "Compile and run the undefined behavior sanitizer" off)
option(BIT_CONCURRENCY_COMPILE_TSAN "Compile and run the thread sanitizer" off)
option(BIT_CONCURRENCY_COMPILE_COVERAGE "Compile and run code coverage" off)

#-----------------------------------------------------------------------------
# Unit Tests
#-----------------------------------------------------------------------------

find_package(Catch REQUIRED)

set(sources

      src/main.test.cpp
)

add_executable(bit_concurrency_test ${sources})

target_link_libraries(bit_concurrency_test PRIVATE "bit::concurrency" "philsquared::Catch")

#-----------------------------------------------------------------------------

add_test( NAME "bit_concurrency_test_default"
          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
          COMMAND "$<TARGET_FILE:bit_concurrency_test>" )

add_test( NAME "bit_concurrency_test_all"
          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
          COMMAND "$<TARGET_FILE:bit_concurrency_test>" "*" )

if( NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND
    NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
  return() # Sanitizers are only for LLVM Clang and G++
endif()

#-----------------------------------------------------------------------------
# Address Sanitizer
#-----------------------------------------------------------------------------

if( BIT_CONCURRENCY_COMPILE_ASAN )

  target_link_libraries(bit_concurrency_test PRIVATE -fsanitize=address
                                             -fsanitize=leak
                                             -fuse-ld=gold)
  target_compile_options(bit_concurrency_test PRIVATE
    -g
    -O1
    -fno-omit-frame-pointer
    -fsanitize=address
    -fsanitize=leak
    -fuse-ld=gold
  )

endif()

#-----------------------------------------------------------------------------
# Memory Sanitizer
#-----------------------------------------------------------------------------

if( BIT_CONCURRENCY_COMPILE_MSAN )

  target_link_libraries(bit_concurrency_test PRIVATE -fsanitize=memory
                                                     -fsanitize-memory-track-origins
                                                     -fsanitize-memory-use-after-dtor
                                                     -fuse-ld=gold)
  target_compile_options(bit_concurrency_test PRIVATE
    -g
    -O1
    -fno-omit-frame-pointer
    -fsanitize=memory
    -fsanitize-memory-track-origins
    -fsanitize-memory-use-after-dtor
    -fuse-ld=gold
  )

endif()

#-----------------------------------------------------------------------------
# Undefined Behavior Sanitizer
#-----------------------------------------------------------------------------

if( BIT_CONCURRENCY_COMPILE_UBSAN )

  target_compile_options(bit_concurrency_test PRIVATE -fsanitize=undefined -fuse-ld=gold)
  target_link_libraries(bit_concurrency_test PRIVATE -fsanitize=undefined -fuse-ld=gold)

endif()

#-----------------------------------------------------------------------------
# Thread Sanitizer
#-----------------------------------------------------------------------------

if( BIT_CONCURRENCY_COMPILE_TSAN )

  target_compile_options(bit_concurrency_test PRIVATE -fsanitize=thread -fuse-ld=gold)
  target_link_libraries(bit_concurrency_test PRIVATE -fsanitize=thread -fuse-ld=gold)

endif()

#-----------------------------------------------------------------------------
# Code Coverage
#-----------------------------------------------------------------------------

if( BIT_CONCURRENCY_COMPILE_COVERAGE )

  target_link_libraries(bit_concurrency_test PRIVATE --coverage)
  target_compile_options(bit_concurrency_test PRIVATE
    -g
    -O0
    -fprofile-arcs
    -ftest-coverage
  )

endif()
