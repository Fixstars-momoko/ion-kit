#include <ion/ion.h>

using namespace ion;

int main(int argc, char *argv[]) {
    Builder b;
    b.set_target(Halide::get_target_from_environment());
    Node in = b.add("core_random_buffer_3d_uint8")
                  .set_param(
                      Param{"extent0", "16"},
                      Param{"extent1", "16"},
                      Param{"extent2", "3"});
    Node out = b.add("image_io_image_saver")
                   .set_param(
                       Param{"path", "out.png"},
                       Param{"width", "16"},
                       Param{"height", "16"})(
                       in["output"]);

    Halide::Buffer<int32_t> buf(std::vector<int>{});

    PortMap pm;
    pm.set(out["output"], buf);

    b.run(pm);

    return 0;
}
