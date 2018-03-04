########################################
# Copyright Adan Moran-MacDonald, 2018 #
########################################

add_library(MatlabProxy
	${CMAKE_CURRENT_SOURCE_DIR}/MatlabProxy.cpp
)
target_link_libraries(MatlabProxy
	${GET_SPARSE_LIB}
	${MATLAB_LIBS}
	${CANNY_LIB}
	${POLYA_TOOLS_LIB}
	Eigen
	Qt5::Widgets
)
target_include_directories(MatlabProxy PUBLIC
	${CMAKE_SOURCE_DIR}/matlab_libs/
	${CMAKE_SOURCE_DIR}/include/
	${CMAKE_SOURCE_DIR}/external/MATLAB/v93/extern/include
)

# vim: set ts=4 sw=4 et :