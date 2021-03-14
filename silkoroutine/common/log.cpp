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

#include "log.hpp"

#include <absl/strings/str_cat.h>
#include <absl/time/clock.h>

namespace silkoroutine {

bool AbslParseFlag(absl::string_view text, LogLevels* level, std::string* error) {
    if (text == "n") {
        *level = LogLevels::LogNone;
        return true;
    }
    if (text == "c") {
        *level = LogLevels::LogCritical;
        return true;
    }
    if (text == "e") {
        *level = LogLevels::LogError;
        return true;
    }
    if (text == "w") {
        *level = LogLevels::LogWarn;
        return true;
    }
    if (text == "i") {
        *level = LogLevels::LogInfo;
        return true;
    }
    if (text == "d") {
        *level = LogLevels::LogDebug;
        return true;
    }
    if (text == "t") {
        *level = LogLevels::LogTrace;
        return true;
    }
    *error = "unknown value for silkrpc::LogLevel";
    return false;
}

std::string AbslUnparseFlag(LogLevels level) {
    switch (level) {
        case LogLevels::LogNone: return "n";
        case LogLevels::LogCritical: return "c";
        case LogLevels::LogError: return "e";
        case LogLevels::LogWarn: return "w";
        case LogLevels::LogInfo: return "i";
        case LogLevels::LogDebug: return "d";
        case LogLevels::LogTrace: return "t";
        default: return absl::StrCat(level);
    }
}

namespace detail {

    teestream log_streams_{std::cerr, null_stream()};

    LogLevels log_verbosity_{LogInfo};

    constexpr char const kLogTags_[7][6] = { "TRACE", "DEBUG", "INFO ", "WARN ", "ERROR", "CRIT ", "NONE ", };

    // Log to one or two output streams - typically the console and optional log file.
    void log_set_streams_(std::ostream& o1, std::ostream& o2) {
        log_streams_.set_streams(o1.rdbuf(), o2.rdbuf());
    }

    std::mutex log_::log_mtx_;

    std::ostream& log_::header_(LogLevels level) {
        return
            log_streams_
                << kLogTags_[level]
                << "["
                << absl::FormatTime("%m-%d|%H:%M:%E3S", absl::Now(), absl::LocalTimeZone())
                << "] ";
    }

    void log_expand_and_compile_test_() {
        SILK_LOG_AS(LogInfo) << "log_expand_and_compile_test_" << std::endl;
    }

}  // namespace detail

std::ostream& null_stream() {
    static struct null_buf : public std::streambuf {
        int overflow(int c) override { return c; }
    } null_buf;
    static struct null_strm : public std::ostream {
        null_strm() : std::ostream(&null_buf) {}
    } null_strm;
    return null_strm;
}

}  // namespace silkoroutine
