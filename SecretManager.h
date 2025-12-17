#pragma once
#include <string>

class SecretManager {
public:
    static inline const std::string base64url_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
    
    void GenerateSecret(std::string alphabet = base64url_alphabet);
};
