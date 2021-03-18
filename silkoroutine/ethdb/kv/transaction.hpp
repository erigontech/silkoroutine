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

#ifndef SILKOROUTINE_ETHDB_KV_TRANSACTION_HPP_
#define SILKOROUTINE_ETHDB_KV_TRANSACTION_HPP_

#include <memory>
#include <string>

#include <silkoroutine/config.hpp>

#include <asio/awaitable.hpp>

#include <silkoroutine/common/util.hpp>
#include <silkoroutine/ethdb/kv/cursor.hpp>

namespace silkoroutine::ethdb::kv {

class Transaction {
public:
    Transaction() = default;

    Transaction(const Transaction&) = delete;
    Transaction& operator=(const Transaction&) = delete;

    virtual ~Transaction() = default;

    virtual std::unique_ptr<Cursor> cursor() = 0;

    virtual asio::awaitable<std::shared_ptr<Cursor>> cursor(const std::string& table) = 0;

    virtual asio::awaitable<void> close() = 0;
};

} // namespace silkoroutine::ethdb::kv

#endif  // SILKOROUTINE_ETHDB_KV_TRANSACTION_HPP_
