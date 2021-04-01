#include <ion/ion.h>

using namespace ion;

int main(int argc, char *argv[]) {
    Builder b;
    b.set_target(Halide::get_target_from_environment());
    Node in = b.add("core_random_buffer_2d_uint8")
                  .set_param(
                      Param{"extent0", "8"},
                      Param{"extent1", "8"});
    Node out = b.add("core_buffer_saver_2d_uint8")
                   .set_param(
                       Param{"path", "./out.dat"},
                       Param{"extent0", "8"},
                       Param{"extent1", "8"})(
                       in["output"]);

    Halide::Buffer<int32_t> buf(std::vector<int>{});

    PortMap pm;
    pm.set(out["output"], buf);

    b.run(pm);

    return 0;
}
