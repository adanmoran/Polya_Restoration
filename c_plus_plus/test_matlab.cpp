#include "matlab/MatlabProxy.h"
#include <iostream>
int run_main(int argc, char** argv)
{
	std::cout << "Initializing mcl..." << std::endl;

	if (!mclInitializeApplication(NULL, 0))
	{
		std::cerr << "Could not initialize the application" << std::endl;
		return -1;
	}
	
	//Load the MATLAB code into the runtime
	if (!get_sparse_adjInitialize())
	{
		std::cerr << "Could not initialize the get_sparse_adj application" << std::endl;
	}

	std::cout << "Initialized!" << std::endl;

	try
	{
		// Create data for our get_sparse_adj inputs
		// column-major size vector
		mwArray rc(1, 2, mxDOUBLE_CLASS, mxREAL);
		double sizes[] = { 4, 4 };
		rc.SetData(sizes, 2);
		// other inputs
		mwArray radius(mxDouble(1));
		mwArray p_norm(mxDouble(1));

		// output element
		mwArray adj;
		std::cout << "Calling get_sparse_adj with rc = " << rc << ", radius = " << radius << ", P = " << p_norm << std::endl;
		get_sparse_adj(1, adj, rc, radius, p_norm);

		std::cout << "The adjacency matrix for a 3x3 is: " << adj << std::endl;
	}
	catch (const mwException& e)
	{
		std::cerr << e.what() << std::endl;
		return -2;
	}

	// Terminate the get_sparse_adj  (must be done before MATLAB Runtime)
	get_sparse_adjTerminate();
	//Release  all resources used by the MATLAB Runtime
	mclTerminateApplication();
	return 0;
}

int main()
{
	// Initialize the MATLAB Runtime
	mclmcrInitialize();

	return mclRunMain(reinterpret_cast<mclMainFcnType>(run_main), 0, NULL);
}

/* vim: set ts=4 sw=4 et : */