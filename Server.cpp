#include "Server.h"
#include "HmacService.h"

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

std::pair<web::http::status_code, json::value> HandleSign(const json::value& json_obj, const http::http_request& request) {

	json::value reply;
	static constexpr char message_field[]("msg");

	if (!json_obj.has_field(message_field)) {
		reply["error"s] = json::value::string("invalid_msg"s);
		return std::make_pair(web::http::status_codes::BadRequest, std::move(reply));
	}
	// TODO: check message length

	const std::string message = json_obj.at(message_field).as_string();
	
	// TODO: remove "secret"
	const std::string signature = HmacService("secret"s).Sign(message);

	reply["signature"s] = json::value::string(std::move(signature));
	return std::make_pair(web::http::status_codes::OK, std::move(reply));
}

std::pair<web::http::status_code, json::value> HandleVerify(const json::value& json_obj, const http::http_request& request) {

	json::value reply;

	static constexpr char message_field[]("msg");
	static constexpr char signature_field[]("signature");
	
	if (!json_obj.has_field(message_field)) {
		reply["error"s] = json::value::string("invalid_msg"s);
		return std::make_pair(web::http::status_codes::BadRequest, std::move(reply));
	}
	// TODO: check message length

	if (!json_obj.has_field(signature_field)) {
		reply["error"s] = json::value::string("invalid_signature"s);
		return std::make_pair(web::http::status_codes::BadRequest, std::move(reply));
	}
	// TODO: check signature length

	const std::string message = json_obj.at(message_field).as_string();
	const std::string signature = json_obj.at(signature_field).as_string();

	// TODO: remove "secret"
	const bool verification = HmacService("secret"s).Verify(message, signature);

	reply["ok"s] = json::value::boolean(verification);
	return std::make_pair(web::http::status_codes::OK, std::move(reply));
}

void HandlePost(http::http_request request) {

	json::value reply;
	if (request.headers().content_type() != "application/json"s) {
		
		reply["error"s] = json::value::string("unsupported_header"s);
		request.reply(web::http::status_codes::UnsupportedMediaType, reply);
		return;
	}
	
	const std::optional<json::value> json_obj = ExtractJson(request);
	if (!json_obj.has_value()) {
		
		reply["error"s] = json::value::string("invalid_json"s);
		request.reply(web::http::status_codes::BadRequest, reply);
		return;
	}

	const std::string endpoint = request.relative_uri().to_string();
	if (endpoint == "/sign"s) {
		const auto [status, reply] = HandleSign(*json_obj, request);
		request.reply(status, reply);
	}
	else if (endpoint == "/verify"s) {
		const auto [status, reply] = HandleVerify(*json_obj, request);
		request.reply(status, reply);
	}
	else {
		reply["error"s] = json::value::string("unsupported_endpoint"s);
		request.reply(web::http::status_codes::BadRequest, reply);
	}
}

}

Server::Server(http::uri uri)
    : listener_(std::move(uri)) {

	listener_.support(web::http::methods::POST, HandlePost);
	listener_.open().wait();
}

}
