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

#ifndef SILKOROUTINE_BINDINGS_KV_GRPC_CLIENT_REACTOR_HPP
#define SILKOROUTINE_BINDINGS_KV_GRPC_CLIENT_REACTOR_HPP

#include <functional>
#include <memory>

#include <grpcpp/grpcpp.h>

#include <silkoroutine/bindings/kv/generated/kv.grpc.pb.h>

namespace silkoroutine::bindings::kv {

class GrpcClientReactor final : public grpc::experimental::ClientBidiReactor<remote::Cursor, remote::Pair> {
public:
    explicit GrpcClientReactor(std::shared_ptr<grpc::Channel> channel) : stub_{remote::KV::NewStub(channel)} {
        stub_->experimental_async()->Tx(&context_, this);
        StartCall();
    }

    void close_start(std::function<void(bool)> close_completed) {
        close_completed_ = close_completed;
        StartWritesDone();
        //context_.TryCancel();
    }

    void read_start(std::function<void(bool,remote::Pair)> read_completed) {
        read_completed_ = read_completed;
        StartRead(&pair_);
    }

    void write_start(remote::Cursor* cursor, std::function<void(bool)> write_completed) {
        write_completed_ = write_completed;
        StartWrite(cursor);
    }

    void OnReadDone(bool ok) override {
        read_completed_(ok, pair_);
    }

    void OnWriteDone(bool ok) override {
        write_completed_(ok);
    }

    void OnDone(const ::grpc::Status& status) override {
        close_completed_(status.ok());
    }

private:
    std::unique_ptr<remote::KV::Stub> stub_;
    grpc::ClientContext context_;
    remote::Pair pair_;
    std::function<void(bool,remote::Pair)> read_completed_;
    std::function<void(bool)> write_completed_;
    std::function<void(bool)> close_completed_;
};

} // namespace silkoroutine::bindings::kv

#endif // SILKOROUTINE_BINDINGS_KV_GRPC_CLIENT_REACTOR_HPP
