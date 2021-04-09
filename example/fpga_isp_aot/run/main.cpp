#include <HalideBuffer.h>

#include "pipeline.h"

int main(int argc, char *argv[]) {
    Halide::Runtime::Buffer<int32_t> output_buf(std::vector<int>{});

    pipeline(output_buf);
    halide_profiler_reset();
    pipeline(output_buf);

    return 0;
}
