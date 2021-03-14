/*
   Copyright 2020 - 2021 The Silkoroutine Authors

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

#ifndef SILKOROUTINE_COMMON_LOG_HPP_
#define SILKOROUTINE_COMMON_LOG_HPP_

#include <absl/strings/string_view.h>

#include <mutex>
#include <string>

#include "tee.hpp"

namespace silkoroutine {

enum LogLevels { LogTrace, LogDebug, LogInfo, LogWarn, LogError, LogCritical, LogNone };

bool AbslParseFlag(absl::string_view text, LogLevels* level, std::string* error);
std::string AbslUnparseFlag(LogLevels level);

#define SILK_TRACE SILK_LOG_AS(LogTrace)
#define SILK_DEBUG SILK_LOG_AS(LogDebug)
#define SILK_INFO  SILK_LOG_AS(LogInfo)
#define SILK_WARN  SILK_LOG_AS(LogWarn)
#define SILK_ERROR SILK_LOG_AS(LogError)
#define SILK_CRIT  SILK_LOG_AS(LogCritical)
#define SILK_LOG   SILK_LOG_AS(LogNone)

#define SILK_LOG_AS(level_) if ((level_) < detail::log_verbosity_) {} else detail::log_(level_) // NOLINT

// change the logging verbosity level - default level is LogInfo
//
#define SILK_LOG_VERBOSITY(level_) (detail::log_verbosity_ = (level_))

// change the logging output streams - default is (cerr, null_stream())
//
#define SILK_LOG_STREAMS(stream1_, stream2_) detail::log_set_streams_((stream1_), (stream2_));

// silence
std::ostream& null_stream();

//
// Below are for access via macros ONLY :(
//
namespace detail {

extern LogLevels log_verbosity_;

void log_set_streams_(std::ostream & o1, std::ostream & o2);

class log_ {
public:
    explicit log_(LogLevels level) : level_(level) { log_mtx_.lock(); }
    ~log_() { log_mtx_.unlock(); }

    std::ostream& header_(LogLevels);

    template <class T> std::ostream& operator<< (const T & message) {
        return header_(level_) << message;
    }

private:
    LogLevels level_;
    static std::mutex log_mtx_;
};

}  // namespace namespace detail

}  // namespace silkoroutine

#endif // SILKOROUTINE_COMMON_LOG_HPP_
