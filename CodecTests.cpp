#include "Codec.h"

#include <gtest/gtest.h>

using namespace std::literals;

TEST(CodecTest, ToBase64UrlEmpty) {
    hmac_service::Codec codec;

    unsigned char data[] = "";
    std::string result = codec.ToBase64Url(data, sizeof(data) - 1);
    ASSERT_TRUE(codec.ValidateBase64Url(result));
    ASSERT_EQ(result, ""s);
}

TEST(CodecTest, ToBase64UrlHelloWorld) {
    hmac_service::Codec codec;

    unsigned char data[] = "hello world!";
    std::string result = codec.ToBase64Url(data, sizeof(data) - 1);
    ASSERT_TRUE(codec.ValidateBase64Url(result));
    ASSERT_EQ(result, "aGVsbG8gd29ybGQh"s);
}

TEST(CodecTest, ToBase64UrlSpecialSymbols) {
    hmac_service::Codec codec;

    unsigned char data[] = "symbols: + - / $ % & _ -; numbers 17 65 3";
    std::string result = codec.ToBase64Url(data, sizeof(data) - 1);
    ASSERT_TRUE(codec.ValidateBase64Url(result));
    ASSERT_EQ(result, "c3ltYm9sczogKyAtIC8gJCAlICYgXyAtOyBudW1iZXJzIDE3IDY1IDM"s);
}
