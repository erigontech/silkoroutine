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

#include "util.hpp"

#include <iomanip>

namespace silkoroutine {

static std::optional<unsigned> decode_hex_digit(char ch) noexcept {
    if (ch >= '0' && ch <= '9') {
        return ch - '0';
    } else if (ch >= 'a' && ch <= 'f') {
        return ch - 'a' + 10;
    } else if (ch >= 'A' && ch <= 'F') {
        return ch - 'A' + 10;
    }
    return std::nullopt;
}

std::optional<Bytes> from_hex(std::string_view hex) noexcept {
    if (hex.length() >= 2 && hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X')) {
        hex.remove_prefix(2);
    }

    if (hex.length() % 2 != 0) {
        return std::nullopt;
    }

    Bytes out{};
    out.reserve(hex.length() / 2);

    unsigned carry{0};
    for (size_t i{0}; i < hex.size(); ++i) {
        std::optional<unsigned> v{decode_hex_digit(hex[i])};
        if (!v) {
            return std::nullopt;
        }
        if (i % 2 == 0) {
            carry = *v << 4;
        } else {
            out.push_back(static_cast<uint8_t>(carry | *v));
        }
    }

    return out;
}

std::ostream& operator<<(std::ostream& out, const ByteView& bytes) {
    for (const auto& b : bytes) {
        out << std::hex << std::setw(2) << std::setfill('0') << int(b);
    }
    out << std::dec;
    return out;
}

}  // namespace silkoroutine
