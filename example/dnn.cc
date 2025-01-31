#include <ion/ion.h>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace ion;

int main(int argc, char *argv[]) {
    try {
        const int height = 369;
        const int width = 512;
        const int channel = 3;

        Builder b;
        b.set_target(Halide::get_target_from_environment());
        b.with_bb_module("ion-bb");

        Node n;
        n = b.add("image_io_color_data_loader").set_param(Param{"url", "http://ion-kit.s3.us-west-2.amazonaws.com/images/pedestrian.jpg"}, Param{"width", std::to_string(width)}, Param{"height", std::to_string(height)});
        n = b.add("base_normalize_3d_uint8")(n["output"]);
        n = b.add("base_reorder_buffer_3d_float")(n["output"]).set_param(Param{"dim0", "2"}, Param{"dim1", "0"}, Param{"dim2", "1"});  // CHW -> HWC
        n = b.add("dnn_object_detection")(n["output"]);
        n = b.add("base_denormalize_3d_uint8")(n["output"]);

        Halide::Buffer<uint8_t> out_buf(channel, width, height);

        PortMap pm;
        pm.set(n["output"], out_buf);
        b.run(pm);

        cv::Mat predicted(height, width, CV_8UC3, out_buf.data());
        cv::cvtColor(predicted, predicted, cv::COLOR_RGB2BGR);
        cv::imwrite("predicted.png", predicted);

        std::cout << "yolov4 example done!!!" << std::endl;

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    } catch (...) {
        return -1;
    }

    return 0;
}
