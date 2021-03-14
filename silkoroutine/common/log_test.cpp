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

#include <iostream>
#include <regex>
#include <sstream>
#include <string>

#include <catch2/catch.hpp>

namespace silkoroutine {

namespace {
    std::ostringstream stream1, stream2;
    const std::string kInfix(R"(\[\d\d-\d\d|\d\d:\d\d:\d\d\.\d{3}\] )");

    bool test_log(std::string prefix, std::string infix, std::string suffix) {
        std::string string1(stream1.str());
        std::string string2(stream2.str());
        stream1.clear();
        stream1.str("");
        stream2.clear();
        stream2.str("");
        if (string1 != string2) return false;

        const std::string pattern = prefix + infix + suffix;
        const std::regex rx(pattern);
        return std::regex_search(string1, rx);
    }
} // namespace

TEST_CASE("Logging") {
    SILK_LOG_STREAMS(stream1, stream2);

    // test true branch of macro
    SILK_LOG_VERBOSITY(LogTrace);
    SILK_LOG_AS(LogCritical) << "LogCritical" << std::endl;
    CHECK(test_log("CRIT ", kInfix, "LogCritical"));
    SILK_LOG_AS(LogError) << "LogError" << std::endl;
    CHECK(test_log("ERROR", kInfix, "LogError"));
    SILK_LOG_AS(LogWarn) << "LogWarn" << std::endl;
    CHECK(test_log("WARN ", kInfix, "LogWarn"));
    SILK_LOG_AS(LogInfo) << "LogInfo" << std::endl;
    CHECK(test_log("INFO ", kInfix, "LogInfo"));
    SILK_LOG_AS(LogDebug) << "LogDebug" << std::endl;
    CHECK(test_log("DEBUG", kInfix, "LogDebug"));
    SILK_LOG_AS(LogTrace) << "LogTrace" << std::endl;
    CHECK(test_log("TRACE", kInfix, "LogTrace"));

    // test false branch of macro
    SILK_LOG_VERBOSITY(LogDebug);
    SILK_LOG_AS(LogTrace) << "LogTrace" << std::endl;
    CHECK(test_log("", "", ""));
}

} // namespace silkoroutine
