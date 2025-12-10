#pragma once

#include <memory>

namespace web::http::experimental::listener {
class http_listener;
}

namespace hmac_service {

class Config;

class Server final {
public:
    Server();
    ~Server();

private:
    std::unique_ptr<Config> config_;
	std::unique_ptr<web::http::experimental::listener::http_listener> listener_;
};

}
