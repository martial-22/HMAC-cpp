#include "Codec.h"

#include <algorithm>
#include <openssl/evp.h>

namespace hmac_service {

std::string Codec::ToBase64Url(const unsigned char* data, std::size_t len) {
    
    static constexpr int bytes_in_symbol = 4;
    static constexpr int bytes_in_encoded = 3;
    static constexpr int padding = 2;

    std::string out(bytes_in_symbol * ((len + padding) / bytes_in_encoded), '\0');
    int n = EVP_EncodeBlock(reinterpret_cast<unsigned char*>(&out[0]), data, static_cast<int>(len));
    out.resize(n);

    for (char& c : out) {
        if (c == '+') {
            c = '-'; 
        }
        else if (c == '/') {
            c = '_';
        }
    }
    while (!out.empty() && out.back() == '=') {
        out.pop_back();
    }
    return out;
}

bool Codec::ValidateBase64Url(std::string_view str) {

    return std::all_of(str.begin(), str.end(), [](char c) { 
        const bool is_number = '0' <= c && c <= '9';
        const bool is_small_letter = 'a' <= c && c <= 'z';
        const bool is_big_letter = 'A' <= c && c <= 'Z';
        const bool is_special_symbol = '-' == c || c == '_';

        return is_number || is_small_letter || is_big_letter || is_special_symbol;
    });
}

}
