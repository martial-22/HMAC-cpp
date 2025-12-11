#include "Server.h"

#include <iostream>

int main(int argc, char* argv[]) {
    try {
        hmac_service::Server server;
		
        std::cout << "Press Enter to stop server\n";
        std::string dummy;
        std::getline(std::cin, dummy);
    }
    catch (const std::exception & e) {
        std::cerr << "Exception: " << e.what() << " was thrown\n";
    }
    return 0;
}
