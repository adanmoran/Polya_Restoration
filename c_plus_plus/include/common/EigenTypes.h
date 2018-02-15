//////////////////////////////
// Copyright MTHE 493, 2018 //
//////////////////////////////

#ifndef EIGEN_TYPES_H_
#define EIGEN_TYPES_H_

#include <Eigen/Dense>
#include <Eigen/Sparse>

/**
 * Wrapper class that enforces creation of a square matrix
 */
template<class T>
class SquareSparseMatrix : public Eigen::SparseMatrix<T>
{
public:
    SquareSparseMatrix(int size)
    : Eigen::SparseMatrix<T>(size,size)
    {}

};

/**
 * Wrapper around Eigen Triplet
 */
using Eigen::Triplet;

/**
 * Wrapper around a collection of triplets, for ease of
 * implementation of sparse matrices
 */
template<class NUMERIC>
using Triplets = std::vector<Triplet<NUMERIC>>;

/**
 * Wrapper around dynamic matrix for arbitrary vector lengths
 */
template<class NUMERIC>
using VectorX = Eigen::Matrix<NUMERIC, Eigen::Dynamic, 1>;

using AdjacencyMatrix = SquareSparseMatrix<int>;
using UrnMatrix = Eigen::SparseMatrix<int>;
using DeltaMatrix = SquareSparseMatrix<int>;
using SuperUrnMatrix = Eigen::SparseMatrix<int>;

// Dynamic Vector sizes
using Dynamic1D_d = Eigen::VectorXd;
using Dynamic1D_i = Eigen::VectorXi;

// Dynamic matrix sizes
using Dynamic2D_d = Eigen::MatrixXd;
using Dynamic2D_i = Eigen::MatrixXi;


#endif

/* vim: set ts=4 sw=4 et : */
