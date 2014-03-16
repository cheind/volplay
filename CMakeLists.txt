cmake_minimum_required(VERSION 2.8)

project(volplay)

set(EIGEN_INCLUDE_DIR "../eigen" CACHE PATH "Where is the include directory of Eigen located")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")

# Setup library

set(VOLPLAY_CORE_FILES
    inc/volplay/types.h
    inc/volplay/fwd.h
    inc/volplay/sdf_node.h
    inc/volplay/sdf_group.h
    inc/volplay/sdf_union.h
    inc/volplay/sdf_intersection.h
    inc/volplay/sdf_difference.h
    inc/volplay/sdf_sphere.h
    src/sdf_group.cpp
    src/sdf_union.cpp
    src/sdf_intersection.cpp
    src/sdf_difference.cpp
    src/sdf_sphere.cpp
)

set(VOLPLAY_UTIL_FILES
    inc/volplay/util/iterator_range.h
)

source_group(core FILES ${VOLPLAY_CORE_FILES})
source_group(util FILES ${VOLPLAY_UTIL_FILES})

include_directories(${EIGEN_INCLUDE_DIR})
include_directories(inc)

add_library(volplay ${VOLPLAY_CORE_FILES} ${VOLPLAY_UTIL_FILES})

# Setup examples

set(VOLPLAY_EXAMPLE_FILES
    examples/main.cpp
)

source_group(examples FILES ${VOLPLAY_EXAMPLE_FILES})

include_directories(examples)
add_executable(volplay_examples ${VOLPLAY_EXAMPLE_FILES})
target_link_libraries(volplay_examples volplay)

# Setup tests

set(VOLPLAY_TEST_FILES
    tests/main.cpp
    tests/test_sdf_sphere.cpp
    tests/test_sdf_union.cpp
    tests/test_sdf_intersection.cpp
    tests/test_sdf_difference.cpp
)

source_group(tests FILES ${VOLPLAY_TEST_FILES})

include_directories(tests)
add_executable(volplay_tests ${VOLPLAY_TEST_FILES})
target_link_libraries(volplay_tests volplay)