#pragma once

#include <memory>

namespace web::http {

class http_request;

namespace experimental::listener {
class http_listener;
}

}

namespace hmac_service {

class Config;

class Server final {
public:
    Server();

    Server(const Server&) = delete;
    Server(Server&&) = delete;
    
    Server& operator=(const Server&) = delete;
    Server& operator=(Server&&) = delete;

    ~Server();

    bool IsListening() const;

private:
    void HandlePost(const web::http::http_request& request);

    std::unique_ptr<Config> config_ = nulltpr;
	std::unique_ptr<web::http::experimental::listener::http_listener> listener_ = nulltpr;
};

}
