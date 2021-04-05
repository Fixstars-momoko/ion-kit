#include <string>
#include <tuple>
#include <vector>

#include <opencv2/imgcodecs.hpp>

#include <httplib.h>

#include "rt_common.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace ion {
namespace bb {
namespace image_io {

std::tuple<std::string, std::string> parse_url(const std::string &url) {
    if (url.rfind("http://", 0) != 0) {  // not starts_with
        return std::tuple<std::string, std::string>("", "");
    }
    auto path_name_pos = url.find("/", 7);
    auto host_name = url.substr(0, path_name_pos);
    auto path_name = url.substr(path_name_pos);
    return std::tuple<std::string, std::string>(host_name, path_name);
}

cv::Mat get_image(const std::string &url) {
    if (url.empty()) {
        return {};
    }

    std::string host_name;
    std::string path_name;
    std::tie(host_name, path_name) = parse_url(url);

    cv::Mat img;
    bool img_loaded = false;
    if (host_name.empty() || path_name.empty()) {
        // fallback to local file
        img = cv::imread(url);
        if (!img.empty()) {
            img_loaded = true;
        }
    } else {
        httplib::Client cli(host_name.c_str());
        cli.set_follow_location(true);
        auto res = cli.Get(path_name.c_str());
        if (res && res->status == 200) {
            std::vector<char> data(res->body.size());
            std::memcpy(data.data(), res->body.c_str(), res->body.size());
            img = cv::imdecode(cv::InputArray(data), cv::IMREAD_COLOR);
            if (!img.empty()) {
                img_loaded = true;
            }
        }
    }

    if (img_loaded) {
        return img;
    } else {
        return {};
    }
}

}  // namespace image_io
}  // namespace bb
}  // namespace ion
