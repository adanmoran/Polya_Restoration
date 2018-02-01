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

using AdjacencyMatrix = SquareSparseMatrix<int>;
using UrnMatrix = Eigen::SparseMatrix<int>;
using DeltaMatrix = SquareSparseMatrix<int>;

#endif

/* vim : set ts=4 sw=4 et : */
