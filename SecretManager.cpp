#include "SecretManager.h"

#include <array>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <random>

using namespace std::literals;

std::string GenerateBase64URLRandomString(const std::string& alphabet, std::size_t size) {
        
    std::random_device rd;
    static std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, alphabet.size() - 1);
        
    std::string result;
    result.resize(size);
        
    for (std::size_t i = 0; i < size; ++i) {
        result[i] = alphabet[distribution(generator)];
    }
    return result;
}

void SecretManager::GenerateSecret(const std::string& alphabet) {

    std::fstream ifs("config.json"s);
    if (!ifs.is_open()) {
        std::cerr << "config.json is not found"s << std::endl;
        return;
    }

    nlohmann::json js;
    try {
        js = nlohmann::json::parse(ifs);
    }
    catch (std::exception& e) {
        std::cerr << "Exception: "s << e.what() << " was thrown"s << std::endl;
        return;
    }

    std::array<unsigned char, 64> secret;
    js["secret"s] = GenerateBase64URLRandomString(alphabet, secret.size());
        
    std::ofstream ofs("config.json"s);
    if (!ofs.is_open()) {
        std::cerr << "config.json cannot be opened for write"s << std::endl;
        return;
    }
    ofs << std::setw(4) << js << std::endl;
}
