//////////////////////////////////////
// Copyright MTHE 493 Group 8, 2018 //
//////////////////////////////////////

#ifndef MATLAB_PROXY_H_
#define MATLAB_PROXY_H_

#include <array>

#include "get_sparse_adj.h"
#include "common/EigenTypes.h"

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
	 * Terminate the MATLAB Runtime and close all functions calleable by this proxy.
	 */
	auto terminate() -> void;

private:
	// Is our connection to the matlab runtime open?
	bool mclInitialized_;
	// Is our connection to the individual functions open?
	bool open_;
};

#endif

/* vim: set ts=4 sw=4 et : */