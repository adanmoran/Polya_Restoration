########################################
# Copyright Adan Moran-MacDonald, 2018 #
########################################

add_executable(TestPolya
    TestPolya.cpp
)
target_include_directories(TestPolya PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(TestPolya
    Polya
    GTest
    GMock
)

add_test(NAME TestPolya COMMAND TestPolya)

# vim: set ts=4 sw=4 et :
