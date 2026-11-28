get_filename_component(ANGULORA_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../.." ABSOLUTE)
set(ANGULORA_SOURCES
    "${ANGULORA_ROOT}/src/angulora_bearing2_q.c"
    "${ANGULORA_ROOT}/src/angulora_cartesian_to_polar_q.c"
    "${ANGULORA_ROOT}/src/angulora_cpx_to_polar_q.c"
    "${ANGULORA_ROOT}/src/angulora_mod_phase_unwrap_i32.c"
    "${ANGULORA_ROOT}/src/angulora_phase_to_tick_u32.c"
    "${ANGULORA_ROOT}/src/angulora_q_hypot2.c"
    "${ANGULORA_ROOT}/src/angulora_stepper_angle_from_steps_q.c"
    "${ANGULORA_ROOT}/src/angulora_stepper_microstep_phase_u32.c")
set(ANGULORA_AUDIT_SOURCE "${ANGULORA_ROOT}/tests/test_audit.c")

idf_component_register(
    SRCS "main.c" "${ANGULORA_AUDIT_SOURCE}" ${ANGULORA_SOURCES}
    INCLUDE_DIRS "${ANGULORA_ROOT}/include/angulora/trigonometry")

set_source_files_properties(${ANGULORA_SOURCES} "${ANGULORA_AUDIT_SOURCE}"
    PROPERTIES COMPILE_OPTIONS "-std=gnu99;-O2;-Wall;-Wextra;-Werror")
set_source_files_properties("${ANGULORA_AUDIT_SOURCE}"
    PROPERTIES COMPILE_DEFINITIONS "main=angulora_audit_test_main")
set_source_files_properties("main.c"
    PROPERTIES COMPILE_OPTIONS "-std=gnu99;-O2;-Wall;-Wextra;-Werror")
