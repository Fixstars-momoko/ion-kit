
#include <Halide.h>

#ifdef _WIN32
#define ION_EXPORT __declspec(dllexport)
#else
#define ION_EXPORT
#endif

extern "C" {
ION_EXPORT int ion_bb_core_buffer_loader(halide_buffer_t *url_buf, int32_t extent0, int32_t extent1, int32_t extent2, int32_t extent3, halide_buffer_t *out);
}
