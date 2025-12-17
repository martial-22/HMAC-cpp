#include "Server.h"

#include <iostream>

int main(int argc, char* argv[]) {
    try {
        hmac_service::Server server;
        if (!server.IsListening()) {
            std::cerr << "Server run is failed" << std::endl;
            return 1;
        }
		
        std::cout << "Press Enter to stop server" << std::endl;
        std::string dummy;
        std::getline(std::cin, dummy);
    }
    catch (const std::exception & e) {
        std::cerr << "Exception: " << e.what() << " was thrown" << std::endl;
        return 1;
    }
    return 0;
}
