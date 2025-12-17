#include "HmacService.h"

#include <chrono>
#include <cstdint>
#include <gtest/gtest.h>

TEST(HmacServiceTest, SucceedVerification1) {
    hmac_service::HmacService hmac("XuggrUnKBul8_Ouh_StFTmMdTFiaDpy8BV88m0jElHbQw4uMoTkWEI4PhomyxuvD");

    const std::string message = "hello";
    const std::string signature = hmac.Sign(message);

    ASSERT_TRUE(hmac.Verify(message, signature));
}

TEST(HmacServiceTest, SucceedVerification2) {
    hmac_service::HmacService hmac("cz_yVDQRuuNnqmA_Jnijsc3wF6oouYhv_s0sDwY2kiqYxXoGVrWHmZ8HXxLNwKLZ");

    const std::string message = "a four-word message";
    const std::string signature = hmac.Sign(message);

    ASSERT_TRUE(hmac.Verify(message, signature));
}

TEST(HmacServiceTest, FailedVerification1) {
    hmac_service::HmacService hmac("XuggrUnKBul8_Ouh_StFTmMdTFiaDpy8BV88m0jElHbQw4uMoTkWEI4PhomyxuvD");

    const std::string message = "hello";
    const std::string signature = hmac.Sign(message);

    ASSERT_FALSE(hmac.Verify(message, std::string(signature + "wrong signature")));
}

TEST(HmacServiceTest, FailedVerification2) {
    hmac_service::HmacService hmac("cz_yVDQRuuNnqmA_Jnijsc3wF6oouYhv_s0sDwY2kiqYxXoGVrWHmZ8HXxLNwKLZ");

    const std::string message = "a four-word message";
    const std::string signature = hmac.Sign(message);

    ASSERT_FALSE(hmac.Verify(message, std::string(signature + "wrong signature")));
}

TEST(HmacServiceTest, Stability) {
    hmac_service::HmacService hmac("cz_yVDQRuuNnqmA_Jnijsc3wF6oouYhv_s0sDwY2kiqYxXoGVrWHmZ8HXxLNwKLZ");

    const std::string message = "a four-word message";

    const std::string signature = hmac.Sign(message);
    for (int16_t i = 0; i < std::numeric_limits<int16_t>::max(); ++i) {
        ASSERT_EQ(hmac.Sign(message), signature);
    }
}

TEST(HmacServiceTest, ConstantTimeCompare) {

    hmac_service::HmacService hmac("ExWlBlL3JLetfXRXvC5bTYPjdz0YT70m475qVT0yHEOGrnHjm3DotaVjb7Lg1pvK");
    auto VariableTimeCompare = [&hmac](std::string_view message, std::string_view to_compare) {

        std::string signature = hmac.Sign(message);
        if (signature.size() != to_compare.size()) {
            return false;
        }

        for (std::size_t i = 0; i < signature.size(); ++i) {
            if (signature[i] != to_compare[i]) {
                return false;
            }
        }
        return true;
    };


    const std::string message = "a four-word message";
    const std::string signature = hmac.Sign(message);

    std::string corrupted_last = signature;
    corrupted_last.back() = corrupted_last.back() == 'a' ? 'b' : 'a';
    
    std::string corrupted_first = signature;
    corrupted_first.front() = corrupted_first.front() == 'a' ? 'b' : 'a';

    ASSERT_FALSE(hmac.Verify(message, std::string(signature + "wrong signature")));

    std::chrono::duration<double, std::milli> varTimeLast;
    {
        auto start_time = std::chrono::steady_clock::now();
        for (std::size_t i = 0; i < 1000000; ++i) {
            VariableTimeCompare(message, corrupted_last);
        }
        auto end_time = std::chrono::steady_clock::now();
        varTimeLast = end_time - start_time;
    }
    
    std::chrono::duration<double, std::milli> varTimeFirst;
    {
        auto start_time = std::chrono::steady_clock::now();
        for (std::size_t i = 0; i < 1000000; ++i) {
            VariableTimeCompare(message, corrupted_first);
        }
        auto end_time = std::chrono::steady_clock::now();
        varTimeFirst = end_time - start_time;
    }

    std::chrono::duration<double, std::milli> constTimeLast;
    {
        auto start_time = std::chrono::steady_clock::now();
        for (std::size_t i = 0; i < 1000000; ++i) {
            hmac.Verify(message, corrupted_last);
        }
        auto end_time = std::chrono::steady_clock::now();
        constTimeLast = end_time - start_time;
    }
    
    std::chrono::duration<double, std::milli> constTimeFirst;
    {
        auto start_time = std::chrono::steady_clock::now();
        for (std::size_t i = 0; i < 1000000; ++i) {
            hmac.Verify(message, corrupted_first);
        }
        auto end_time = std::chrono::steady_clock::now();
        constTimeFirst = end_time - start_time;
    }

    const double constDiff = std::abs(constTimeLast.count() - constTimeFirst.count());
    const double varDiff = std::abs(varTimeLast.count() - varTimeFirst.count());

    ASSERT_TRUE(constDiff < varDiff);
}
