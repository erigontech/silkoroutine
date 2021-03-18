/*
    Copyright 2020 The Silkoroutine Authors

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

#ifndef SILKOROUTINE_KV_REMOTE_TRANSACTION_HPP
#define SILKOROUTINE_KV_REMOTE_TRANSACTION_HPP

#include <map>
#include <memory>
#include <string>

#include <silkoroutine/config.hpp>

#include <asio/use_awaitable.hpp>

#include <silkoroutine/bindings/kv/awaitables.hpp>
#include <silkoroutine/bindings/kv/grpc_client_reactor.hpp>
#include <silkoroutine/ethdb/kv/cursor.hpp>
#include <silkoroutine/ethdb/kv/remote_cursor.hpp>
#include <silkoroutine/ethdb/kv/transaction.hpp>

namespace silkoroutine::ethdb::kv {

class RemoteTransaction : public Transaction {
public:
    explicit RemoteTransaction(asio::io_context& context, std::shared_ptr<grpc::Channel> channel)
    : context_(context), reactor_{channel}, kv_awaitable_{context_, reactor_} {}

    std::unique_ptr<Cursor> cursor() override {
        return std::make_unique<RemoteCursor>(kv_awaitable_);
    }

    asio::awaitable<std::shared_ptr<Cursor>> cursor(const std::string& table) override;

    asio::awaitable<void> close() override;

private:
    asio::io_context& context_;
    bindings::kv::GrpcClientReactor reactor_;
    bindings::kv::GrpcAsioAwaitable<asio::io_context::executor_type> kv_awaitable_;
    std::map<std::string, std::shared_ptr<Cursor>> cursors_;
};

} // namespace silkoroutine::ethdb::kv

#endif // SILKOROUTINE_KV_REMOTE_TRANSACTION_HPP
