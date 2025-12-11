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

    const web::uri uri(std::string(config.GetUri()));
    web::http::client::http_client client(uri);

    json::value request_json;
    request_json["msg"] = web::json::value::string("message to server");

    web::http::status_code status;
    json::value reply;

	client.request(web::http::methods::POST, U("/sign"), request_json)
        .then([&status, &reply](pplx::task<web::http::http_response> task) {
            web::http::http_response response = task.get();
            status = response.status_code();
            reply = response.extract_json().get();
        })
        .wait();

    ASSERT_EQ(status, web::http::status_codes::OK);
    ASSERT_TRUE(reply.has_field("signature"));

    request_json["signature"] = reply["signature"];
	client.request(web::http::methods::POST, U("/verify"), request_json)
        .then([&status, &reply](pplx::task<web::http::http_response> task) {
            web::http::http_response response = task.get();
            status = response.status_code();
            reply = response.extract_json().get();
        })
        .wait();

    ASSERT_EQ(status, web::http::status_codes::OK);
    ASSERT_TRUE(reply.has_field("ok"));
    ASSERT_TRUE(reply["ok"].as_bool());
}
