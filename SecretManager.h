#pragma once
#include <string>

class SecretManager {
public:
    void GenerateSecret(const std::string& alphabet = base64url_alphabet);

private:
    static inline const std::string base64url_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
};
