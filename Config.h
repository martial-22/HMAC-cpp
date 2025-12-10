#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace hmac_service {

class Config {
public:
    ~Config();

    bool Upload();

    std::string_view GetSecret() const;
    std::string_view GetUri() const;
    std::int32_t GetMaxMessageLen() const;

private:
    std::string secret_;
    std::string uri_;
    std::int32_t max_msg_len_;
};

}
