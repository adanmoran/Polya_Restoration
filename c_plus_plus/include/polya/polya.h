//////////////////////////////
// Copyright MTHE 493, 2018 //
//////////////////////////////

#ifndef POLYA_H_
#define POLYA_H_

#include "common/Types.h"
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
    const UrnMatrix& V,
    const AdjacencyMatrix& A,
    const DeltaMatrix& Delta,
    SamplingType type) -> UrnMatrix;

#endif

/* vim : set ts=4 sw=4 et : */
