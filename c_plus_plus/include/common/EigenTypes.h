//////////////////////////////
// Copyright MTHE 493, 2018 //
//////////////////////////////

#ifndef EIGEN_TYPES_H_
#define EIGEN_TYPES_H_

#include <Eigen/Sparse>

/**
 * This file contains common typedefs
 * for the eigen library, so we don't have
 * to keep using Eigen::X<> for everything.
 */

/**
 * Simple class to enforce square sparse matrix.
 */
template<class T>
class SquareSparseMatrix : public Eigen::SparseMatrix<T>
{
public:
    SquareSparseMatrix(T size)
    : Eigen::SparseMatrix<T>(size,size)
    {}
};

template<class T>
auto createSquareMatrix(int size, const std::vector<Eigen::Triplet<T>>& ijk) -> SquareSparseMatrix<T>
{
    SquareSparseMatrix<T> a(size);
    a.setFromTriplets(ijk.begin(), ijk.end());

    return a;
}

using AdjacencyMatrix = SquareSparseMatrix<int>;
using UrnMatrix = Eigen::SparseMatrix<int>;
using DeltaMatrix = SquareSparseMatrix<int>;

// Dynamic matrix sizes
using Dynamic2D_d = Eigen::MatrixXd;
using Dynamic2D_i = Eigen::MatrixXi;

#endif

/* vim : set ts=4 sw=4 et : */
