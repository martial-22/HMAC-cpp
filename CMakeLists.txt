cmake_minimum_required(VERSION 3.15)
project(HMAC_Service)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)
find_package(cpprestsdk REQUIRED)
find_package(nlohmann_json REQUIRED)

set(LIB
        Codec.h
        Codec.cpp
        Config.h
        Config.cpp
        HmacService.h
        HmacService.cpp
        Server.h
        Server.cpp
)

add_library(hmac_service ${LIB})

target_link_libraries(hmac_service
        PRIVATE
        cpprestsdk::cpprest
        OpenSSL::SSL
        OpenSSL::Crypto
        nlohmann_json::nlohmann_json
)

add_executable(hmac_service_main
        main.cpp
)

target_link_libraries(hmac_service_main
        hmac_service
)

# Google tests
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        tests
        CodecTests.cpp
        HmacServiceTests.cpp
        ServerTests.cpp
)

target_link_libraries(tests
        GTest::gtest_main
        hmac_service
)

include(GoogleTest)
gtest_discover_tests(tests)

# Rotate-secret
add_executable(rotate_secret
        RotateSecret.cpp
)

target_link_libraries(rotate_secret
        OpenSSL::SSL
        nlohmann_json::nlohmann_json
)
