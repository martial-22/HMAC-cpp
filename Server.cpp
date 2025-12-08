#include "Server.h"

namespace hmac_service {

using namespace web;

namespace {

void handle_post(http::http_request request) {
    // TODO: implement
}

}

Server::Server(http::uri uri)
    : listener_(std::move(uri)) {

	listener_.support(web::http::methods::POST, handle_post);
	listener_.open()
		.then([]() { /* TODO: replace with logger*/ std::cout << "Starting server\n"; })
		.wait();
}

}
