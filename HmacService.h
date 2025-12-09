#include <string_view>

namespace hmac_service {

struct Secret {
    const void* data = nullptr;
    const int len = 0;
};

class HmacService {
public:
    HmacService(Secret secret);

    std::string Sign(std::string_view message);
    bool Verify(std::string_view message, std::string_view signature);

private:
    const Secret secret_;
};

}
