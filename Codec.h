#pragma once

#include <string>

namespace hmac_service {

class Codec {
public:
    std::string ToBase64Url(const unsigned char* data, std::size_t len);
};

}
