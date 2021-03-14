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

#ifndef SILKOROUTINE_COMMON_CONSTANTS_HPP_
#define SILKOROUTINE_COMMON_CONSTANTS_HPP_

#include <chrono>

namespace silkoroutine {

constexpr const char* kDefaultTarget{"localhost:9090"};
constexpr const std::chrono::milliseconds kDefaultTimeout{10000};

}  // namespace silkoroutine

#endif  // SILKOROUTINE_COMMON_CONSTANTS_HPP_
