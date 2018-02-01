//////////////////////////////
// Copyright MTHE 493, 2018 //
//////////////////////////////

#include "common/EigenTypes.h"
#include "polya/polya.h"

auto polya(
    const UrnMatrix& V, 
    const AdjacencyMatrix& A, 
    const DeltaMatrix& Delta, 
    SamplingType type) -> UrnMatrix
{
    /* Polya code here */
    return UrnMatrix(V.rows(), V.cols());
}

/* vim: set ts=4 sw=4 et :*/
