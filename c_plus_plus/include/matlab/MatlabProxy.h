//////////////////////////////////////
// Copyright MTHE 493 Group 8, 2018 //
//////////////////////////////////////

#ifndef MATLAB_PROXY_H_
#define MATLAB_PROXY_H_

#include <array>
#include <string>

//Qt objects
#include <QImage>

#include "common/EigenTypes.h"

// MATLAB Runtime wrappers
#include "get_sparse_adj.h"
#include "polyaTools.h"

struct MatrixSize
{
	size_t rows;
	size_t cols;

	MatrixSize(size_t r, size_t c)
	: rows(r)
	, cols(c)
	{}

	MatrixSize(const std::array<size_t, 2>& size)
		: rows(size[0])
		, cols(size[1])
	{}
};

struct Prefs
{
	enum class ImageType
	{
		RGB,
		GRAY,
		BW
	};

	struct Image
	{
		double binarize_thresh = 0.0;
		ImageType type = ImageType::RGB;
	} image;
};

struct Noise
{
	enum class Type
	{
		NONE,
		GAUSSIAN,
		BINARY_BURST,
		GAUSS_BURST,
		GAUSSIAN_AND_BINARY_BURST,
		GAUSSIAN_AND_GAUSS_BURST,
		GAUSS_MARKOV
	};
	enum class BurstType
	{
		BINARY,
		GAUSSIAN
	};

	Type type = Type::NONE;

	struct BlackAndWhite
	{
		double gaussian_std_dev = 1.0;
		double gaussian_mean = 0.0;
		double gaussian_confidence_interval = 0.8;
	} bw;
	struct Gaussian
	{
		double sigma = 1.0;
		double mean = 0.0;
	} gaussian;
	struct Bursty
	{
		BurstType type = BurstType::BINARY;
		double transition_prob = 0.0;
		double error = 0.0;
		double mean = 0.0;
		double sigma = 100.0;
	} bursty;
	struct GaussMarkov
	{
		double correlation = 0.5;
		double mean = 0.0;
		double sigma = 1.0;
	} gauss_markov;
};

class MatlabProxy
{
public:
	enum class PNorm
	{
		ONE = 1,
		TWO = 2,
		INF
	};

	MatlabProxy();
	~MatlabProxy();

	/**
	 * Allocate memory for the MATLAB Runtime and
	 * open the ability to use any function calleable by this proxy.
	 */
	auto Initialize() -> bool;

	/**
	 * Use the get_sparse_adj method from MATLAB to get an adjacency matrix.
	 * 
	 * Inputs:
	 * rc = row and column sizes
	 * radius = adjacency radius
	 * norm = norm over which the radius is taken
	 */
	auto getSparseAdj(const MatrixSize& rc, size_t radius, PNorm norm) -> AdjacencyMatrix;

	/**
	 * Add noise to the image using the desired type
	 *
	 * Inputs:
	 * image = pointer to the image to which we add noise. This image will be modified directly.
	 * noiseType = the type of noise to add
	 * bw = set to true if the image is logical (i.e. 0 or 1 valued)
	 * 
	 * Returns:
	 * true if successfully modified the image pointer to contain noise.
	 */
	auto addNoise(QImage* image, Prefs prefs, Noise noise) -> bool;

	/**
	 * Terminate the MATLAB Runtime and close all functions calleable by this proxy.
	 */
	auto terminate() -> void;

private:
	// Convert an image to an mwArray
	auto toMATLABArray(const QImage& image, Prefs::ImageType type)->mwArray;

	// Convert an mwArray to an image
	auto toQImage(const mwArray& imageMatrix)->QImage;

	// Is our connection to the matlab runtime open?
	bool mclInitialized_;
	// Is our connection to the individual functions open?
	bool open_;

	const std::string INF_STR = "inf";
	
	enum class ColorDepth
	{
		RED = 0,
		GREEN = 1,
		BLUE = 2
	};
};

#endif

/* vim: set ts=4 sw=4 et : */