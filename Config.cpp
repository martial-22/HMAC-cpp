#include "Config.h"
#include "Codec.h"

#include <fstream>
#include <nlohmann/json.hpp>

using namespace std::literals;

namespace hmac_service {

Config::~Config() {
}

bool Config::Upload() {

    std::ifstream ifs("config.json"s);
    if (!ifs.is_open()) {
        return false;
    }

    nlohmann::json json;
    try {
        json = nlohmann::json::parse(ifs);
    }
    catch (std::exception& ) {
        return false;
    }

    auto secret_iter = json.find("secret"s);
    auto uri_iter = json.find("listen"s);
    auto max_msg_len_iter = json.find("max_msg_size_bytes"s);

    if (secret_iter == json.end() || uri_iter == json.end() || max_msg_len_iter == json.end()) {
        return false;
    }
    
    if (!max_msg_len_iter->is_number_integer()) {
        return false;
    }
    max_msg_len_ = *max_msg_len_iter;

    secret_ = *secret_iter;
    if (!Codec().ValidateBase64Url(secret_)) {
        return false;
    }

    uri_ = *uri_iter;
    return true;
}

std::string_view Config::GetSecret() const {
    return secret_;
}

std::string_view Config::GetUri() const {
    return uri_;
}

std::int32_t Config::GetMaxMessageLen() const {
    return max_msg_len_;
}

}
