########################################
# Copyright Adan Moran-MacDonald, 2018 #
########################################

cmake_minimum_required(VERSION 3.5)

project(PolyaRestoration)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/external/eigen/cmake)

# Get Eigen as a package (from its installed location)
find_package(Eigen3 REQUIRED)
add_library(Eigen INTERFACE)
target_include_directories(Eigen INTERFACE
    ${EIGEN3_INCLUDE_DIR}
)

# Now do our stuff

add_executable(Main main.cpp)
target_link_libraries(Main
    Eigen
)
# vim: set ts=4 sw=4 et :
