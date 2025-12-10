#include "Codec.h"

#include <gtest/gtest.h>

using namespace std::literals;

TEST(CodecTest, ToBase64UrlEmpty) {
    hmac_service::Codec codec;

    unsigned char data[] = "";
    ASSERT_EQ(codec.ToBase64Url(data, sizeof(data) - 1), ""s);
}

TEST(CodecTest, ToBase64UrlHelloWorld) {
    hmac_service::Codec codec;

    unsigned char data[] = "hello world!";
    ASSERT_EQ(codec.ToBase64Url(data, sizeof(data) - 1), "aGVsbG8gd29ybGQh"s);
}

TEST(CodecTest, ToBase64UrlSpecialSymbols) {
    hmac_service::Codec codec;

    unsigned char data[] = "symbols: + - / $ % & _ -; numbers 17 65 3";
    ASSERT_EQ(codec.ToBase64Url(data, sizeof(data) - 1), "c3ltYm9sczogKyAtIC8gJCAlICYgXyAtOyBudW1iZXJzIDE3IDY1IDM"s);
}
