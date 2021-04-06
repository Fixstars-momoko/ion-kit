#include <ion/ion.h>

namespace ion {
namespace bb {
namespace image_io {

class D435 : public ion::BuildingBlock<D435> {
public:
    GeneratorParam<std::string> gc_title{"gc_title", "D435"};
    GeneratorParam<std::string> gc_description{"gc_description", "This captures D435 stereo image and depth."};
    GeneratorParam<std::string> gc_tags{"gc_tags", "input,sensor"};
    GeneratorParam<std::string> gc_inference{"gc_inference", R"((function(v){ return { output_l: [1280, 720], output_r: [1280, 720], output_d: [1280, 720] }}))"};
    GeneratorParam<std::string> gc_mandatory{"gc_mandatory", ""};
    GeneratorParam<std::string> gc_strategy{"gc_strategy", "self"};
    GeneratorParam<std::string> gc_prefix{"gc_prefix", ""};

    GeneratorOutput<Halide::Func> output_l{"output_l", Halide::type_of<uint8_t>(), 2};
    GeneratorOutput<Halide::Func> output_r{"output_r", Halide::type_of<uint8_t>(), 2};
    GeneratorOutput<Halide::Func> output_d{"output_d", Halide::type_of<uint16_t>(), 2};

    void generate() {
        using namespace Halide;
        Func realsense_d435_frameset(static_cast<std::string>(gc_prefix) + "realsense_d435_frameset");
        realsense_d435_frameset.define_extern("ion_bb_image_io_realsense_d435_frameset", {}, type_of<uint64_t>(), 0);
        realsense_d435_frameset.compute_root();

        Func realsense_d435_infrared(static_cast<std::string>(gc_prefix) + "realsense_d435_infrared");
        realsense_d435_infrared.define_extern("ion_bb_image_io_realsense_d435_infrared", {realsense_d435_frameset}, {type_of<uint8_t>(), type_of<uint8_t>()}, 2);
        realsense_d435_infrared.compute_root();

        Func realsense_d435_depth(static_cast<std::string>(gc_prefix) + "realsense_d435_depth");
        realsense_d435_depth.define_extern("ion_bb_image_io_realsense_d435_depth", {realsense_d435_frameset}, type_of<uint16_t>(), 2);
        realsense_d435_depth.compute_root();

        output_l(_) = realsense_d435_infrared(_)[0];
        output_r(_) = realsense_d435_infrared(_)[1];
        output_d(_) = realsense_d435_depth(_);
    }
};

}  // namespace image_io
}  // namespace bb
}  // namespace ion

ION_REGISTER_BUILDING_BLOCK(ion::bb::image_io::D435, image_io_d435);
