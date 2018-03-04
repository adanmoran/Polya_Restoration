#include "matlab/MatlabProxy.h"
#include <iostream>
int run_main(int argc, char** argv)
{
	MatlabProxy mp;
	mp.Initialize();
	auto a = mp.getSparseAdj({ 3,3 }, 1, MatlabProxy::PNorm::ONE);

	return 0;
}

int main()
{
	// Initialize the MATLAB Runtime
	mclmcrInitialize();

	return mclRunMain(reinterpret_cast<mclMainFcnType>(run_main), 0, NULL);
}

/* vim: set ts=4 sw=4 et : */