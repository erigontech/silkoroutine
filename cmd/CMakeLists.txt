#[[
   Copyright 2021 The Silkoroutine Authors

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
]]

if(MSVC)
  add_link_options(/STACK:10000000)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  add_link_options(-Wl,-stack_size -Wl,0x1000000)
endif()

find_package(gRPC CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)

add_compile_options(-std=c++2a)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-fcoroutines)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-stdlib=libc++)
endif()

# KV interface tools

add_executable(kv_seek_async_asio kv_seek_async_asio.cpp)
target_include_directories(kv_seek_async_asio PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(kv_seek_async_asio absl::flags_parse gRPC::grpc++_unsecure protobuf::libprotobuf silkoroutine)

add_executable(kv_seek_async_callback kv_seek_async_callback.cpp)
target_include_directories(kv_seek_async_callback PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(kv_seek_async_callback absl::flags_parse gRPC::grpc++_unsecure protobuf::libprotobuf silkoroutine)

add_executable(kv_seek_async_coroutines kv_seek_async_coroutines.cpp)
target_include_directories(kv_seek_async_coroutines PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(kv_seek_async_coroutines absl::flags_parse gRPC::grpc++_unsecure protobuf::libprotobuf silkoroutine)

add_executable(kv_seek_async kv_seek_async.cpp)
target_include_directories(kv_seek_async PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(kv_seek_async absl::flags_parse gRPC::grpc++_unsecure protobuf::libprotobuf silkoroutine)

add_executable(kv_seek kv_seek.cpp)
target_include_directories(kv_seek PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(kv_seek absl::flags_parse gRPC::grpc++_unsecure protobuf::libprotobuf silkoroutine)

# SENTRY interface tools

add_executable(sentry_set_status sentry_set_status.cpp)
target_include_directories(sentry_set_status PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(sentry_set_status absl::flags_parse gRPC::grpc++_unsecure protobuf::libprotobuf silkoroutine)

add_executable(sentry_set_status_async sentry_set_status_async.cpp)
target_include_directories(sentry_set_status_async PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(sentry_set_status_async absl::flags_parse gRPC::grpc++_unsecure protobuf::libprotobuf silkoroutine)

# Unit tests
enable_testing()

find_package(Catch2 CONFIG REQUIRED)

file(GLOB_RECURSE silkoroutine_TESTS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/silkoroutine/*_test.cpp")
add_executable(unit_test unit_test.cpp ${silkoroutine_TESTS})
target_link_libraries(unit_test silkoroutine Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(unit_test)
