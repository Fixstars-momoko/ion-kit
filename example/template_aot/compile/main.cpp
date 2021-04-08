#include <ion/ion.h>

using namespace ion;

int main(int argc, char *argv[]) {
    Builder b;
    b.set_target(Halide::get_target_from_environment());

    b.compile("pipeline");

    return 0;
}
