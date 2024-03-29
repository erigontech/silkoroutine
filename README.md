# Silkoroutine

C++20 coroutine framework wrapping Turbo-Geth internal [interfaces](https://github.com/ledgerwatch/interfaces)

[![CircleCI](https://circleci.com/gh/torquem-ch/silkoroutine.svg?style=shield)](https://circleci.com/gh/torquem-ch/silkoroutine)
[![Codecov master](https://img.shields.io/codecov/c/github/torquem-ch/silkoroutine/master.svg?style=shield&logo=codecov&logoColor=white)](https://codecov.io/gh/torquem-ch/silkoroutine)
![version](https://img.shields.io/github/v/release/torquem-ch/silkoroutine?sort=semver&color=normal)
[![License](https://img.shields.io/github/license/torquem-ch/silkoroutine?color=lightgrey)](https://img.shields.io/github/license/torquem-ch/silkoroutine)
![semver](https://img.shields.io/badge/semver-2.0.0-blue)

<br>

# Building the source

## Clone the repository

```
git clone --recurse-submodules git@github.com:torquem-ch/silkoroutine.git
```

To update the submodules later on run 
```
git submodule update --init --recursive
```

## Linux & MacOS
Building Silkoroutine requires
* C++20 compiler: [GCC](https://www.gnu.org/software/gcc/) >= 10.2.0 or [Clang](https://clang.llvm.org/) >= 10.0.0
* [CMake](http://cmake.org) >= 3.18.4
* [Cpplint] (https://github.com/cpplint/cpplint) (`pip3 install cpplint`)

Once the prerequisites are installed, bootstrap cmake by running
```
mkdir build
cd build
cmake ..
```
(BTW, you have to run `cmake ..` just the first time).

Generate the [gRPC](https://grpc.io/) Sentry interface protocol bindings
```
cmake --build . --target generate_sentry_grpc
```

Then run the build itself
```
cmake --build .
```

Now you can run the unit tests
```
cmd/unit_test
```

and check the code style running
```
./run_linter.sh
```

There are also convenience [bash](https://www.gnu.org/software/bash/) scripts for a complete rebuild both in debug and release configurations using [GCC](https://www.gnu.org/software/gcc/) compiler:
```
./build_gcc_debug.sh
./build_gcc_release.sh
```
and [Clang](https://clang.llvm.org/) compiler:
```
./build_clang_debug.sh
./build_clang_release.sh
```
The resulting build folders are `build_[gcc, clang]_[debug, release]` according to your choice.

## Windows
* Install [Visual Studio](https://www.visualstudio.com/downloads) 2019. Community edition is fine.
* Make sure your setup includes CMake support and Windows 10 SDK.
* Install [vcpkg](https://github.com/microsoft/vcpkg#quick-start-windows).
* `.\vcpkg\vcpkg install mpir:x64-windows`
* Add <VCPKG_ROOT>\installed\x64-windows\include to your `INCLUDE` environment variable.
* Add <VCPKG_ROOT>\installed\x64-windows\bin to your `PATH` environment variable.
* Open Visual Studio and select File -> CMake...
* Browse the folder where you have cloned this repository and select the file CMakeLists.txt
* Let CMake cache generation complete (it may take several minutes)
* Solution explorer shows the project tree.
* To build simply `CTRL+Shift+B`
* Binaries are written to `%USERPROFILE%\CMakeBuilds\silkoroutine\build` If you want to change this path simply edit `CMakeSettings.json` file.

# Examples

Usage examples of Silkoroutine framework for [gRPC](https://grpc.io/) interface bindings can be found in the [cmd](./cmd) folder: there you can find basically more than one example for each remote Turbo-Geth internal interface:

* gRPC synchronous I/F
* gRPC asynchronous I/F
* gRPC callback I/F
* gRPC coroutine I/F

# Code style

We use the standard C++20 programming language. We follow the [Google's C++ Style Guide](https://google.github.io/styleguide/cppguide.html) with the following differences:

* `snake_case` for function names.
* .cpp & .hpp file extensions are used for C++; .cc are used just for [gRPC](https://grpc.io/) generated C++; .c & .h are reserved for C.
* `using namespace foo` is allowed inside .cpp files, but not inside headers.
* Exceptions are allowed.
* User-defined literals are allowed.
* Maximum line length is 170, indentation is 4 spaces – see `.clang-format`.
