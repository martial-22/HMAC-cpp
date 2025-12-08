#pragma once

#include <cpprest/http_listener.h>
#include <cpprest/json.h>

namespace hmac_service {

class Server final {
public:
    Server(web::http::uri uri);

private:
	web::http::experimental::listener::http_listener listener_;
};

}
