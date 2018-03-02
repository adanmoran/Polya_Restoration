########################################
# Copyright Adan Moran-MacDonald, 2018 #
########################################

add_subdirectory(polya)
add_subdirectory(matlab)


message(${CMAKE_CURRENT_SOURCE_DIR})
add_library(ImageViewer
    ${CMAKE_CURRENT_SOURCE_DIR}/qt/imageviewer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/qt/slidersgroup.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qt/NoiseToolbar.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/qt/EdgeToolbar.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/qt/QuantizeToolbar.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/qt/PolyaToolbar.cpp
)
target_include_directories(ImageViewer PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(ImageViewer
    Qt5::Widgets
	Qt5::Core
)
# vim: set ts=4 sw=4 et :
