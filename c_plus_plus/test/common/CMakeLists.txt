########################################
# Copyright Adan Moran-MacDonald, 2018 #
########################################

add_executable(TestEigenHelpers
    TestEigenHelpers.cpp
)
target_include_directories(TestEigenHelpers PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(TestEigenHelpers
    Polya
    GTest
    GMock
)

add_test(NAME TestEigenHelpers COMMAND TestEigenHelpers)

# vim: set ts=4 sw=4 et :
