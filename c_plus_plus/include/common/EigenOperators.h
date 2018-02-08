//////////////////////////////
// Copyright MTHE 493, 2018 //
//////////////////////////////

#ifndef EIGEN_OPERATORS_H_
#define EIGEN_OPERATORS_H_

#include <Eigen/Dense>
#include <Eigen/Sparse>

/**
 * Component-wise compare the matrix (lhs) to the vector (rhs)
 */
template<class T>
auto operator > (
    const Eigen::SparseMatrix<T>& matrix,
    const Eigen::VectorXd& vector) -> Eigen::SparseMatrix<T>
{
    // The output matrix of ones and zeros
    Eigen::SparseMatrix<T> GT(matrix.rows(), matrix.cols());
    // The output matrix should have ones only where there are nonzero elements
    GT.reserve(matrix.nonZeros());

    for (int outer = 0; outer < matrix.outerSize(); ++outer)
    {
        for (typename Eigen::SparseMatrix<T>::InnerIterator it(matrix, outer); it; ++it)
        {
            auto row = it.row();
            auto col = it.col();

            if (matrix.coeff(row,col) > vector[row])
            {
                GT.insert(row,col) = 1;
            }
        }
    }

    // Remove all zero-elements that were reserved before
    GT.makeCompressed();
    return GT;
}
#endif
/* vim: set ts=4 sw=4 et : */
