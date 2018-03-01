
# We need thread support
find_package(Threads REQUIRED)

# Enable ExternalProject CMake module
include(ExternalProject)

# Download and install GoogleTest
ExternalProject_Add(
    gtest
    URL https://github.com/google/googletest/archive/master.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    # Disable install step
    INSTALL_COMMAND ""
)

# Get GTest source and binary directories from CMake project
ExternalProject_Get_Property(gtest source_dir binary_dir)

# Create a libgtest target to be used as a dependency by test programs
add_library(GTest IMPORTED STATIC GLOBAL)
add_dependencies(GTest 
    gtest
)

if(WIN32)
set_target_properties(GTest PROPERTIES
	"IMPORTED LOCATION" "${binary_dir}/googlemock/gtest/Release/gtest_main.lib"
	"IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"     
		"${binary_dir}/googlemock/gtest/Release/gtest_main.lib"
		"${binary_dir}/googlemock/gtest/Release/gtest.lib"
)
else()
# Set libgtest properties
set_target_properties(GTest PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/googlemock/gtest/libgtest.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)
endif()

# Create a libgmock target to be used as a dependency by test programs
add_library(GMock IMPORTED STATIC GLOBAL)
add_dependencies(GMock 
    gtest
)
if(WIN32)
set_target_properties(GMock PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/googlemock/Release/gmock_main.lib"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
		"${binary_dir}/googlemock/Release/gmock_main.lib"
		"${binary_dir}/googlemock/Release/gmock.lib"
)
else()
# Set libgmock properties
set_target_properties(GMock PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/googlemock/libgmock.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)
endif()

# I couldn't make it work with INTERFACE_INCLUDE_DIRECTORIES
include_directories(
    "${source_dir}/googletest/include"
    "${source_dir}/googlemock/include"
)
					


# vim: set ts=4 sw=4 et :
