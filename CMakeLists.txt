cmake_minimum_required(VERSION 3.20)
project(angulora VERSION 0.1.0 LANGUAGES C)
option(ANGULORA_BUILD_TESTS 'Build migrated tests' ON)
file(GLOB ANGULORA_SOURCES CONFIGURE_DEPENDS src/*.c)
add_library(angulora STATIC ${ANGULORA_SOURCES})
target_include_directories(angulora PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/angulora/trigonometry)
target_compile_features(
angulora
 PUBLIC c_std_99)
if(ANGULORA_BUILD_TESTS)
 enable_testing()
 file(GLOB ANGULORA_TESTS CONFIGURE_DEPENDS tests/*.c)
 foreach(test_source IN LISTS 
ANGULORA
_TESTS)
  get_filename_component(test_name "${test_source}" NAME_WE)
  add_executable(angulora_${test_name} ${test_source})
  target_include_directories(angulora_${test_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/angulora/trigonometry)
  target_link_libraries(
angulora
_${test_name} PRIVATE 
angulora
)
  add_test(NAME "${test_name}" COMMAND "angulora_${test_name}")
 endforeach()
endif()
