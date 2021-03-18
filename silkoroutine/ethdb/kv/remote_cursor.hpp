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

#ifndef SILKOROUTINE_KV_REMOTE_CURSOR_H_
#define SILKOROUTINE_KV_REMOTE_CURSOR_H_

#include <silkoroutine/config.hpp>

#include <memory>

#include <asio/awaitable.hpp>
#include <asio/io_context.hpp>
#include <asio/use_awaitable.hpp>

#include <silkoroutine/common/clock_time.hpp>
#include <silkoroutine/common/log.hpp>
#include <silkoroutine/common/util.hpp>
#include <silkoroutine/bindings/kv/awaitables.hpp>
#include <silkoroutine/ethdb/kv/cursor.hpp>

namespace silkoroutine::ethdb::kv {

class RemoteCursor : public Cursor {
public:
    RemoteCursor(bindings::kv::KvAsioAwaitable<asio::io_context::executor_type>& kv_awaitable)
    : kv_awaitable_(kv_awaitable), cursor_id_{0} {}

    RemoteCursor(const RemoteCursor&) = delete;
    RemoteCursor& operator=(const RemoteCursor&) = delete;

    uint32_t cursor_id() const override { return cursor_id_; };

    asio::awaitable<KeyValue> seek(const std::string& table_name, const Bytes& seek_key) override {
        co_await open_cursor(table_name);
        auto kv_pair = co_await seek(seek_key);
        co_await close_cursor();
        co_return kv_pair;
    }

    asio::awaitable<void> open_cursor(const std::string& table_name) override {
        const auto start_time = clock_time::now();
        if (cursor_id_ == 0) {
            cursor_id_ = co_await kv_awaitable_.async_open_cursor(table_name, asio::use_awaitable);
            SILK_TRACE << "RemoteCursor::open_cursor cursor: " << cursor_id_ << " for table: " << table_name << "\n";
        }
        SILK_DEBUG << "RemoteCursor::open_cursor t=" << clock_time::since(start_time) << "\n";
        co_return;
    }

    asio::awaitable<KeyValue> seek(const Bytes& seek_key) override {
        const auto start_time = clock_time::now();
        auto seek_pair = co_await kv_awaitable_.async_seek(cursor_id_, seek_key, asio::use_awaitable);
        const auto k = bytes_of_string(seek_pair.k());
        const auto v = bytes_of_string(seek_pair.v());
        SILK_DEBUG << "RemoteCursor::seek t=" << clock_time::since(start_time) << "\n";
        co_return KeyValue{k, v};
    }

    asio::awaitable<KeyValue> next() override {
        const auto start_time = clock_time::now();
        auto next_pair = co_await kv_awaitable_.async_next(cursor_id_, asio::use_awaitable);
        const auto k = bytes_of_string(next_pair.k());
        const auto v = bytes_of_string(next_pair.v());
        SILK_DEBUG << "RemoteCursor::next t=" << clock_time::since(start_time) << "\n";
        co_return KeyValue{k, v};
    }

    asio::awaitable<void> close_cursor() override {
        const auto start_time = clock_time::now();
        if (cursor_id_ != 0) {
            co_await kv_awaitable_.async_close_cursor(cursor_id_, asio::use_awaitable); // Can we shoot and forget?
            SILK_TRACE << "RemoteCursor::close_cursor cursor: " << cursor_id_ << "\n";
            cursor_id_ = 0;
        }
        SILK_DEBUG << "RemoteCursor::close_cursor t=" << clock_time::since(start_time) << "\n";
        co_return;
    }

private:
    bindings::kv::KvAsioAwaitable<asio::io_context::executor_type>& kv_awaitable_;
    uint32_t cursor_id_;
};

} // namespace silkoroutine::ethdb::kv

#endif  // SILKOROUTINE_KV_REMOTE_CURSOR_H_
