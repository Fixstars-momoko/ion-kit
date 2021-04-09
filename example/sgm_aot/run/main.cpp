#include <HalideBuffer.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

#include "pipeline.h"

int main(int argc, char *argv[]) {
    try {
        int input_width = 1282;
        int input_height = 1110;
        float scale = 0.5;
        int output_width = static_cast<int>(input_width * scale);
        int output_height = static_cast<int>(input_height * scale);
        const int disp = 16;

        Halide::Runtime::Buffer<uint8_t> obuf(std::vector<int>{output_width, output_height});

        // Initial execution
        halide_reuse_device_allocations(nullptr, true);
        pipeline(obuf);
        halide_profiler_reset();

        const int iter = 10;
        for (int i = 0; i < iter; ++i) {
            pipeline(obuf);
        }

        obuf.copy_to_host();
        cv::Mat img(std::vector<int>{output_height, output_width}, CV_8UC1, obuf.data());
        cv::imwrite("sgm-out.png", img);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    } catch (...) {
        return -1;
    }

    return 0;
}
