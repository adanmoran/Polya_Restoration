########################################
# Copyright Adan Moran-MacDonald, 2018 #
########################################

cmake_minimum_required(VERSION 3.5)

project(PolyaRestoration)

#########################
# C++-Oriented Commands #
#########################
# Set the compiler to use C++14
set(CMAKE_CXX_STANDARD 14)

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
# Define where we can find the Qt installation for this OS
if(APPLE)
    set(QT_COMPILER clang_64)
    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${PROJECT_SOURCE_DIR}/external/Qt5.10/5.10.0/${QT_COMPILER})
elseif(UNIX)
    set(QT_COMPILER gcc_64)
endif()

# Append to CMake Modules to find Qt in the downloaded directory
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/external/Qt5.10/5.10.0/${QT_COMPILER}/lib/cmake)

message("CMAKE_MODULE_PATH is now ${CMAKE_MODULE_PATH}")
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)

##############
# Polya Code #
##############

add_executable(Main main.cpp)
target_include_directories(Main PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(Main
    Eigen
    Qt5::Widgets
)

add_subdirectory(src)

# vim: set ts=4 sw=4 et :

