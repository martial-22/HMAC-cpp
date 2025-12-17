#pragma once

#include <string>
#include <string_view>

namespace hmac_service {

class Codec {
public:
    std::string ToBase64Url(const unsigned char* data, std::size_t len);
    bool ValidateBase64Url(std::string_view str);
};

}
