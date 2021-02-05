#include <cassert>
#include <cmath>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <string>
#include <vector>

#include <ion/ion.h>

#define DISABLE_SCHEDULE
#include "ion-bb-core/bb.h"
#include "ion-bb-image-io/bb.h"
#include "ion-bb-image-processing/bb.h"
#include "ion-bb-internal/bb.h"

#include "ion-bb-core/rt.h"
#include "ion-bb-image-io/rt.h"
#include "ion-bb-image-processing/rt.h"

using namespace ion;

int main(int argc, char *argv[]) {
    assert(argc >= 2);

    Builder b;
    b.set_target(Halide::get_target_from_environment());

    Port offset_r{"offset_r", Halide::type_of<float>()};
    Port offset_g{"offset_g", Halide::type_of<float>()};
    Port offset_b{"offset_b", Halide::type_of<float>()};
    Port gain_r{"gain_r", Halide::type_of<float>()};
    Port gain_g{"gain_g", Halide::type_of<float>()};
    Port gain_b{"gain_b", Halide::type_of<float>()};
    Port shading_correction_slope_r{"shading_correction_slope_r", Halide::type_of<float>()};
    Port shading_correction_slope_g{"shading_correction_slope_g", Halide::type_of<float>()};
    Port shading_correction_slope_b{"shading_correction_slope_b", Halide::type_of<float>()};
    Port shading_correction_offset_r{"shading_correction_offset_r", Halide::type_of<float>()};
    Port shading_correction_offset_g{"shading_correction_offset_g", Halide::type_of<float>()};
    Port shading_correction_offset_b{"shading_correction_offset_b", Halide::type_of<float>()};
    Port coef_color{"coef_color", Halide::type_of<float>()};
    Port coef_space{"coef_space", Halide::type_of<float>()};
    Port gamma{"gamma", Halide::type_of<float>()};
    Port k1{"k1", Halide::type_of<float>()};
    Port k2{"k2", Halide::type_of<float>()};
    Port k3{"k3", Halide::type_of<float>()};
    Port p1{"p1", Halide::type_of<float>()};
    Port p2{"p2", Halide::type_of<float>()};
    Port fx{"fx", Halide::type_of<float>()};
    Port fy{"fy", Halide::type_of<float>()};
    Port cx{"cx", Halide::type_of<float>()};
    Port cy{"cy", Halide::type_of<float>()};
    Port output_scale{"output_scale", Halide::type_of<float>()};

    // Parameters for IMX219
    int32_t width = 3264;
    int32_t height = 2464;
    int32_t bit_width = 10;
    int32_t bit_shift = 6;
    float scale = 0.25;

    PortMap pm;
    pm.set(offset_r, 1.f / 16.f);
    pm.set(offset_g, 1.f / 16.f);
    pm.set(offset_b, 1.f / 16.f);
    pm.set(gain_r, 2.5f);
    pm.set(gain_g, 2.0f);
    pm.set(gain_b, 3.2f);
    pm.set(shading_correction_slope_r, 0.7f);
    pm.set(shading_correction_slope_g, 0.2f);
    pm.set(shading_correction_slope_b, 0.1f);
    pm.set(shading_correction_offset_r, 1.f);
    pm.set(shading_correction_offset_g, 1.f);
    pm.set(shading_correction_offset_b, 1.f);
    pm.set(coef_color, 100.f);
    pm.set(coef_space, 0.03f);
    pm.set(gamma, 1.f / 2.2f);
    pm.set(k1, 0.f);
    pm.set(k2, 0.f);
    pm.set(k3, 0.f);
    pm.set(p1, 0.f);
    pm.set(p2, 0.f);
    pm.set(fx, static_cast<float>(sqrt(width * width + height * height) / 2));
    pm.set(fy, static_cast<float>(sqrt(width * width + height * height) / 2));
    pm.set(cx, width * 0.5f);
    pm.set(cy, height * 0.6f);
    pm.set(output_scale, 1.f);

    int32_t output_width = width * scale;
    int32_t output_height = height * scale;

    // IMX219
    Node imx = b.add("image_io_imx219")
                   .set_param(
                       Param{"index", "0"},
                       Param{"force_sim_mode", "true"},
                       Param{"url", argv[1]});
    // ISP
    Node normalize = b.add("image_processing_normalize_raw_image")
                         .set_param(
                             Param{"bit_width", std::to_string(bit_width)},
                             Param{"bit_shift", std::to_string(bit_shift)})(
                             imx["output"]);
    Node offset = b.add("image_processing_bayer_offset")
                      .set_param(
                          Param{"bayer_pattern", "0"})(  // RGGB
                          offset_r,
                          offset_g,
                          offset_b,
                          normalize["output"]);
    Node shading_correction = b.add("image_processing_lens_shading_correction_linear")
                                  .set_param(
                                      Param{"bayer_pattern", "0"},  // RGGB
                                      Param{"width", std::to_string(width)},
                                      Param{"height", std::to_string(height)})(
                                      shading_correction_slope_r,
                                      shading_correction_slope_g,
                                      shading_correction_slope_b,
                                      shading_correction_offset_r,
                                      shading_correction_offset_g,
                                      shading_correction_offset_b,
                                      offset["output"]);
    Node white_balance = b.add("image_processing_bayer_white_balance")
                             .set_param(
                                 Param{"bayer_pattern", "0"})(  // RGGB
                                 gain_r,
                                 gain_g,
                                 gain_b,
                                 shading_correction["output"]);
    Node white_balance_s = b.add("internal_schedule")
                               .set_param(
                                   Param{"output_name", "white_balance"},
                                   Param{"output_replace", "true"},
                                   Param{"compute_level", "compute_root"})(
                                   white_balance["output"]);
    Node demosaic = b.add("image_processing_bayer_demosaic_filter")
                        .set_param(
                            Param{"bayer_pattern", "0"},  // RGGB
                            Param{"width", std::to_string(width)},
                            Param{"height", std::to_string(height)})(
                            white_balance_s["output"]);
    Node demosaic_s = b.add("internal_schedule")
                          .set_param(
                              Param{"output_name", "demosaic"},
                              Param{"output_replace", "true"},
                              Param{"compute_level", "compute_root"})(
                              demosaic["output"]);
    Node luminance = b.add("image_processing_calc_luminance")
                         .set_param(
                             Param{"luminance_method", "0"})(  // Average
                             demosaic_s["output"]);
    Node luminance_filter = b.add("core_constant_buffer_2d_float")
                                .set_param(
                                    Param{"values", "0.04"},
                                    Param{"extent0", "5"},
                                    Param{"extent1", "5"});
    Node filtered_luminance = b.add("image_processing_convolution_2d")
                                  .set_param(
                                      Param{"boundary_conditions_method", "0"},  // RepeatEdge
                                      Param{"window_size", "2"},
                                      Param{"width", std::to_string(width)},
                                      Param{"height", std::to_string(height)})(
                                      luminance_filter["output"],
                                      luminance["output"]);
    Node filtered_luminance_s = b.add("internal_schedule")
                                    .set_param(
                                        Param{"output_name", "filtered_luminance"},
                                        Param{"output_replace", "true"},
                                        Param{"compute_level", "compute_root"})(
                                        filtered_luminance["output"]);
    Node noise_reduction = b.add("image_processing_bilateral_filter_3d")
                               .set_param(
                                   Param{"color_difference_method", "1"},  // Average
                                   Param{"window_size", "2"},
                                   Param{"width", std::to_string(width)},
                                   Param{"height", std::to_string(height)})(
                                   coef_color,
                                   coef_space,
                                   filtered_luminance_s["output"],
                                   demosaic_s["output"]);
    Node noise_reduction_s = b.add("internal_schedule")
                                 .set_param(
                                     Param{"output_name", "noise_reduction"},
                                     Param{"output_replace", "true"},
                                     Param{"compute_level", "compute_root"})(
                                     noise_reduction["output"]);
    Node color_matrix = b.add("core_constant_buffer_2d_float")
                            .set_param(
                                Param{"values", "2.20213000 -1.27425000 0.07212000 -0.25650000 1.45961000 -0.20311000 0.07458000 -1.35791000 2.28333000"},
                                Param{"extent0", "3"},
                                Param{"extent1", "3"});
    Node color_conversion = b.add("image_processing_color_matrix")(
        color_matrix["output"],
        noise_reduction_s["output"]);
    Node color_conversion_s = b.add("internal_schedule")
                                  .set_param(
                                      Param{"output_name", "color_conversion"},
                                      Param{"output_replace", "true"},
                                      Param{"compute_level", "compute_root"})(
                                      color_conversion["output"]);
    Node distortion_correction = b.add("image_processing_lens_distortion_correction_model_3d")
                                     .set_param(
                                         Param{"width", std::to_string(width)},
                                         Param{"height", std::to_string(height)})(
                                         k1,
                                         k2,
                                         k3,
                                         p1,
                                         p2,
                                         fx,
                                         fy,
                                         cx,
                                         cy,
                                         output_scale,
                                         color_conversion_s["output"]);
    Node distortion_correction_s = b.add("internal_schedule")
                                       .set_param(
                                           Param{"output_name", "distortion_correction"},
                                           Param{"output_replace", "true"},
                                           Param{"compute_level", "compute_root"})(
                                           distortion_correction["output"]);
    Node resize = b.add("image_processing_resize_area_average_3d")
                      .set_param(
                          Param{"width", std::to_string(width)},
                          Param{"height", std::to_string(height)},
                          Param{"scale", std::to_string(scale)})(
                          distortion_correction_s["output"]);
    Node resize_s = b.add("internal_schedule")
                        .set_param(
                            Param{"output_name", "resize"},
                            Param{"output_replace", "true"},
                            Param{"compute_level", "compute_root"})(
                            resize["output"]);
    Node gamma_correction = b.add("image_processing_gamma_correction_3d")(
        gamma,
        resize_s["output"]);
    Node denormalize = b.add("core_denormalize_3d_uint8")(
        gamma_correction["output"]);
    Node denormalize_s = b.add("internal_schedule")
                             .set_param(
                                 Param{"output_name", "denormalize"},
                                 Param{"output_replace", "true"},
                                 Param{"compute_level", "compute_root"})(
                                 denormalize["output"]);
    Node output = b.add("image_io_image_saver")
                      .set_param(
                          Param{"path", "output.png"},
                          Param{"width", std::to_string(output_width)},
                          Param{"height", std::to_string(output_height)})(
                          denormalize_s["output"]);

    Halide::Buffer<int32_t> obuf(std::vector<int>{});
    pm.set(output["output"], obuf);

    b.run(pm);

    return 0;
}
