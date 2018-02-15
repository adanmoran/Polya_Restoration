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
 * Helper function for doing a row-wise cumsum along sparse matrices (like MATLAB) for column-major matrices
 */
template<class T>
auto cumsum(const Eigen::SparseMatrix<T>& matrix) -> Eigen::SparseMatrix<T>
{
    Eigen::SparseMatrix<T> Y(matrix.rows(), matrix.cols());
    Y.reserve(matrix.nonZeros());

    Triplets<T> values;
    values.reserve(matrix.nonZeros());

    for(int o = 0 ; o < matrix.outerSize(); o++)
    {
        for (typename Eigen::SparseMatrix<T>::InnerIterator it(matrix,o); it; ++it)
        {
            auto i = it.index();

            if (o == 0)
            {

                Y.insert(i,o) = it.value();
            }
            else
            {

                Y.insert(i,o) = Y.coeff(i,o-1) + it.value();
            }
        }
    }
    return Y;
}

/**
 * Helper function for getting the index over the row-wise cumsum efficiently.
 * e.g.
 *             1 2 3   3
 * cumsumFind( 4 5 6 , 7.5 )
 *             7 8 9   12
 *
 * <=>
 *                  1 2 3     3
 *     max( cumsum( 4 5 6 ) > 7.5 )
 *                  7 8 9     12
 * <=>
 *          1 3  6    3
 *     max( 4 9  15 > 7.5 )
 *          7 15 24   12
 * <=>
 *          0 0 1
 *     max( 0 1 1 )
 *          0 1 1
 * <=>
 *     3
 *     2
 *     2
 */
template<class U, class V>
auto cumsumFind(
    const Eigen::SparseMatrix<U>& lhs,
    const VectorX<V> rhs) -> VectorX<V>
{
    VectorX<V> indices(rhs.size());

    // TODO:
    // Iterate row-wise and find the element that is the maximum
    return indices;
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
