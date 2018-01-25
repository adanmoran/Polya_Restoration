########################################
# Copyright Adan Moran-MacDonald, 2018 #
########################################

cmake_minimum_required(VERSION 3.5)

project(PolyaRestoration)

#########################
# C++-Oriented Commands #
#########################
set(CMAKE_CXX_FLAGS "-std=c++11")

#########
# Eigen #
#########

# Append to CMake Modules to find eigen in the downloaded directory
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/external/eigen/cmake)

# Get Eigen as a package (from its installed location)
if(APPLE)
    # This is so Alfred can find eigen, since his Mac can't do find_package
    set(EIGEN3_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/eigen)
else()
    find_package(Eigen3 REQUIRED)
endif()

# Create an interface to "link" the eigen headers to the targets that need it
add_library(Eigen INTERFACE)
target_include_directories(Eigen INTERFACE
    ${EIGEN3_INCLUDE_DIR}
)

######
# Qt #
######


##############
# Polya Code #
##############

add_executable(Main main.cpp)
target_link_libraries(Main
    Eigen
)

add_subdirectory(src)

# vim: set ts=4 sw=4 et :

