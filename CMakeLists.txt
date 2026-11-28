cmake_minimum_required(VERSION 3.16)
project(Angulora VERSION 0.2.0 LANGUAGES C)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(ANGULORA_BUILD_TESTS "Build Angulora validation tests" OFF)
option(ANGULORA_BUILD_EXAMPLES "Build Angulora examples" OFF)
option(ANGULORA_STRICT_WARNINGS "Treat supported compiler warnings as errors" ON)

function(angulora_apply_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
        if(ANGULORA_STRICT_WARNINGS)
            target_compile_options(${target} PRIVATE /WX)
        endif()
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
        if(ANGULORA_STRICT_WARNINGS)
            target_compile_options(${target} PRIVATE -Werror)
        endif()
    endif()
endfunction()

add_library(angulora STATIC
    src/angulora_bearing2_q.c
    src/angulora_cartesian_to_polar_q.c
    src/angulora_cpx_to_polar_q.c
    src/angulora_mod_phase_unwrap_i32.c
    src/angulora_phase_to_tick_u32.c
    src/angulora_q_hypot2.c
    src/angulora_stepper_angle_from_steps_q.c
    src/angulora_stepper_microstep_phase_u32.c
)
add_library(Angulora::Angulora ALIAS angulora)
target_compile_features(angulora PUBLIC c_std_99)
set_target_properties(angulora PROPERTIES
    C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO EXPORT_NAME Angulora)
target_include_directories(angulora PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
angulora_apply_warnings(angulora)

if(ANGULORA_BUILD_EXAMPLES)
    add_executable(angulora_example_minimal examples/minimal.c)
    target_link_libraries(angulora_example_minimal PRIVATE Angulora::Angulora)
    target_compile_features(angulora_example_minimal PRIVATE c_std_99)
    set_target_properties(angulora_example_minimal PROPERTIES
        C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    angulora_apply_warnings(angulora_example_minimal)
endif()

if(ANGULORA_BUILD_TESTS)
    enable_testing()
    enable_language(CXX)
    foreach(test_name IN ITEMS audit to_polar)
        add_executable(angulora_test_${test_name} tests/test_${test_name}.c)
        target_link_libraries(angulora_test_${test_name} PRIVATE Angulora::Angulora)
        target_compile_features(angulora_test_${test_name} PRIVATE c_std_99)
        set_target_properties(angulora_test_${test_name} PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        angulora_apply_warnings(angulora_test_${test_name})
        if(UNIX AND NOT APPLE)
            target_link_libraries(angulora_test_${test_name} PRIVATE m)
        endif()
        add_test(NAME angulora.${test_name} COMMAND angulora_test_${test_name})
    endforeach()

    set(ANGULORA_PUBLIC_HEADERS
        trigonometry/bearing2_q.h
        trigonometry/cartesian_to_polar_q.h
        trigonometry/cpx_q.h
        trigonometry/cpx_to_polar_q.h
        trigonometry/mod_phase_unwrap_i32.h
        trigonometry/phase_to_tick_u32.h
        trigonometry/q_hypot2.h
        trigonometry/stepper_angle_from_steps_q.h
        trigonometry/stepper_microstep_phase_u32.h
    )
    foreach(header IN LISTS ANGULORA_PUBLIC_HEADERS)
        string(REPLACE "/" "_" header_test_name "${header}")
        string(REPLACE ".h" "" header_test_name "${header_test_name}")
        set(header_smoke_source "${CMAKE_CURRENT_BINARY_DIR}/header_${header_test_name}.c")
        file(WRITE "${header_smoke_source}" "#include <angulora/${header}>\nint main(void) { return 0; }\n")
        add_executable(angulora_header_${header_test_name} "${header_smoke_source}")
        target_link_libraries(angulora_header_${header_test_name} PRIVATE Angulora::Angulora)
        target_compile_features(angulora_header_${header_test_name} PRIVATE c_std_99)
        set_target_properties(angulora_header_${header_test_name} PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        angulora_apply_warnings(angulora_header_${header_test_name})
        add_test(NAME angulora.header.${header_test_name} COMMAND angulora_header_${header_test_name})
    endforeach()

    set(ANGULORA_CPP_SMOKE "")
    foreach(header IN LISTS ANGULORA_PUBLIC_HEADERS)
        string(APPEND ANGULORA_CPP_SMOKE "#include <angulora/${header}>\n")
    endforeach()
    string(APPEND ANGULORA_CPP_SMOKE
        "int main() { uint32_t phase = 1u; uint32_t ticks = 0u; return angulora_phase_to_tick_u32(phase, 4u, &ticks) && ticks == 0u ? 0 : 1; }\n")
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/headers_cpp.cpp" "${ANGULORA_CPP_SMOKE}")
    add_executable(angulora_headers_cpp "${CMAKE_CURRENT_BINARY_DIR}/headers_cpp.cpp")
    target_link_libraries(angulora_headers_cpp PRIVATE Angulora::Angulora)
    angulora_apply_warnings(angulora_headers_cpp)
    add_test(NAME angulora.headers_cpp COMMAND angulora_headers_cpp)
endif()

install(TARGETS angulora EXPORT AnguloraTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT AnguloraTargets FILE AnguloraTargets.cmake NAMESPACE Angulora::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Angulora)
configure_package_config_file(cmake/AnguloraConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/AnguloraConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Angulora)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/AnguloraConfigVersion.cmake
    VERSION ${PROJECT_VERSION} COMPATIBILITY SameMinorVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/AnguloraConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/AnguloraConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Angulora)
