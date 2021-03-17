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

#include <iomanip>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include <absl/flags/flag.h>
#include <absl/flags/parse.h>
#include <absl/flags/usage.h>
#include <grpcpp/grpcpp.h>

#include <silkoroutine/common/constants.hpp>
#include <silkoroutine/common/util.hpp>
#include <silkoroutine/bindings/p2psentry/generated/sentry.grpc.pb.h>

struct ChainConfig {
    const char* genesis_hash{nullptr};

    // https://eips.ethereum.org/EIPS/eip-155
    uint64_t chain_id{0};

    // https://eips.ethereum.org/EIPS/eip-606
    std::optional<uint64_t> homestead_block;

    // https://eips.ethereum.org/EIPS/eip-608
    // https://ecips.ethereumclassic.org/ECIPs/ecip-1015
    std::optional<uint64_t> tangerine_whistle_block;

    // TODO(ETC) EIP-160 was applied to ETC before the rest of Spurious Dragon; see
    // https://ecips.ethereumclassic.org/ECIPs/ecip-1066

    // https://eips.ethereum.org/EIPS/eip-607
    // https://ecips.ethereumclassic.org/ECIPs/ecip-1054
    std::optional<uint64_t> spurious_dragon_block;

    // https://eips.ethereum.org/EIPS/eip-609
    // https://ecips.ethereumclassic.org/ECIPs/ecip-1054
    std::optional<uint64_t> byzantium_block;

    // https://eips.ethereum.org/EIPS/eip-1013
    // https://ecips.ethereumclassic.org/ECIPs/ecip-1056
    std::optional<uint64_t> constantinople_block;

    // https://eips.ethereum.org/EIPS/eip-1716
    // https://ecips.ethereumclassic.org/ECIPs/ecip-1056
    std::optional<uint64_t> petersburg_block;

    // https://eips.ethereum.org/EIPS/eip-1679
    // https://ecips.ethereumclassic.org/ECIPs/ecip-1088
    std::optional<uint64_t> istanbul_block;

    // https://eips.ethereum.org/EIPS/eip-2387
    std::optional<uint64_t> muir_glacier_block;

    // https://github.com/ethereum/eth1.0-specs/blob/master/network-upgrades/berlin.md
    std::optional<uint64_t> berlin_block;

    // https://eips.ethereum.org/EIPS/eip-779
    std::optional<uint64_t> dao_block;

    // TODO(ETC) ECIP-1017

    bool has_homestead(uint64_t block_num) const noexcept {
        return homestead_block.has_value() && homestead_block <= block_num;
    }

    bool has_tangerine_whistle(uint64_t block_num) const noexcept {
        return tangerine_whistle_block.has_value() && tangerine_whistle_block <= block_num;
    }

    bool has_spurious_dragon(uint64_t block_num) const noexcept {
        return spurious_dragon_block.has_value() && spurious_dragon_block <= block_num;
    }

    bool has_byzantium(uint64_t block_num) const noexcept {
        return byzantium_block.has_value() && byzantium_block <= block_num;
    }

    bool has_constantinople(uint64_t block_num) const noexcept {
        return constantinople_block.has_value() && constantinople_block <= block_num;
    }

    bool has_petersburg(uint64_t block_num) const noexcept {
        return petersburg_block.has_value() && petersburg_block <= block_num;
    }

    bool has_istanbul(uint64_t block_num) const noexcept {
        return istanbul_block.has_value() && istanbul_block <= block_num;
    }

    bool has_muir_glacier(uint64_t block_num) const noexcept {
        return muir_glacier_block.has_value() && muir_glacier_block <= block_num;
    }

    bool has_berlin(uint64_t block_num) const noexcept { return berlin_block.has_value() && berlin_block <= block_num; }
};

constexpr ChainConfig kMainnetConfig{
    "d4e56740f876aef8c010b86a40d5f56745a118d0906a34e69aec8c0db1cb8fa3", // genesis hash

    1,  // chain_id

    1'150'000,   // homestead_block
    2'463'000,   // tangerine_whistle_block
    2'675'000,   // spurious_dragon_block
    4'370'000,   // byzantium_block
    7'280'000,   // constantinople_block
    7'280'000,   // petersburg_block
    9'069'000,   // istanbul_block
    9'200'000,   // muir_glacier_block
    12'244'000,  // berlin_block

    1'920'000,  // dao_block
};

constexpr ChainConfig kRopstenConfig{
    "0x", // genesis hash

    3,  // chain_id

    0,          // homestead_block
    0,          // tangerine_whistle_block
    10,         // spurious_dragon_block
    1'700'000,  // byzantium_block
    4'230'000,  // constantinople_block
    4'939'394,  // petersburg_block
    6'485'846,  // istanbul_block
    7'117'117,  // muir_glacier_block
    9'812'189,  // berlin_block
};

constexpr ChainConfig kGoerliConfig{
    "0x", // genesis hash

    5,  // chain_id

    0,             // homestead_block
    0,             // tangerine_whistle_block
    0,             // spurious_dragon_block
    0,             // byzantium_block
    0,             // constantinople_block
    0,             // petersburg_block
    1'561'651,     // istanbul_block
    std::nullopt,  // muir_glacier_block
    4'460'644,     // berlin_block
};

// https://ecips.ethereumclassic.org/ECIPs/ecip-1066
constexpr ChainConfig kClassicMainnetConfig{
    "0x", // genesis hash

    61,  // chain_id

    1'150'000,   // homestead_block
    2'500'000,   // tangerine_whistle_block
    8'772'000,   // spurious_dragon_block
    8'772'000,   // byzantium_block
    9'573'000,   // constantinople_block
    9'573'000,   // petersburg_block
    10'500'839,  // istanbul_block
};

const ChainConfig* lookup_chain_config(uint64_t chain_id) noexcept {
    switch (chain_id) {
        case kMainnetConfig.chain_id:
            return &kMainnetConfig;
        case kRopstenConfig.chain_id:
            return &kRopstenConfig;
        case kGoerliConfig.chain_id:
            return &kGoerliConfig;
        case kClassicMainnetConfig.chain_id:
            return &kClassicMainnetConfig;
        default:
            return nullptr;
    }
}

ABSL_FLAG(uint64_t, chainid, kMainnetConfig.chain_id, "chain ID as integer");
ABSL_FLAG(std::string, target, "localhost:9091", "P2PSentry location as string <address>:<port>");
ABSL_FLAG(std::string, blockHash, "", "head block hash as string starting with 0x");
ABSL_FLAG(std::string, totalDifficulty, "", "total difficulty as string starting with 0x");

int main(int argc, char* argv[]) {
    absl::SetProgramUsageMessage("Set Status on P2PSentry using Sentry remote interface");
    absl::ParseCommandLine(argc, argv);

    auto chain_id{absl::GetFlag(FLAGS_chainid)};
    const auto chain_config = lookup_chain_config(chain_id);
    if (chain_config == nullptr) {
        std::cerr << "Parameter chainid is invalid: [" << chain_id << "]\n";
        std::cerr << "Use --chainid flag to specify the chain ID\n";
        return -1;
    }

    const auto target{absl::GetFlag(FLAGS_target)};
    if (target.empty() || target.find(":") == std::string::npos) {
        std::cerr << "Parameter target is invalid: [" << target << "]\n";
        std::cerr << "Use --target flag to specify the location of P2PSentry running instance\n";
        return -1;
    }

    const auto head_hash{absl::GetFlag(FLAGS_blockHash)};
    if (head_hash.empty()) {
        std::cerr << "Parameter blockHash is invalid: [" << head_hash << "]\n";
        std::cerr << "Use --blockHash flag to specify the head block hash as string starting w/o 0x\n";
        return -1;
    }

    const auto total_difficulty{absl::GetFlag(FLAGS_totalDifficulty)};
    if (total_difficulty.empty()) {
        std::cerr << "Parameter totalDifficulty is invalid: [" << total_difficulty << "]\n";
        std::cerr << "Use --totalDifficulty flag to specify the total difficulty as string w/o 0x\n";
        return -1;
    }

    // Build list of hard-fork blocks
    std::vector<std::optional<uint64_t>> hard_forks;
    hard_forks.push_back(chain_config->homestead_block);
    hard_forks.push_back(chain_config->tangerine_whistle_block);
    hard_forks.push_back(chain_config->spurious_dragon_block);
    hard_forks.push_back(chain_config->byzantium_block);
    hard_forks.push_back(chain_config->constantinople_block);
    hard_forks.push_back(chain_config->petersburg_block);
    hard_forks.push_back(chain_config->istanbul_block);
    hard_forks.push_back(chain_config->muir_glacier_block);
    hard_forks.push_back(chain_config->berlin_block);

    // Create Sentry stub using insecure channel to target
    grpc::ClientContext context;

    const auto channel = grpc::CreateChannel(target, grpc::InsecureChannelCredentials());
    const auto stub = sentry::Sentry::NewStub(channel);

    sentry::StatusData request;
    request.set_network_id(chain_id);
    request.set_total_difficulty(total_difficulty);
    request.set_best_hash(head_hash.data(), head_hash.size());
    auto forks = new sentry::Forks{};
    forks->set_genesis(chain_config->genesis_hash, std::strlen(chain_config->genesis_hash));
    for (std::optional<uint64_t> block : hard_forks) {
        forks->add_forks(block.value());
    }
    request.set_allocated_fork_data(forks); // take ownership

    // Send SetStatus message to Sentry
    google::protobuf::Empty response;
    std::cout << "SENTRY SetStatus -> chain_id: " << chain_id << " head_hash: " << head_hash << "\n";
    grpc::Status result = stub->SetStatus(&context, request, &response);
    std::cout << "SENTRY SetStatus <- result ok: " << std::boolalpha << result.ok() << "\n";
    if (!result.ok()) {
        std::cout << "SENTRY SetStatus <- result error_code: " << result.error_code() << "\n";
        std::cout << "SENTRY SetStatus <- result error_message: " << result.error_message() << "\n";
        std::cout << "SENTRY SetStatus <- result error_details: " << result.error_details() << "\n";
        return 1;
    }

    return 0;
}
