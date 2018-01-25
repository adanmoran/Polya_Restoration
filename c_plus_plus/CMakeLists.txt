########################################
# Copyright Adan Moran-MacDonald, 2018 #
########################################

cmake_minimum_required(VERSION 3.5)

project(PolyaRestoration)

include(${CMAKE_SOURCE_DIR}/cmake/DownloadEigen.cmake)

add_executable(Main main.cpp)
target_link_libraries(Main eigen)
# vim: set ts=4 sw=4 et :
