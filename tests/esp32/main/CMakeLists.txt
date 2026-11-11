get_filename_component(ANGULORA_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../.." ABSOLUTE)
file(GLOB ANGULORA_SOURCES "${ANGULORA_ROOT}/src/*.c")
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
