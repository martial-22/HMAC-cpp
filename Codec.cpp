#include "Codec.h"

#include <openssl/evp.h>

namespace hmac_service {

std::string Codec::ToBase64Url(const unsigned char* data, std::size_t len) {
    
    std::string out(4 * ((len + 2) / 3), '\0');
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

}
