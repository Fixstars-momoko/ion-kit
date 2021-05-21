#ifndef ION_BB_INTERNAL_BB_H
#define ION_BB_INTERNAL_BB_H

#include "ion/ion.h"

namespace ion {
namespace bb {
namespace internal {

class Schedule : public ion::BuildingBlock<Schedule> {
public:
    GeneratorParam<std::string> output_name{"output_name", ""};
    GeneratorParam<bool> output_replace{"output_replace", false};
    GeneratorParam<std::string> compute_level{"compute_level", ""}; // "compute_inline" or "compute_root"
    GeneratorParam<std::string> concurrency{"concurrency", ""}; // comma separated string

    GeneratorInput<Halide::Func> input{"input"};
    GeneratorOutput<Halide::Func> output{"output"};

    void generate() {
        using namespace Halide;
        Func f(static_cast<std::string>(output_name));
        f(_) = input(_);

        if (static_cast<std::string>(compute_level) == "compute_inline") {
            // NOP
        } else if (static_cast<std::string>(compute_level) == "compute_root") {
            f.compute_root();
            if (get_target().has_gpu_feature()) {
                if (f.args().size() == 0) {
                    // NOP
                } else if (f.args().size() == 1) {
                    Var i = f.args()[0];
                    Var block, thread;
                    f.split(i, block, thread, 64);
                    f.gpu_blocks(block).gpu_threads(thread);
                } else {
                    Var x = f.args()[0];
                    Var y = f.args()[1];
                    for (int i=2; i<f.args().size(); ++i) {
                        f.fuse(y, f.args()[i], y);
                    }
                    Var xo, yo, xi, yi;
                    f.gpu_tile(x, y, xo, yo, xi, yi, 16, 16);
                }
            } else {
                if (f.args().size() == 0) {
                    // NOP
                } else if (f.args().size() == 1) {
                    f.parallel(f.args()[0]);
                } else {
                    Var x = f.args()[0];
                    Var y = f.args()[1];
                    for (int i=2; i<f.args().size(); ++i) {
                        f.fuse(y, f.args()[i], y);
                    }
                    f.parallel(y);
                }
            }
        } else {
            throw std::runtime_error("Unreachable");
        }

        output = f;
    }
};

} // internal
} // bb
} // ion

ION_REGISTER_BUILDING_BLOCK(ion::bb::internal::Schedule, internal_schedule);

namespace ion {
namespace bb {
namespace internal {

class ScheduleForPreview : public ion::BuildingBlock<ScheduleForPreview> {
public:
    GeneratorParam<std::string> output_name{"output_name", ""};
    GeneratorParam<bool> output_replace{"output_replace", false};
    GeneratorParam<std::string> compute_level{"compute_level", ""};

    GeneratorInput<Halide::Func> input{"input"};
    GeneratorOutput<Halide::Func> output{"output"};
    GeneratorOutput<Halide::Func> output_for_preview{"output_for_preview"};

    void generate() {
        using namespace Halide;
        {
            Func f(static_cast<std::string>(output_name) + "_");
            f(_) = input(_);
            f.compute_root();
            output = f;
        }

        {
            Func f(static_cast<std::string>(output_name));
            f(_) = input(_);
            f.compute_root();
            output_for_preview = f;
        }
    }
};

} // internal
} // bb
} // ion

ION_REGISTER_BUILDING_BLOCK(ion::bb::internal::ScheduleForPreview, internal_schedule_for_preview);

// #include "bb_sgm.h"

#endif
