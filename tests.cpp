#include "HmacService.h"

#include <cstdint>
#include <gtest/gtest.h>

TEST(HmacServiceTest, SucceedVerification1) {
    hmac_service::HmacService hmac("secret");

    const std::string message = "hello";
    const std::string signature = hmac.Sign(message);

    ASSERT_TRUE(hmac.Verify(message, signature));
}

TEST(HmacServiceTest, SucceedVerification2) {
    hmac_service::HmacService hmac("one two three +*-_-*+");

    const std::string message = "a four-word message";
    const std::string signature = hmac.Sign(message);

    ASSERT_TRUE(hmac.Verify(message, signature));
}

TEST(HmacServiceTest, FailedVerification1) {
    hmac_service::HmacService hmac("secret");

    const std::string message = "hello";
    const std::string signature = hmac.Sign(message);

    ASSERT_FALSE(hmac.Verify(message, std::string(signature + "wrong signature")));
}

TEST(HmacServiceTest, FailedVerification2) {
    hmac_service::HmacService hmac("one two three +*-_-*+");

    const std::string message = "a four-word message";
    const std::string signature = hmac.Sign(message);

    ASSERT_FALSE(hmac.Verify(message, std::string(signature + "wrong signature")));
}

TEST(HmacServiceTest, Stability) {
    hmac_service::HmacService hmac("one two three +*-_-*+");

    const std::string message = "a four-word message";

    const std::string signature = hmac.Sign(message);
    for (int16_t i = 0; i < std::numeric_limits<int16_t>::max(); ++i) {
        ASSERT_EQ(hmac.Sign(message), signature);
    }
}

