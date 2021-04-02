#include <HalideBuffer.h>

#include "core.h"

int main(int argc, char *argv[]) {
    auto buf = Halide::Runtime::Buffer<int32_t>::make_scalar();

    core(buf);

    return 0;
}
