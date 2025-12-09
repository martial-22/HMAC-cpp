cmake_minimum_required(VERSION 3.15)
project(HMAC_Service)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)
find_package(cpprestsdk REQUIRED)

set(LIB
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
        HmacServiceTests.cpp
)

target_link_libraries(tests
        GTest::gtest_main
        hmac_service
)

include(GoogleTest)
gtest_discover_tests(tests)


# Optional
# add_executable(rotate_secret
# )

# target_link_libraries(rotate_secret
#         PRIVATE
#         OpenSSL::SSL
#         OpenSSL::Crypto
# )
