//////////////////////////////
// Copyright MTHE 493, 2018 //
//////////////////////////////

#ifndef POLYA_H_
#define POLYA_H_

// May need regular matrices too, which are in <Eigen/Dense>
#include <Eigen/Sparse>

/**
 * Sampling scheme for a polya urn contagion
 */
enum class SamplingType : int
{
    RANDOM,
    MEDIAN
};

/**
 * Documentation here please
 */
auto polya(
    Eigen::SparseMatrix<int> V,
    Eigen::SparseMatrix<int> A,
    Eigen::SparseMatrix<int> Delta,
    SamplingType type) -> Eigen::SparseMatrix<int>;

#endif

/* vim : set ts=4 sw=4 et : */
