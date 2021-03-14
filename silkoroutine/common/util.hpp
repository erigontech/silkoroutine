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

#ifndef SILKOROUTINE_COMMON_UTIL_HPP_
#define SILKOROUTINE_COMMON_UTIL_HPP_

#include <iostream>
#include <optional>
#include <string>

#include "base.hpp"

namespace silkoroutine {

std::optional<Bytes> from_hex(std::string_view hex) noexcept;

std::ostream& operator<<(std::ostream& out, const ByteView& bytes);

inline ByteView byte_view_of_string(const std::string& s) {
    return {reinterpret_cast<const uint8_t*>(s.data()), s.length()};
}

inline Bytes bytes_of_string(const std::string& s) {
    return Bytes(s.begin(), s.end());
}

} // namespace silkoroutine

#endif  // SILKOROUTINE_COMMON_UTIL_HPP_
