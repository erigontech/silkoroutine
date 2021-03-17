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

# Find Protobuf installation
set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
find_program(PROTOBUF_PROTOC protoc REQUIRED)

# Find gRPC installation
find_package(gRPC CONFIG REQUIRED)
find_program(GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin REQUIRED)

# gRPC proto file
set(SENTRY_PROTO "${CMAKE_SOURCE_DIR}/interfaces/p2psentry/sentry.proto")
get_filename_component(SENTRY_PROTO_PATH "${SENTRY_PROTO}" PATH)

# Generate sources
set(SENTRY_PROTO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/sentry.pb.cc")
set(SENTRY_PROTO_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/sentry.pb.h")
set(SENTRY_GRPC_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/sentry.grpc.pb.cc")
set(SENTRY_GRPC_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/sentry.grpc.pb.h")

add_custom_command(
    OUTPUT "${SENTRY_PROTO_SOURCES}" "${SENTRY_PROTO_HEADERS}" "${SENTRY_GRPC_SOURCES}" "${SENTRY_GRPC_HEADERS}"
    COMMAND ${PROTOBUF_PROTOC}
    ARGS --grpc_out "${CMAKE_CURRENT_SOURCE_DIR}"
      --cpp_out "${CMAKE_CURRENT_SOURCE_DIR}"
      -I "${SENTRY_PROTO_PATH}"
      --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN_EXECUTABLE}"
      "${SENTRY_PROTO}"
    DEPENDS "${SENTRY_PROTO}"
)

add_custom_target(
    generate_sentry_grpc
    DEPENDS "${SENTRY_PROTO_SOURCES}" "${SENTRY_PROTO_HEADERS}" "${SENTRY_GRPC_SOURCES}" "${SENTRY_GRPC_HEADERS}"
)
