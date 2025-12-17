#pragma once

#include <string>
#include <string_view>

namespace hmac_service {

class HmacService final {
public:
    HmacService(std::string_view secret);

    std::string Sign(std::string_view message);
    bool Verify(std::string_view message, std::string_view signature);

private:
    const std::string_view secret_;
};

}
