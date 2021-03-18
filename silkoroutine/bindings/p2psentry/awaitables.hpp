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

#ifndef SILKOROUTINE_BINDINGS_P2PSENTRY_AWAITABLES_HPP_
#define SILKOROUTINE_BINDINGS_P2PSENTRY_AWAITABLES_HPP_

#include <silkoroutine/config.hpp>

#include <functional>
#include <string>
#include <system_error>
#include <thread>

#include <asio/async_result.hpp>
#include <asio/detail/non_const_lvalue.hpp>
#include <asio/error.hpp>
#include <asio/io_context.hpp>
#include <asio/post.hpp>
#include <grpcpp/grpcpp.h>

#include <silkoroutine/common/util.hpp>
#include <silkoroutine/bindings/p2psentry/async_set_status.hpp>
#include <silkoroutine/bindings/p2psentry/grpc_client.hpp>
#include <silkoroutine/bindings/p2psentry/generated/sentry.grpc.pb.h>

namespace silkoroutine::bindings::p2psentry {

template<typename Executor>
struct GrpcAsioAwaitable;

template<typename Executor>
class initiate_async_set_status {
public:
    typedef Executor executor_type;

    explicit initiate_async_set_status(GrpcAsioAwaitable<Executor>* self, const sentry::StatusData& status_data)
    : self_(self), status_data_(status_data) {}

    executor_type get_executor() const noexcept { return self_->get_executor(); }

    template <typename WaitHandler>
    void operator()(WaitHandler&& handler) {
        ASIO_WAIT_HANDLER_CHECK(WaitHandler, handler) type_check;

        asio::detail::non_const_lvalue<WaitHandler> handler2(handler);
        typedef async_set_status<WaitHandler, Executor> op;
        typename op::ptr p = {asio::detail::addressof(handler2.value), op::ptr::allocate(handler2.value), 0};
        wrapper_ = new op(handler2.value, self_->context_.get_executor());

        self_->client_.set_status(status_data, [this](grpc::Status status) {
            if (!status.ok()) {
                throw std::system_error{std::make_error_code(std::errc::io_error), "failure in SET_STATUS"};
            }
            typedef async_set_status<WaitHandler, Executor> op;
            auto set_status_op = static_cast<op*>(wrapper_);

            // Make the io_context thread execute the operation completion
            self_->context_.post([this, set_status_op]() {
                set_status_op->complete(this, {});
            });
        });
    }

private:
    GrpcAsioAwaitable<Executor>* self_;
    const sentry::StatusData& status_data_;
    void* wrapper_;
};

template<typename Executor>
struct GrpcAsioAwaitable {
    typedef Executor executor_type;

    explicit GrpcAsioAwaitable(asio::io_context& context, GrpcClient& client)
    : context_(context), client_(client) {}

    template<typename WaitHandler>
    auto async_set_status(const sentry::StatusData& status_data, WaitHandler&& handler) {
        return asio::async_initiate<WaitHandler, void(google::protobuf::Empty)>(initiate_async_set_status{this, status_data}, handler);
    }

    asio::io_context& context_;
    GrpcClient& client_;
};

} // namespace silkoroutine::bindings::p2psentry

#endif // SILKOROUTINE_BINDINGS_P2PSENTRY_AWAITABLES_HPP_
