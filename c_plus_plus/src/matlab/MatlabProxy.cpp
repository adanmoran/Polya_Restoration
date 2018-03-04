//////////////////////////////////////
// Copyright MTHE 493 Group 8, 2018 //
//////////////////////////////////////
#include <iostream>
#include "matlab/MatlabProxy.h"
#include "common/EigenHelpers.h"

MatlabProxy::MatlabProxy()
: open_(false)
, mclInitialized_(false)
{}

MatlabProxy::~MatlabProxy()
{
	terminate();
}

auto MatlabProxy::Initialize() -> bool
{
	std::cout << "Initializing code for MATLAB" << std::endl;
	// Load the MATLAB Runtime
	if (!mclInitializeApplication(NULL, 0))
	{
		std::cerr << "Could not initialize the application" << std::endl;
		return false;
	}
	mclInitialized_ = true;

	//Load the function code into the MATLAB Runtime
	if (!get_sparse_adjInitialize())
	{
		std::cerr << "Could not initialize the get_sparse_adj application" << std::endl;
		return false;
	}
	open_ = true;

	std::cout << "Initialized!" << std::endl;
	return open_;
}

auto MatlabProxy::getSparseAdj(const MatrixSize& rc, size_t radius, MatlabProxy::PNorm norm) -> AdjacencyMatrix
{
	if (open_)
	{
		try
		{
			//Set the rc input to the get_sparse_adj function
			mwArray rowcol(mwSize(1), mwSize(2), mxDOUBLE_CLASS, mxREAL);
			mxDouble sizes[] = { mxDouble(rc.rows), mxDouble(rc.cols) };
			rowcol.SetData(sizes, 2);

			// Set the radius
			mwArray rad(mxDouble(static_cast<int>(radius)));
			
			// Set the norm. Note that allowable values are 1, 2, or 'inf'.
			mwArray p_norm;
			switch (norm)
			{
			case MatlabProxy::PNorm::INF:
				// TODO: set p_norm to 'inf'
			default:
				p_norm = mwArray(mxDouble(static_cast<int>(norm)));
				break;
			}

			// output element
			mwArray adj;
			std::cout << "Calling get_sparse_adj with rc = " << rowcol << ", radius = " << rad << ", P = " << p_norm << std::endl;
			get_sparse_adj(1, adj, rowcol, rad, p_norm);

			std::cout << "The adjacency matrix for a 3x3 is: " << adj << std::endl;


			// Get the row indices of the non zero elements
			mwArray adjRows = adj.RowIndex();
			mwArray adjCols = adj.ColumnIndex();

			// Create the adjacency matrix elements
			Triplets<int> adjacencyValues;
			//Allocate for the number of non zeros in the adjacency matrix returned
			adjacencyValues.reserve(adj.MaximumNonZeros());

			// MATLAB indices start at 1
			for (int i = 1; i <= adj.MaximumNonZeros(); ++i)
			{
				// Eigen indices start at 0
				adjacencyValues.push_back({ static_cast<int>(adjRows.Get(1,i)) - 1, static_cast<int>(adjCols.Get(1,i)) - 1, 1});
			}

			// Construct the adjacency matrix. It is square, so we read one dimension of it to build it.
			AdjacencyMatrix eigenAdjacency = createSquareMatrix(static_cast<int>(adj.GetDimensions().Get(1, 1)), adjacencyValues);
			std::cout << Eigen::MatrixXi(eigenAdjacency) << std::endl;
		}
		catch (const mwException& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	// Return a 1x1 empty adjacency matrix
	return AdjacencyMatrix(1);
}

auto MatlabProxy::terminate() -> void
{
	if (open_)
	{
		std::cout << "Terminating get_sparse_adj" << std::endl;
		// Terminate the get_sparse_adj  (must be done before MATLAB Runtime)
		get_sparse_adjTerminate();
		open_ = false;
	}

	if (mclInitialized_)
	{
		std::cout << "Terminating mclTerminateApplication" << std::endl;
		//Release  all resources used by the MATLAB Runtime
		mclTerminateApplication();
		mclInitialized_ = false;
	}

}

/* vim: set ts=4 sw=4 et : */