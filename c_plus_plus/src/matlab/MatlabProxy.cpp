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
	if (!polyaToolsInitialize())
	{
		std::cerr << "Could not initialize the polya tools library." << std::endl;
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
				p_norm = mwArray(INF_STR.c_str());
				break;
				// TODO: set p_norm to 'inf'
			default:
				p_norm = mwArray(mxDouble(static_cast<int>(norm)));
				break;
			}

			// output element
			mwArray adj;
			get_sparse_adj(1, adj, rowcol, rad, p_norm);

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
			return eigenAdjacency;
		}
		catch (const mwException& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	// Return a 1x1 empty adjacency matrix
	return AdjacencyMatrix(1);
}

auto MatlabProxy::addNoise(QImage* image, Prefs prefs, Noise noise) -> bool
{
	// If the pointer is null, we can't do anything!
	if (!image)
	{
		return false;
	}
	// We can only run the polya code if the library is open
	if (!open_)
	{
		return false;
	}
	
	// Convert the image to a MATLAB mwArray
	auto imageMatrix = toMATLABArray(*image, prefs.image.type);
	*image = toQImage(imageMatrix);


	// TODO: Convert the noise struct to an mwArray

	// Create a "prefs" struct as required by MATLAB which contains the required elements

	//TODO: call add_noise from MATLAB with the image and the noise
	
	return true;
}

auto MatlabProxy::terminate() -> void
{
	if (open_)
	{
		std::cout << "Terminating get_sparse_adj" << std::endl;
		// Terminate the get_sparse_adj  (must be done before MATLAB Runtime)
		get_sparse_adjTerminate();

		std::cout << "Terminating polyaTools" << std::endl;
		polyaToolsTerminate();
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

///////////////////////
// Private Functions //
///////////////////////

auto MatlabProxy::toMATLABArray(const QImage& image, Prefs::ImageType type) -> mwArray
{
	// Compute the desired mwArray size
	int depth = 1;
	int width = image.width();
	int height = image.height();
	if (type == Prefs::ImageType::RGB)
	{
		depth = 3;
	}

	// Allocate the memory that will contain the data for the mwArray.
	std::vector<mxDouble> columnMajorData;
	columnMajorData.reserve(depth * width * height);

	// Fill the vector with the colour values
	for (int k = 0; k < depth; ++k)
	{
		for (int row = 0; row < height; ++row)
		{
			auto line = reinterpret_cast<const QRgb*>(image.scanLine(row));
			for (int col = 0; col < width; ++col)
			{
				// Filling in the vector column-wise while iterating row-wise means we need to
				// do these jumps
				int arrayPosition = k*width*height + row*height + col;
				QRgb colour = line[col];
//				std::cout << "The colour is: " << colour << std::endl;
				switch (depth)
				{
				case static_cast<int>(ColorDepth::RED) :
					columnMajorData[arrayPosition] = mxDouble(qRed(colour));
//					std::cout << "The Red colour is: " << columnMajorData[arrayPosition] << std::endl;
					break;
				case static_cast<int>(ColorDepth::GREEN) :
					columnMajorData[arrayPosition] = mxDouble(qGreen(colour));
//					std::cout << "The Green colour is: " << columnMajorData[arrayPosition] << std::endl;
					break;
				case static_cast<int>(ColorDepth::BLUE) :
					columnMajorData[arrayPosition] = mxDouble(qBlue(colour));
//					std::cout << "The Blue colour is: " << columnMajorData[arrayPosition] << std::endl;
					break;
				}
			}
		}
	}

	// Create an mwArray from the generated data above
	mwSize imMatrixSize[] = { width, height, depth };
	mwArray imMatrix(3, imMatrixSize, mxDOUBLE_CLASS);
	imMatrix.SetData(columnMajorData.data(), width*height);
//	std::cout << "The Image is: " << imMatrix << std::endl;

	return imMatrix;
}

auto MatlabProxy::toQImage(const mwArray& imageMatrix)->QImage
{
	auto dimensions = imageMatrix.GetDimensions();
	auto numDims = dimensions.NumberOfElements();

	// Make sure the image is of the form n x m or n x m x 3
	if (numDims < 2 || numDims > 3)
	{
		std::cout << "The image has unexpected number of dimensions: " << numDims << std::endl;
		return QImage();
	}
	if (numDims == 3 && static_cast<int>(dimensions.Get(1, 3)) != 3)
	{
		std::cout << "The RGB image is not of the form RGB - it is an n x m x " << static_cast<int>(dimensions.Get(1, 3)) << " matrix." << std::endl;
		return QImage();
	}

	bool rgb = numDims == 3;

	int width = static_cast<int>(dimensions.Get(1, 1));
	int height = static_cast<int>(dimensions.Get(1, 2));
	int depth = 1;
	if (rgb)
	{
		depth = 3;
	}

	// Convert the mwArray to a QImage with 24
	QImage image(width, height, QImage::Format::Format_RGB32);
	// MATLAB arrays are 1-based
	for (int i = 1; i <= width; ++i)
	{
		for (int j = 1; j <= width; ++j)
		{
			QColor colour;

			// If we have a 3D matrix, write the colours as desired
			if (rgb)
			{
				colour.setRed(imageMatrix(i, j, 1));
				colour.setBlue(imageMatrix(i, j, 2));
				colour.setGreen(imageMatrix(i, j, 3));
			}
			else
			{
				colour.setRed(imageMatrix(i, j));
				colour.setGreen(imageMatrix(i, j));
				colour.setBlue(imageMatrix(i, j));
			}
			// C++ is 0-based
			image.setPixelColor(i-1, j-1, colour);

		}
	}
	return image;
}

/* vim: set ts=4 sw=4 et : */