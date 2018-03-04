//////////////////////////////
// Copyright MTHE 493, 2018 //
//////////////////////////////

#ifndef EIGEN_HELPERS_H_
#define EIGEN_HELPERS_H_

#include <stdexcept>
#include <string>
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
    a.reserve(ijk.size());

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
    const VectorX<V> rhs) -> Eigen::VectorXi
{
    Eigen::VectorXi indices(rhs.rows());

    // If sizes don't match, we return all -1's
    if (lhs.rows() != rhs.rows())
    {
        for(auto i = 0; i < rhs.rows(); ++i)
        {
            indices[i] = -1;
        }
        return indices;
    }

    // Transpose to do row operations as opposed to column operations
    Eigen::SparseMatrix<U> transpose = lhs.transpose();

    using II = typename Eigen::SparseMatrix<U>::InnerIterator; 

    // Iterate and cumsum while finding the first element
    // above rhs(row)
    // Column major of transpose is row-major of regular
    for(int row = 0; row < transpose.outerSize(); ++row)
    {
        U summed = static_cast<U>(0);
        bool found = false;

        // Iterate over the rows of the original sparse matrix
        II it(transpose,row); 
        if (!it)
        {
            std::string msg = "cumsumFind: sparse row ";
            msg += std::to_string(row);
            msg += " was all zero. Please ensure every row has at least one element.";
            throw std::runtime_error(msg);
        }
        for (II it(transpose, row); it && !found; ++it)
        {
            auto col = it.index();

            // do a cumulative sum
            summed += lhs.coeff(row, col);

            // find which index provides the median value
            if (summed > rhs[row])
            {
                indices[row] = col;
                found = true;
            }
        }
        if (!found)
        {
            std::string msg = "cumsumFind: vector element ";
            msg += std::to_string(indices[row]);
            msg += " was too large for sparse matrix row ";
            msg += std::to_string(row);
            msg += ". Please ensure all values of the comparison vector are smaller than the maximum element of the sparse matrix.";
            throw std::runtime_error(msg);
        }
    }
    
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

/**
 * Convert a sparse matrix to a full matrix
 */
template<class T>
auto full(const Eigen::SparseMatrix<T>& matrix) -> Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>
{
	return Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>(matrix);
}
#endif
/* vim: set ts=4 sw=4 et : */
