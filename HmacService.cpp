#include "HmacService.h"

#include <array>
#include <openssl/hmac.h>

namespace hmac_service {

HmacService::HmacService(std::string secret)
    : secret_(std::move(secret)) {
}

std::string HmacService::Sign(std::string_view message) {

    const void* key = reinterpret_cast<const void*>(secret_.data());
    std::size_t key_len = 0;
    if (!secret_.empty()) {
        key_len = secret_.size() * sizeof(secret_.front()) / sizeof(unsigned char);
    }

    const unsigned char* data = reinterpret_cast<const unsigned char*>(message.data());
    std::size_t data_len = 0;
    if (!message.empty()) {
        data_len = message.size() * sizeof(message.front()) / sizeof(unsigned char);
    }

    std::array<unsigned char, EVP_MAX_MD_SIZE> out;
    unsigned int out_len = 0;
    
    HMAC(EVP_sha256(), key, key_len, data, data_len, out.data(), &out_len);
    return std::string(out.begin(), std::next(out.begin(), out_len));
}

bool HmacService::Verify(std::string_view message, std::string_view signature) {
    const std::string actual_signature = Sign(message);
    
    bool result = actual_signature.size() != signature.size();
    for (std::size_t i = 0; i < signature.size(); ++i) {
        result = result || signature[i] ^ actual_signature[i];
    }
    return result == 0;
}

}
