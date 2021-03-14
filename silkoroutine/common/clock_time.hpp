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

#ifndef SILKOROUTINE_COMMON_CLOCK_TIME_HPP_
#define SILKOROUTINE_COMMON_CLOCK_TIME_HPP_

#include <chrono>
#include <cstdint>

namespace silkoroutine::clock_time {

uint64_t now();
uint64_t since(uint64_t start);

} // namespace silkoroutine::clock_time

#endif  // SILKOROUTINE_COMMON_CLOCK_TIME_HPP_
