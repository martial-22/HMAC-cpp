#include "HmacService.h"

#include <openssl/hmac.h>
#include <string>

namespace hmac_service {

HmacService::HmacService(Secret secret)
    : secret_(secret) {
}

std::string HmacService::Sign(std::string_view message) {
    unsigned int len = 0;
    unsigned char out[EVP_MAX_MD_SIZE];
    HMAC(EVP_sha256(), secret_.data, secret_.len, message.data(), message.size(), out, &len);

    std::string result;
    result.reserve(len);

    std::copy(std::begin(out), std::end(out), std::back_inserter(result));
    return result;
}

bool HmacService::Verify(std::string_view message, std::string_view signature) {
    const std::string actual_signature = Sign(message);

    if (actual_signature.size() != signature.size()) {
        return false;
    }

    bool result = false;
    for (std::size_t i = 0; i < signature.size(); ++i) {
        result |= signature[i] ^ actual_signature[i];
    }
    return result == false;
}

}
