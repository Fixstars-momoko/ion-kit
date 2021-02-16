#include "ion/util.h"

#include <iomanip>
#include <random>
#include <sstream>

namespace ion {

std::string output_name(const std::string &node_id, const std::string &port_key) {
    std::stringstream ss;

    // Make sure to start from legal character;
    ss << "_";

    // Rpleace '-' by '_'
    for (auto c : node_id + "_" + port_key) {
        if (c == '-') {
            ss << '_';
        } else {
            ss << c;
        }
    }

    return ss.str();
}

std::string array_name(const std::string &port_key, size_t i) {
    return port_key + "_" + std::to_string(i);
}

std::string make_uuid() {
    static std::random_device rng;
    uint32_t n;
    std::stringstream ss;
    ss << std::hex << std::nouppercase << std::setfill('0');

    n = rng();
    ss << std::setw(8) << n << '-';
    n = rng();
    ss << std::setw(4) << (n & 0xFFFFu) << '-' << ((n >> 16) & 0x0FFFu | 0x4000u) << '-';
    n = rng();
    ss << std::setw(4) << (n & 0x3FFFu | 0x8000u) << '-' << (n >> 16);
    n = rng();
    ss << std::setw(8) << n;

    return ss.str();
}

}  // namespace ion
