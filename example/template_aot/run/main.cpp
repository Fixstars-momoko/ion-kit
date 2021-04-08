#include <HalideBuffer.h>

#include "pipeline.h"

int main(int argc, char *argv[]) {
    auto buf = Halide::Runtime::Buffer<int32_t>::make_scalar();

    pipeline(buf);

    return 0;
}
