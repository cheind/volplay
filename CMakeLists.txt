cmake_minimum_required(VERSION 2.8)

project(volplay)

set(EIGEN_INCLUDE_DIR "../eigen" CACHE PATH "Where is the include directory of Eigen located")

if (MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else ()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
endif ()

# Setup externals

find_package(OpenCV)

if (OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
    link_libraries(${OpenCV_LIBS})
    add_definitions(-DVOLPLAY_WITH_OPENCV)
else ()
    message("OpenCV not found")
endif ()

# Setup library

set(VOLPLAY_CORE_FILES
    inc/volplay/fwd.h
    inc/volplay/types.h
    inc/volplay/sdf_result.h
    inc/volplay/sdf_node.h
    inc/volplay/sdf_node_attachment.h
    inc/volplay/sdf_unary.h
    inc/volplay/sdf_group.h
    inc/volplay/sdf_union.h
    inc/volplay/sdf_intersection.h
    inc/volplay/sdf_difference.h
    inc/volplay/sdf_rigid_transform.h
    inc/volplay/sdf_repetition.h
    inc/volplay/sdf_sphere.h
    inc/volplay/sdf_box.h
    inc/volplay/sdf_plane.h
    inc/volplay/sdf_make.h
    src/sdf_node.cpp
    src/sdf_node_attachment.cpp
    src/sdf_unary.cpp
    src/sdf_group.cpp
    src/sdf_union.cpp
    src/sdf_intersection.cpp
    src/sdf_difference.cpp
    src/sdf_rigid_transform.cpp
    src/sdf_repetition.cpp
    src/sdf_sphere.cpp
    src/sdf_box.cpp
    src/sdf_plane.cpp
    src/sdf_make.cpp
)

set(VOLPLAY_RENDERING_FILES
    inc/volplay/rendering/camera.h
    inc/volplay/rendering/image.h
    inc/volplay/rendering/saturate.h
    inc/volplay/rendering/light.h
    inc/volplay/rendering/material.h
    inc/volplay/rendering/renderer.h
    inc/volplay/rendering/image_generator.h
    inc/volplay/rendering/heat_image_generator.h
    inc/volplay/rendering/depth_image_generator.h
    inc/volplay/rendering/blinn_phong_image_generator.h
    inc/volplay/rendering/fxaa.h
    src/rendering/renderer.cpp
    src/rendering/camera.cpp
    src/rendering/light.cpp
    src/rendering/material.cpp
    src/rendering/heat_image_generator.cpp
    src/rendering/depth_image_generator.cpp
    src/rendering/blinn_phong_image_generator.cpp
    src/rendering/fxaa.cpp
)

set(VOLPLAY_SURFACE_FILES
	inc/volplay/surface/types.h
    inc/volplay/surface/dual_contouring.h
	inc/volplay/surface/off_export.h
	src/surface/dual_contouring.cpp
	src/surface/off_export.cpp
)

set(VOLPLAY_UTIL_FILES
    inc/volplay/util/iterator_range.h
	inc/volplay/util/function_output_iterator.h
	inc/volplay/util/voxel_grid.h
)

source_group(core FILES ${VOLPLAY_CORE_FILES})
source_group(rendering FILES ${VOLPLAY_RENDERING_FILES})
source_group(util FILES ${VOLPLAY_UTIL_FILES})
source_group(surface FILES ${VOLPLAY_SURFACE_FILES})

include_directories(${EIGEN_INCLUDE_DIR})
include_directories(inc)

add_library(volplay ${VOLPLAY_CORE_FILES} ${VOLPLAY_RENDERING_FILES} ${VOLPLAY_UTIL_FILES} ${VOLPLAY_SURFACE_FILES})

# Setup tests

set(VOLPLAY_TEST_FILES
    tests/main.cpp
    tests/test_sdf_result.cpp
    tests/test_sdf_node.cpp
    tests/test_sdf_sphere.cpp
    tests/test_sdf_box.cpp
    tests/test_sdf_plane.cpp
    tests/test_sdf_union.cpp
    tests/test_sdf_intersection.cpp
    tests/test_sdf_difference.cpp
    tests/test_sdf_rigid_transform.cpp
    tests/test_sdf_repetition.cpp
    tests/test_camera.cpp
    tests/test_image.cpp
    tests/test_saturate.cpp
    tests/test_voxel_grid.cpp
)

source_group(tests FILES ${VOLPLAY_TEST_FILES})

include_directories(tests)
add_executable(volplay_tests ${VOLPLAY_TEST_FILES})
target_link_libraries(volplay_tests volplay)

# Setup examples

set(VOLPLAY_EXAMPLE_FILES
    examples/main.cpp
)

if(OpenCV_FOUND)
    set(VOLPLAY_EXAMPLE_FILES
        ${VOLPLAY_EXAMPLE_FILES}
        examples/example_raytracer.cpp		
		examples/example_surface_export.cpp
    )
endif()

source_group(examples FILES ${VOLPLAY_EXAMPLE_FILES})

include_directories(examples)
add_executable(volplay_examples ${VOLPLAY_EXAMPLE_FILES})
target_link_libraries(volplay_examples volplay)