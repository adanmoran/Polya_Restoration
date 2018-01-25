cmake_minimum_required(VERSION 3.0.0)

project(eigen-download NONE)

include(${CMAKE_SOURCE_DIR}/cmake/DownloadProject.cmake)

download_project(PROJ                eigen
	         GIT_REPOSITORY      https://github.com/RLovelett/eigen
	         GIT_TAG             3.3.4
	         UPDATE_DISCONNECTED 1
)  

add_subdirectory(${eigen_SOURCE_DIR} ${eigen_BINARY_DIR})
