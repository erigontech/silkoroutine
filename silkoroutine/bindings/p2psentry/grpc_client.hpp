/*
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
*/

#ifndef SILKOROUTINE_BINDINGS_P2PSENTRY_GRPC_CLIENT_HPP_
#define SILKOROUTINE_BINDINGS_P2PSENTRY_GRPC_CLIENT_HPP_

#include <functional>
#include <memory>

#include <grpcpp/grpcpp.h>

#include <silkoroutine/bindings/p2psentry/generated/sentry.grpc.pb.h>

namespace silkoroutine::bindings::p2psentry {

class GrpcClient final : public grpc::experimental::ClientUnaryReactor {
public:
    explicit GrpcClient(std::shared_ptr<grpc::Channel> channel) : stub_{sentry::Sentry::NewStub(channel)} {}

    void set_status(const sentry::StatusData& status_data, std::function<void(grpc::Status)> callback) {
        stub_->experimental_async()->SetStatus(&context_, &status_data, &response_, callback);
    }

private:
    std::unique_ptr<sentry::Sentry::Stub> stub_;
    grpc::ClientContext context_;
    google::protobuf::Empty response_;
};

} // namespace silkoroutine::bindings::p2psentry

#endif // SILKOROUTINE_BINDINGS_P2PSENTRY_GRPC_CLIENT_HPP_
