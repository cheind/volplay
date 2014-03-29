cmake_minimum_required(VERSION 2.8)

project(volplay)

set(EIGEN_INCLUDE_DIR "../eigen" CACHE PATH "Where is the include directory of Eigen located")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")

# Setup library

set(VOLPLAY_CORE_FILES
    inc/volplay/types.h
    inc/volplay/fwd.h
    inc/volplay/sdf_result.h
    inc/volplay/sdf_node.h
    inc/volplay/sdf_unary.h
    inc/volplay/sdf_group.h
    inc/volplay/sdf_union.h
    inc/volplay/sdf_intersection.h
    inc/volplay/sdf_difference.h
    inc/volplay/sdf_rigid_transform.h
    inc/volplay/sdf_repetition.h
    inc/volplay/sdf_sphere.h
    inc/volplay/sdf_plane.h
    inc/volplay/sdf_making.h
    inc/volplay/camera.h
    inc/volplay/util/iterator_range.h
    src/sdf_node.cpp
    src/sdf_unary.cpp
    src/sdf_group.cpp
    src/sdf_union.cpp
    src/sdf_intersection.cpp
    src/sdf_difference.cpp
    src/sdf_rigid_transform.cpp
    src/sdf_repetition.cpp
    src/sdf_sphere.cpp
    src/sdf_plane.cpp
    src/sdf_making.cpp
    src/camera.cpp
)

set(VOLPLAY_UTIL_FILES
    inc/volplay/util/iterator_range.h
)

source_group(core FILES ${VOLPLAY_CORE_FILES})
source_group(util FILES ${VOLPLAY_UTIL_FILES})

include_directories(${EIGEN_INCLUDE_DIR})
include_directories(inc)

add_library(volplay ${VOLPLAY_CORE_FILES} ${VOLPLAY_UTIL_FILES})

# Setup tests

set(VOLPLAY_TEST_FILES
    tests/main.cpp
    tests/test_derivative.cpp
    tests/test_sdf_result.cpp
    tests/test_sdf_sphere.cpp
    tests/test_sdf_plane.cpp
    tests/test_sdf_union.cpp
    tests/test_sdf_intersection.cpp
    tests/test_sdf_difference.cpp
    tests/test_sdf_rigid_transform.cpp
    tests/test_sdf_repetition.cpp
    tests/test_ray_tracer.cpp
    tests/test_camera.cpp
)

source_group(tests FILES ${VOLPLAY_TEST_FILES})

include_directories(tests)
add_executable(volplay_tests ${VOLPLAY_TEST_FILES})
target_link_libraries(volplay_tests volplay)

# Setup examples

find_package(OpenCV)

set(VOLPLAY_EXAMPLE_FILES
    examples/main.cpp
)

if (OpenCV_FOUND)
    set(VOLPLAY_EXAMPLE_FILES
        ${VOLPLAY_EXAMPLE_FILES}
        examples/example_raytracer.cpp
    )
    include_directories(${OpenCV_INCLUDE_DIRS})
    link_libraries(${OpenCV_LIBS})
else ()
    message("OpenCV not found, skipping OpenCV related examples")
endif ()

source_group(examples FILES ${VOLPLAY_EXAMPLE_FILES})

include_directories(examples)
add_executable(volplay_examples ${VOLPLAY_EXAMPLE_FILES})
target_link_libraries(volplay_examples volplay)