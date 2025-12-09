#include "Server.h"

namespace hmac_service {

using namespace web;

namespace {

void HandlePost(http::http_request request) {
	// TODO: implement
	json::value json_obj;
    request.extract_json()
      	.then([&json_obj](json::value obj) {
        json_obj = obj;
	})
    .wait();
	
	json::value result = json_obj;
	result[U("signature")] = json::value::string(U("result"));
	request.reply(web::http::status_codes::OK, result);
}

}

Server::Server(http::uri uri)
    : listener_(std::move(uri)) {

	listener_.support(web::http::methods::POST, HandlePost);
	listener_.open()
		.then([]() { /* TODO: replace with logger*/ std::cout << "Starting server\n"; })
		.wait();
}

}
