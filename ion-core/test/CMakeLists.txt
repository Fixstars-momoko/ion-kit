#
# ion-core Testing
#

# Test BB module
add_library(ion-bb-test SHARED ion-bb-test.cc)
if(UNIX)
    target_compile_options(ion-bb-test PUBLIC -fno-rtti)
endif()
target_link_libraries(ion-bb-test ion-core)

# C interface test
ion_jit(c_test SRCS c_test.c)
ion_register_test(c_test c_test)
add_dependencies(c_test ion-bb-test)

# Error case test
ion_jit(error SRCS error.cc)
ion_register_test(error error)

# Metadata test
ion_jit(metadata SRCS metadata.cc)
ion_register_test(metadata metadata)
add_dependencies(metadata ion-bb-test)

# Multi-out test
ion_jit(multi_out SRCS multi_out.cc)
ion_register_test(multi_out multi_out)

# Array In/Out test
ion_jit(array_inout SRCS array_inout.cc)
ion_register_test(array_inout array_inout)

# Array Input test
ion_jit(array_input SRCS array_input.cc)
ion_register_test(array_input array_input)

# Array Output test
ion_jit(array_output SRCS array_output.cc)
ion_register_test(array_output array_output)

# Duplicate array names test
ion_jit(array_dup_names SRCS array_dup_names.cc)
ion_register_test(array_dup_names array_dup_names)

# Inverted dep test
ion_jit(inverted_dep SRCS inverted_dep.cc)
ion_register_test(inverted_dep inverted_dep)
add_dependencies(inverted_dep ion-bb-test)

# Duplicate port
ion_jit(dup SRCS dup.cc)
ion_register_test(dup dup)

# Output error handling
ion_jit(port-binding SRCS port-binding.cc)
ion_register_test(port-binding port-binding)

# Export test
ion_jit(export SRCS export.cc)
ion_register_test(export export)

#
# Pipeline testing
#
ion_compile(simple_graph_compile SRCS simple_graph_compile.cc PIPELINE_NAME simple_graph)
ion_run(simple_graph_aot_test simple_graph_compile SRCS simple_graph_run.cc test-rt.h TARGET_STRING "host-profile")
ion_jit(simple_graph_jit SRCS simple_graph_jit.cc test-rt.h)
ion_register_test(simple_graph_jit_test simple_graph_jit TARGET_STRING "host-profile")

ion_compile(complex_graph_compile SRCS complex_graph_compile.cc PIPELINE_NAME complex_graph)
ion_run(complex_graph_aot_test complex_graph_compile SRCS complex_graph_run.cc test-rt.h TARGET_STRING "host-profile")
ion_jit(complex_graph_jit SRCS complex_graph_jit.cc test-rt.h)
ion_register_test(complex_graph_jit_test complex_graph_jit TARGET_STRING "host-profile")

if (UNIX)
    install(TARGETS ion-bb-test DESTINATION lib)
endif()
