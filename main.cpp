#include <iostream>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>

using namespace web;

namespace {

void handle_post(http::http_request request) {
    // TODO: implement
}

}

class Server final {
public:
    Server(http::uri uri)
        : listener_(std::move(uri)) {

	    listener_.support(web::http::methods::POST, handle_post);
		listener_.open()
			.then([]() { /* TODO: replace with logger*/ std::cout << "Starting server\n"; })
			.wait();
    }

private:
	web::http::experimental::listener::http_listener listener_;
};

int main(int argc, char* argv[])
{
	try{
        Server server(U("http://localhost:8080/"));
		
        std::cout << "Press Enter to stop server\n";
        std::string dummy;
        std::getline(std::cin, dummy);
	}
	catch (const std::exception & e) {
        std::cerr << "Exception: " << e.what() << " was thrown\n";
	}
	return 0;
}
