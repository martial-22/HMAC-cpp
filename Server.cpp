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

void HandleSign(const json::value& json_obj, const http::http_request& request) {

	static constexpr char message_field[]("msg");
	if (!json_obj.has_field(message_field)) {
		// TODO: process
		return;
	}

	std::string message = json_obj.at(message_field).as_string();
	
	// TODO: remove "secret"
	const std::string signature = HmacService("secret"s).Sign(message);

	json::value reply;
	reply["signature"s] = json::value::string(std::move(signature));
	request.reply(web::http::status_codes::OK, reply);
}

void HandleVerify(const json::value& json_obj, const http::http_request& request) {

	static constexpr char message_field[]("msg");
	static constexpr char signature_field[]("signature");

	if (!json_obj.has_field(message_field) || !json_obj.has_field(signature_field)) {
		// TODO: process
		return;
	}

	std::string message = json_obj.at(message_field).as_string();
	std::string signature = json_obj.at(signature_field).as_string();

	// TODO: remove "secret"
	const bool verification = HmacService("secret"s).Verify(message, signature);

	json::value reply;
	reply["ok"s] = json::value::boolean(verification);
	request.reply(web::http::status_codes::OK, reply);
}

void HandlePost(http::http_request request) {
	
	const std::optional<json::value> json_obj = ExtractJson(request);
	if (!json_obj.has_value()) {
		// TODO: Process
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
		// TODO: process
		return;
	}
}

}

Server::Server(http::uri uri)
    : listener_(std::move(uri)) {

	listener_.support(web::http::methods::POST, HandlePost);
	listener_.open()
		.then([]() { /* TODO: replace with logger*/ std::cout << "Starting server\n"s; })
		.wait();
}

}
