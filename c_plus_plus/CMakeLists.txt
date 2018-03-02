########################################
# Copyright Adan Moran-MacDonald, 2018 #
########################################

cmake_minimum_required(VERSION 3.5)

project(PolyaRestoration)

include(CTest)

#########################
# C++-Oriented Commands #
#########################
# Set the compiler to use C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE "Release")
cmake_policy(SET CMP0071 NEW)

#########
# GTest #
#########
if(NOT WIN32)
include(${CMAKE_SOURCE_DIR}/cmake/DownloadGTest.cmake)
endif()

#########
# Eigen #
#########

# Append to CMake Modules to find eigen in the downloaded directory
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/external/eigen/cmake)

# Get Eigen as a package (from its installed location)
if(APPLE OR WIN32)
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
elseif(UNIX)
    set(QT_COMPILER gcc_64)
elseif("${MSVC_VERSION}" STREQUAL 1900)
	set(QT_COMPILER msvc2015_64)
else()
	if("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)") # 64-bit windows
		if(MSVC_VERSION STREQUAL 1900)
			set(QT_COMPILER msvc2015_64)
		else()
			set(QT_COMPILER msvc2017_64)
		endif()
	else() # 32-bit windows
		if(MSVC_VERSION STREQUAL 1900)
			set(QT_COMPILER msvc2015)
		else()
			set(QT_COMPILER msvc2017)
		endif()
	endif()
endif()

# Append to CMAKE_PREFIX_PATH to find the correct find_scripts
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${PROJECT_SOURCE_DIR}/external/Qt5.10/5.10.0/${QT_COMPILER})
# Append to CMake Modules to find Qt in the downloaded directory
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/external/Qt5.10/5.10.0/${QT_COMPILER}/lib/cmake)

message(STATUS "CMAKE_MODULE_PATH is now ${CMAKE_MODULE_PATH}")
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5Core)

##############
# Polya Code #
##############

# notice that you need to pass the *header* here, not the source file
file(GLOB QT_HEADERS ${CMAKE_SOURCE_DIR}/include/qt/*.h)
QT5_WRAP_CPP(MY_MOCED_FILES ${QT_HEADERS})

add_executable(Main WIN32 main.cpp ${MY_MOCED_FILES})
target_include_directories(Main PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(Main
    Eigen
    Qt5::Widgets
    ImageViewer
    Qt5::Gui
)

add_executable(TestMatlab
	${CMAKE_CURRENT_SOURCE_DIR}/test_matlab.cpp
)
file(GLOB MATLAB_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/external/MATLAB/v93/extern/lib/win64/microsoft/*)

# find_library (MATLAB_RUNTIME libmx 
    # PATHS ${CMAKE_CURRENT_SOURCE_DIR}/external/MATLAB/v93/bin/win64/
    # NO_DEFAULT_PATH 
# )
# find_library (MATLAB_MCL mclmcrrt9_3
	# PATHS ${CMAKE_CURRENT_SOURCE_DIR}/external/MATLAB/v93/runtime/win64/
	# NO_DEFAULT_PATH
# )
find_library (GET_SPARSE_LIB get_sparse_adj
	PATHS ${CMAKE_CURRENT_SOURCE_DIR}/matlab_libs/
	NO_DEFAULT_PATH
)
message(STATUS "Found sparse lib at ${GET_SPARSE_LIB}")
#message(STATUS "Found Matlab mcl lib at ${MATLAB_MCL}")
#message(STATUS "Found Matlab runtime lib at ${MATLAB_RUNTIME}")
message(STATUS "Found Matlab libs at ${MATLAB_LIBS}")

target_link_libraries(TestMatlab
	${GET_SPARSE_LIB}
	${MATLAB_LIBS}
	#${MATLAB_MCL}
	#${MATLAB_RUNTIME}
)
target_include_directories(TestMatlab PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/matlab_libs/
	${CMAKE_CURRENT_SOURCE_DIR}/external/MATLAB/v93/extern/include
)

# Source Code
add_subdirectory(src)
if(NOT WIN32)
# Google Test Scripts
add_subdirectory(test)
endif()

# vim: set ts=4 sw=4 et :

