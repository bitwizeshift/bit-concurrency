# `bit::concurrency` library

[![Build Status](https://travis-ci.org/bitwizeshift/bit-concurrency.svg?branch=master)](https://travis-ci.org/bitwizeshift/bit-concurrency)
[![Build status](https://ci.appveyor.com/api/projects/status/a0gi3i20pye7038a/branch/master?svg=true)](https://ci.appveyor.com/project/bitwizeshift/bit-concurrency/branch/master)
[![Github Issues](https://img.shields.io/github/issues/bitwizeshift/bit-concurrency.svg)](http://github.com/bitwizeshift/bit-concurrency/issues)
[![Tested Compilers](https://img.shields.io/badge/compilers-gcc%20%7C%20clang%20%7C%20msvc-blue.svg)](#tested-compilers)
[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](http://bitwizeshift.github.io/bit-concurrency)
[![GitHub License](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/bitwizeshift/bit-concurrency/master/LICENSE.md)
[![Github Releases](https://img.shields.io/github/release/bitwizeshift/bit-concurrency.svg)](https://github.com/bitwizeshift/bit-concurrency/releases)

This library contains various concurrency utilities.

<!-- SECTION: Getting Started -->

## <a name="getting-started"></a>Getting Started

### <a name="getting-started-prerequisites"></a>Prerequisites

- [CMake](https://cmake.org) version 3.1 or above
- [Catch](https://github.com/philsquared/catch) version 2 (Unit Tests only)

<!-- SECTION: Building -->

## <a name="building"></a>Building

All building is done using CMake

### <a name="building-library"></a>Building Library

Building this library is just like building any library built with cmake

1. Configure the project
2. Build the target

```bash
mkdir build && cd build
cmake ..
cmake --build . # alternatively, cmake --build . --target bit_concurrency
```

### <a name="building-header-self-containment-tests"></a>Building Header Self-Containment Tests

This library includes a separate target that generates a series of `.cpp` files that include public
header files independently. The purpose of this target is to ensure that each header is written
without requiring transitive dependencies from the included file.

This catches headers written like:

**`foo.hpp`**
```c++
class foo{ ... };
```

**`foo_utilities.hpp`**
```c++
// Accidentally does not include 'B'
template<typename...Args>
foo make_foo(Args&&...args);
```

**`foo_utilities.cpp`**
```c++
#include "foo.hpp" // includes 'foo' before 'foo_utilities'
#include "foo_utilites.hpp"
```

Such an ordering would compile correctly when compiling `*.cpp` files, but would fail upon including
`foo_utilities.hpp` without first including `foo.hpp`

The Header Self-Containment Tests prevent these errors, since it will attempt to compile `foo.hpp`, which
will not resolve all the possible dependencies.

To build this test:

1. specify the CMake argument `-DBIT_CONCURRENCY_COMPILE_HEADER_SELF_CONTAINMENT_TESTS=On` during configuration,
2. build the containment tests target `bit_concurrency_header_self_containment_test`

```bash
mkdir build && cd build
cmake -DBIT_CONCURRENCY_COMPILE_HEADER_SELF_CONTAINMENT_TESTS=On ..
cmake --build . --target bit_concurrency_header_self_containment_test
```

### <a name="building-tests"></a>Building Tests

To build and run unit tests:

1. specify the CMake argument `-DBIT_CONCURRENCY_COMPILE_UNIT_TESTS=On` during configuration,
2. build the unit test target, and
3. run the unit tests with `ctest`

```bash
mkdir build && cd build
cmake -DBIT_CONCURRENCY_COMPILE_UNIT_TESTS=On ..
cmake --build . --target bit_concurrency_test
ctest . --output-on-failure
```

#### <a name="building-tests-sanitizers"></a>Sanitizers

There are various sanitizers available, all of which run with the current unit test suite.
Sanitizers currently only run for systems that support the `gold` linker (*nix based systems).

Note that only one sanitizer may be run at a given time due to conflicting compile-flag requirements.

##### ASAN (Address Sanitizer)

To enable Address Sanitizer (ASAN):

1. specify the CMake arguments `-DBIT_CONCURRENCY_COMPILE_UNIT_TESTS=On -DBIT_CONCURRENCY_COMPILE_ASAN=On` during configuration,
2. build the unit test target, and
3. run the unit tests with `ctest`

```bash
mkdir build && cd build
cmake -DBIT_CONCURRENCY_COMPILE_UNIT_TESTS=On -DBIT_CONCURRENCY_COMPILE_ASAN=On ..
cmake --build . --target bit_concurrency_test
ctest . --output-on-failure
```

##### MSAN (Memory Sanitizer)

To enable Memory Sanitizer (MSAN):

1. export the environment variable `MSAN_OPTIONS=poison_in_dtor=1`,
2. specify the CMake arguments `-DBIT_CONCURRENCY_COMPILE_UNIT_TESTS=On -DBIT_CONCURRENCY_COMPILE_MSAN=On` during configuration,
3. build the unit test target, and
4. run the unit tests with `ctest`

```bash
mkdir build && cd build
export MSAN_OPTIONS=poison_in_dtor=1
cmake -DBIT_CONCURRENCY_COMPILE_UNIT_TESTS=On -DBIT_CONCURRENCY_COMPILE_MSAN=On ..
cmake --build . --target bit_concurrency_test
ctest . --output-on-failure
```

##### TSAN (Thread Sanitizer)

To enable Thread Sanitizer (TSAN):

1. specify the CMake arguments `-DBIT_CONCURRENCY_COMPILE_UNIT_TESTS=On -DBIT_CONCURRENCY_COMPILE_TSAN=On` during configuration,
2. build the unit test target, and
3. run the unit tests with `ctest`

```bash
mkdir build && cd build
cmake -DBIT_CONCURRENCY_COMPILE_UNIT_TESTS=On -DBIT_CONCURRENCY_COMPILE_TSAN=On ..
cmake --build . --target bit_concurrency_test
ctest . --output-on-failure
```

##### UBSAN (Undefined-Behavior Sanitizer)

To enable Undefined Behavior Sanitizer (UBSAN):

1. specify the CMake arguments `-DBIT_CONCURRENCY_COMPILE_UNIT_TESTS=On -DBIT_CONCURRENCY_COMPILE_UBSAN=On` during configuration,
2. build the unit test target, and
3. run the unit tests with `ctest`

```bash
mkdir build && cd build
cmake -DBIT_CONCURRENCY_COMPILE_UNIT_TESTS=On -DBIT_CONCURRENCY_COMPILE_UBSAN=On ..
cmake --build . --target bit_concurrency_test
ctest . --output-on-failure
```

#### <a name="building-tests-code-formatters"></a>Code Formatters

TODO: Add code-formatters

### <a name="building-tests-generating-documentation"></a>Generating Doxygen Documentation

To generate documentation:

1. specify the cmake configuration argument `-DBIT_CONCURRENCY_GENERATE_DOCUMENTATION=On`, and
2. build the cmake target `bit_concurrency_doxygen`

```bash
mkdir build && cd build
cmake -DBIT_CONCURRENCY_GENERATE_DOCUMENTATION=On ..
cmake --build . --target bit_concurrency_doxygen
```

This will generate documentation into the `doc/` directory

<!-- SECTION: Deploying -->

## <a name="deploying"></a>Deploying

TODO: Document deployment

### Installing

TODO: Document installation

### Using as Submodule

TODO: Document consuming as a submodule

<!-- SECTION: Tested Compilers -->

## <a name="tested-compilers"></a>Tested Compilers

Note that `bit-concurrency` is only tested on compilers that provide proper conformance for c++14 or above

The following compilers are currently being tested through continuous integration with [Travis](https://travis-ci.org/bitwizeshift/bit-concurrency) and [AppVeyor](https://ci.appveyor.com/project/bitwizeshift/bit-concurrency/)

| Compiler              | Operating System                   |
|-----------------------|------------------------------------|
| g++ 6.3.0             | Ubuntu 14.04.3 TLS                 |
| g++ 7.2.0             | Ubuntu 14.04.3 TLS                 |
| clang++ 3.9.0         | Ubuntu 14.04.3 TLS                 |
| clang++ 4.0.1         | Ubuntu 14.04.3 TLS                 |
| clang xcode 7.3       | Darwin Kernel 15.6.0 (OSX 10.11.6) |
| clang xcode 8.0       | Darwin Kernel 15.6.0 (OSX 10.11.6) |
| clang xcode 8.1       | Darwin Kernel 16.1.0 (OSX 10.12.1) |
| clang xcode 8.2       | Darwin Kernel 16.1.0 (OSX 10.12.1) |
| clang xcode 8.3       | Darwin Kernel 16.6.0 (OSX 10.12.5) |
| clang xcode 9.0       | Darwin Kernel 16.7.0 (OSX 10.12.6) |
| Visual Studio 2017    | Windows Server 2016 (x64)          |

<!-- SECTION: License -->

## <a name="license"></a>License

<img align="right" src="http://opensource.org/trademarks/opensource/OSI-Approved-License-100x137.png">

The class is licensed under the [MIT License](http://opensource.org/licenses/MIT):

Copyright &copy; 2018 Matthew Rodusek

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
