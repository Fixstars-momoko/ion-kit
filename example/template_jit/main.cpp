#include <ion/ion.h>

using namespace ion;

int main(int argc, char *argv[]) {
    Builder b;
    b.set_target(Halide::get_target_from_environment());

    Halide::Buffer<int32_t> buf(std::vector<int>{});

    PortMap pm;
    pm.set(out["output"], buf);

    b.run(pm);

    return 0;
}
