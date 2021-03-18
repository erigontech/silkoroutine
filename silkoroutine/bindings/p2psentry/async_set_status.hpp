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

#ifndef SILKOROUTINE_BINDINGS_P2PSENTRY_ASYNC_SET_STATUS_HPP_
#define SILKOROUTINE_BINDINGS_P2PSENTRY_ASYNC_SET_STATUS_HPP_

#include <asio/detail/config.hpp>
#include <asio/detail/bind_handler.hpp>
#include <asio/detail/fenced_block.hpp>
#include <asio/detail/handler_alloc_helpers.hpp>
#include <asio/detail/handler_work.hpp>
#include <asio/detail/memory.hpp>

#include <silkoroutine/bindings/async_operation.hpp>
#include <silkoroutine/bindings/p2psentry/generated/sentry.grpc.pb.h>

namespace silkoroutine::bindings::p2psentry {

template <typename Handler, typename IoExecutor>
class async_set_status : public async_operation<void, google::protobuf::Empty> {
public:
    ASIO_DEFINE_HANDLER_PTR(async_set_status);

    async_set_status(Handler& h, const IoExecutor& io_ex)
    : async_operation(&async_set_status::do_complete), handler_(ASIO_MOVE_CAST(Handler)(h)), work_(handler_, io_ex)
    {}

    static void do_complete(void* owner, async_operation* base, google::protobuf::Empty response = {}) {
        // Take ownership of the handler object.
        async_set_status* h{static_cast<async_set_status*>(base)};
        ptr p = {asio::detail::addressof(h->handler_), h, h};

        ASIO_HANDLER_COMPLETION((*h));

        // Take ownership of the operation's outstanding work.
        auto work{ASIO_MOVE_CAST2(asio::detail::handler_work<Handler, IoExecutor>)(h->work_)};

        // Make a copy of the handler so that the memory can be deallocated before
        // the upcall is made. Even if we're not about to make an upcall, a
        // sub-object of the handler may be the true owner of the memory associated
        // with the handler. Consequently, a local copy of the handler is required
        // to ensure that any owning sub-object remains valid until after we have
        // deallocated the memory here.
        asio::detail::binder1<Handler, google::protobuf::Empty> handler{h->handler_, response};
        p.h = asio::detail::addressof(handler.handler_);
        p.reset();

        // Make the upcall if required.
        if (owner) {
            asio::detail::fenced_block b(asio::detail::fenced_block::half);
            ASIO_HANDLER_INVOCATION_BEGIN((handler.arg1_));
            work.complete(handler, handler.handler_);
            ASIO_HANDLER_INVOCATION_END;
        }
    }

private:
    Handler handler_;
    asio::detail::handler_work<Handler, IoExecutor> work_;
};

} // namespace silkoroutine::bindings::p2psentry

#endif // SILKOROUTINE_BINDINGS_P2PSENTRY_ASYNC_SET_STATUS_HPP_
