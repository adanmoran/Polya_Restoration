########################################
# Copyright Adan Moran-MacDonald, 2018 #
########################################

add_library(Polya
    polya.cpp
)

target_include_directories(Polya PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(Polya
    Eigen
)

# vim: set ts=4 sw=4 et :
