//////////////////////////////
// Copyright MTHE 493, 2018 //
//////////////////////////////

#ifndef EIGEN_HELPERS_H_
#define EIGEN_HELPERS_H_

#include <vector>
#include <Eigen/Core>
#include <Eigen/Sparse>
#include <Eigen/Dense>

/**
 * Helper function to generate regular matrices from a set of triplets
 */
template<class T>
auto createSparseMatrix(
    int rows, 
    int cols, 
    const Triplets<T>& ijk) -> Eigen::SparseMatrix<T>
{
    Eigen::SparseMatrix<T> a(rows, cols);
    a.setFromTriplets(ijk.begin(), ijk.end());
    return a;
}

/**
 * Helper function to generate square matrices from a set of triplets
 */
template<class T>
auto createSquareMatrix(int size, const Triplets<T>& ijk) -> Eigen::SparseMatrix<T>
{
    return createSparseMatrix(size, size, ijk);
}

/**
 * Helper function for doing a row-wise cumsum along sparse matrices (like MATLAB)
 */
template<class T>
auto cumsum(const Eigen::SparseMatrix<T>& matrix) -> Eigen::Matrix<T, Eigen::Dynamic, 1>
{
    // Cumulatively sum by multiplying by a vector of all ones
    return matrix * VectorX<T>::Ones(matrix.cols());
}

/**
 * Helper function to generate square, sparse identity of a given size
 */
template<class T = int>
auto eye(int size) -> Eigen::SparseMatrix<T>
{
    Triplets<T> ijk;
    ijk.reserve(size);
    for(int i = 0; i < size; ++i)
    {
        ijk.push_back(Triplet<int>(i,i,1));
    }

    return createSquareMatrix(size, ijk);
}
#endif
/* vim: set ts=4 sw=4 et : */
