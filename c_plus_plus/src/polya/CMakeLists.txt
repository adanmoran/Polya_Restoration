add_library(Polya
    polya.cpp
)

target_include_directories(Polya PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(Polya
    Eigen
)
