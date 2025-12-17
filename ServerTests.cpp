#include "Server.h"
#include "Config.h"

#include <gtest/gtest.h>
#include <cpprest/http_headers.h>
#include <cpprest/http_client.h>
#include <cpprest/json.h>

using namespace web;
using namespace std::literals;

TEST(ServerTest, SucceedVerification) {

    hmac_service::Config config;
    ASSERT_TRUE(config.Upload());

    hmac_service::Server server;

    const uri uri(std::string(config.GetUri()));
    http::client::http_client client(uri);

    json::value request_json;
    request_json["msg"] = json::value::string("hello");

    http::status_code status;
    json::value reply;

	client.request(http::methods::POST, U("/sign"), request_json)
        .then([&status, &reply](pplx::task<http::http_response> task) {
            http::http_response response = task.get();
            status = response.status_code();
            reply = response.extract_json().get();
        })
        .wait();

    ASSERT_EQ(status, http::status_codes::OK);
    ASSERT_TRUE(reply.has_field("signature"));

    request_json["signature"] = reply["signature"];
	client.request(http::methods::POST, U("/verify"), request_json)
        .then([&status, &reply](pplx::task<http::http_response> task) {
            http::http_response response = task.get();
            status = response.status_code();
            reply = response.extract_json().get();
        })
        .wait();

    ASSERT_EQ(status, http::status_codes::OK);
    ASSERT_TRUE(reply.has_field("ok"));
    ASSERT_TRUE(reply["ok"].as_bool());
}

TEST(ServerTest, CorruptedSignature) {

    hmac_service::Config config;
    ASSERT_TRUE(config.Upload());

    hmac_service::Server server;

    const uri uri(std::string(config.GetUri()));
    http::client::http_client client(uri);

    json::value request_json;
    request_json["msg"] = json::value::string("hello");

    http::status_code status;
    json::value reply;

	client.request(http::methods::POST, U("/sign"), request_json)
        .then([&status, &reply](pplx::task<http::http_response> task) {
            http::http_response response = task.get();
            status = response.status_code();
            reply = response.extract_json().get();
        })
        .wait();

    ASSERT_EQ(status, http::status_codes::OK);
    ASSERT_TRUE(reply.has_field("signature"));

    std::string signature = reply["signature"].as_string();
    signature.back() = signature.back() == 'a' ? 'b' : 'a';

    request_json["signature"] = web::json::value::string(signature);
	client.request(http::methods::POST, U("/verify"), request_json)
        .then([&status, &reply](pplx::task<http::http_response> task) {
            http::http_response response = task.get();
            status = response.status_code();
            reply = response.extract_json().get();
        })
        .wait();

    ASSERT_EQ(status, http::status_codes::OK);
    ASSERT_TRUE(reply.has_field("ok"));
    ASSERT_FALSE(reply["ok"].as_bool());
}

TEST(ServerTest, CorruptedMessage) {

    hmac_service::Config config;
    ASSERT_TRUE(config.Upload());

    hmac_service::Server server;

    const uri uri(std::string(config.GetUri()));
    http::client::http_client client(uri);

    json::value request_json;
    request_json["msg"] = json::value::string("hello");

    http::status_code status;
    json::value reply;

	client.request(http::methods::POST, U("/sign"), request_json)
        .then([&status, &reply](pplx::task<http::http_response> task) {
            http::http_response response = task.get();
            status = response.status_code();
            reply = response.extract_json().get();
        })
        .wait();

    ASSERT_EQ(status, http::status_codes::OK);
    ASSERT_TRUE(reply.has_field("signature"));

    request_json["msg"] = json::value::string("hello!");
    request_json["signature"] = reply["signature"];

	client.request(http::methods::POST, U("/verify"), request_json)
        .then([&status, &reply](pplx::task<http::http_response> task) {
            http::http_response response = task.get();
            status = response.status_code();
            reply = response.extract_json().get();
        })
        .wait();

    ASSERT_EQ(status, http::status_codes::OK);
    ASSERT_TRUE(reply.has_field("ok"));
    ASSERT_FALSE(reply["ok"].as_bool());
}

TEST(ServerTest, InvalidSignatureFormat) {

    hmac_service::Config config;
    ASSERT_TRUE(config.Upload());

    hmac_service::Server server;

    const uri uri(std::string(config.GetUri()));
    http::client::http_client client(uri);

    json::value request_json;

    http::status_code status;
    json::value reply;

    request_json["msg"] = json::value::string("hello!");
    request_json["signature"] = json::value::string("@@@");

	client.request(http::methods::POST, U("/verify"), request_json)
        .then([&status, &reply](pplx::task<http::http_response> task) {
            http::http_response response = task.get();
            status = response.status_code();
            reply = response.extract_json().get();
        })
        .wait();

    ASSERT_EQ(status, http::status_codes::BadRequest);
    ASSERT_TRUE(reply.has_field("error"s));

    ASSERT_EQ(reply["error"].as_string(), "invalid_signature_format"s);
}

TEST(ServerTest, EmptyMessage) {

    hmac_service::Config config;
    ASSERT_TRUE(config.Upload());

    hmac_service::Server server;

    const uri uri(std::string(config.GetUri()));
    http::client::http_client client(uri);

    json::value request_json;
    request_json["msg"] = json::value::string("");

    http::status_code status;
    json::value reply;

	client.request(http::methods::POST, U("/sign"s), request_json)
        .then([&status, &reply](pplx::task<http::http_response> task) {
            http::http_response response = task.get();
            status = response.status_code();
            reply = response.extract_json().get();
        })
        .wait();

    ASSERT_EQ(status, http::status_codes::BadRequest);
    ASSERT_TRUE(reply.has_field("error"s));

    ASSERT_EQ(reply["error"].as_string(), "invalid_msg"s);
}

TEST(ServerTest, MessageLenIsExceeded) {

    hmac_service::Config config;
    ASSERT_TRUE(config.Upload());

    hmac_service::Server server;

    const uri uri(std::string(config.GetUri()));
    http::client::http_client client(uri);

    json::value request_json;

    request_json["msg"] = json::value::string(std::string(config.GetMaxMessageLen() + 1, 'a'));

    http::status_code status;
    json::value reply;

	client.request(http::methods::POST, U("/sign"), request_json)
        .then([&status, &reply](pplx::task<http::http_response> task) {
            http::http_response response = task.get();
            status = response.status_code();
            reply = response.extract_json().get();
        })
        .wait();

    ASSERT_EQ(status, http::status_codes::RequestEntityTooLarge);
    ASSERT_TRUE(reply.has_field("error"));

    ASSERT_EQ(reply["error"].as_string(), "too_large_msg"s);
}

TEST(ServerTest, Stability) {

    hmac_service::Config config;
    ASSERT_TRUE(config.Upload());

    hmac_service::Server server;

    const uri uri(std::string(config.GetUri()));
    http::client::http_client client(uri);

    json::value request_json;
    request_json["msg"] = json::value::string("two words...");

    http::status_code status;
    json::value reply;

	client.request(http::methods::POST, U("/sign"), request_json)
        .then([&status, &reply](pplx::task<http::http_response> task) {
            http::http_response response = task.get();
            status = response.status_code();
            reply = response.extract_json().get();
        })
        .wait();

    ASSERT_EQ(status, http::status_codes::OK);
    ASSERT_TRUE(reply.has_field("signature"));

    const std::string signature = reply["signature"].as_string();

    for (std::int16_t i = 0; i < 1000; ++i) {
        client.request(http::methods::POST, U("/sign"), request_json)
        .then([&status, &reply](pplx::task<http::http_response> task) {
            http::http_response response = task.get();
            status = response.status_code();
            reply = response.extract_json().get();
        })
        .wait();

        ASSERT_EQ(status, http::status_codes::OK);
        ASSERT_TRUE(reply.has_field("signature"));

        ASSERT_EQ(signature, reply["signature"].as_string());
    }
}

TEST(ServerTest, ConstantTimeCompare) {

}

TEST(ServerTest, InvalidSecret) {

}
