#include <array>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <openssl/rand.h>

using namespace std::literals;

int main(int argc, char* argv[]) {

    std::fstream fs("config.json"s);
    if (!fs.is_open()) {
        std::cerr << "config.json is not found"s << std::endl;
        return 1;
    }

    nlohmann::json js;
    try {
        js = nlohmann::json::parse(fs);
    }
    catch (std::exception& e) {
        std::cerr << "Exception: "s << e.what() << " was thrown"s << std::endl;
        return 1;
    }

    std::array<unsigned char, 64> secret;
    int ret = RAND_bytes(secret.data(), secret.size());
    js["secret"s] = secret;
    
    fs << js;
    return 0;
}
