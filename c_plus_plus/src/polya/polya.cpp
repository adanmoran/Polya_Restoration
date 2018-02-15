//////////////////////////////
// Copyright MTHE 493, 2018 //
//////////////////////////////

#include "common/EigenTypes.h"
#include "polya/polya.h"
#include <iostream>
#include "common/EigenHelpers.h"


/* Polya Contagion Model
% V = Urn Matrix; A nxk matrix where n is the number of vertices, and k is
% the number of different types of balls
%
% A = Adjacency Matrix; a binary nxn matrix where a 1 represents 
% a connection between the vertices (i,j). Note that A must contain
# 1's along the diagonal so that the superurn can be correctly retrieved.
%
% Delta = a kxk matrix where the (i,j)th entry denotes the number of balls 
% of type j that will be added when a ball of type i is drawn.
%
% SampleType (optional) = a string telling the polya model how to sample
% from the urns. Options are 'random' (for random urn draws) or 'median'
% (for drawing the median urn). Default is 'random'.
*/
auto polya(
    const UrnMatrix& V, 
    const AdjacencyMatrix& A, 
    const DeltaMatrix& Delta, 
    SamplingType type) -> UrnMatrix
{
    // Number of types (of balls) we have
    int k = Delta.rows();
    // Compute the superurn of each pixel. As a matrix, this is A*V
    SuperUrnMatrix S = A * V;
    // create a vector of 1's that has the length of the number of available types
    Dynamic1D_i ones = Dynamic1D_i::Ones(k); 
    // Vector containing the total number of balls for each superurn row
    Dynamic1D_i T = S * ones;

    auto W = S.cwiseQuotient(T.sparseView());

    switch(type)
    {
        case SamplingType::RANDOM:

            break;
        case SamplingType::MEDIAN:

            // Divide the totals by 2 to be able to find the median
            T *= 0.5;
            auto median = cumsumFind(S, T);

            //Build the matrix out of these cumulative sum indices
            int n = median.rows();
            int m = Delta.cols();

            Triplets<int> BValues;
            BValues.reserve(n);

            for(int i = 0; i < n; ++i)
            {
                BValues.push_back(Triplet<int>(i,median[i],1));
            }

            auto B = createSparseMatrix(n, m, BValues);

        break;
    }
    return UrnMatrix(V.rows(), V.cols());
}

/* vim: set ts=4 sw=4 et :*/
