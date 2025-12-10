#pragma once

#include <string>
#include <string_view>

namespace hmac_service {

class Config {
public:
    bool Upload();

    std::string_view GetSecret() const;
    std::string_view GetUri() const;
    std::string_view GetMaxMessageLen() const;

private:
    std::string secret_;
    std::string uri_;
    std::string max_msg_len_;
};

}
