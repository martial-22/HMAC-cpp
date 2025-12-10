#include "Server.h"
#include "Config.h"
#include "HmacService.h"

#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <optional>

namespace hmac_service {

using namespace web;
using namespace std::literals;

namespace {

std::optional<json::value> ExtractJson(const http::http_request& request) {
	
	json::value json_obj;
	try {
		request.extract_json()
		.then([&json_obj](json::value obj) {
			json_obj = std::move(obj);
		})
		.wait();
	}
	catch (std::exception& e) {
		return std::nullopt;
	}
	return json_obj;
}

void SetErrorReply(const http::http_request& request, http::status_code status, std::string error_msg) {
	
	json::value reply;
	reply["error"s] = json::value::string(std::move(error_msg));
	request.reply(status, reply);
}

void HandleSign(const json::value& json_obj, const http::http_request& request) {

	static constexpr char message_field[]("msg");
	if (!json_obj.has_field(message_field)) {
		SetErrorReply(request, http::status_codes::BadRequest, "invalid_msg"s);
		return;
	}
	// TODO: check message length

	const std::string message = json_obj.at(message_field).as_string();
	
	// TODO: remove "secret"
	const std::string signature = HmacService("secret"s).Sign(message);

	json::value reply;
	reply["signature"s] = json::value::string(std::move(signature));
	request.reply(http::status_codes::OK, std::move(reply));
}

void HandleVerify(const json::value& json_obj, const http::http_request& request) {

	static constexpr char message_field[]("msg");
	if (!json_obj.has_field(message_field)) {
		SetErrorReply(request, http::status_codes::BadRequest, "invalid_msg"s);
		return;
	}
	// TODO: check message length

	static constexpr char signature_field[]("signature");
	if (!json_obj.has_field(signature_field)) {
		SetErrorReply(request, http::status_codes::BadRequest, "invalid_signature"s);
		return;
	}
	// TODO: check signature length

	const std::string message = json_obj.at(message_field).as_string();
	const std::string signature = json_obj.at(signature_field).as_string();

	// TODO: remove "secret"
	const bool verification = HmacService("secret"s).Verify(message, signature);

	json::value reply;
	reply["ok"s] = json::value::boolean(verification);
	request.reply(http::status_codes::OK, std::move(reply));
}

}

Server::Server() {

	config_ = std::make_unique<Config>();

	if (config_->Upload()) {
		listener_ = std::make_unique<http::experimental::listener::http_listener>(std::string(config_->GetUri()));

		listener_->support(web::http::methods::POST, [this](const http::http_request& request) {
			HandlePost(request);
		});
		listener_->open().wait();
	}
}

Server::~Server() {
}

void Server::HandlePost(const http::http_request& request) {

	if (request.headers().content_type() != "application/json"s) {
		SetErrorReply(request, http::status_codes::UnsupportedMediaType, "unsupported_header"s);
		return;
	}
	
	const std::optional<json::value> json_obj = ExtractJson(request);
	if (!json_obj.has_value()) {
		SetErrorReply(request, http::status_codes::BadRequest, "invalid_json"s);
		return;
	}

	const std::string endpoint = request.relative_uri().to_string();
	if (endpoint == "/sign"s) {
		HandleSign(*json_obj, request);
	}
	else if (endpoint == "/verify"s) {
		HandleVerify(*json_obj, request);
	}
	else {
		SetErrorReply(request, http::status_codes::BadRequest, "unsupported_endpoint"s);
	}
}

}
