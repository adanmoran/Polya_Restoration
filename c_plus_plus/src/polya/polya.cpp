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
using Dynamic1D = Eigen::VectorXi;
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
    Dynamic1D ones = Dynamic1D::Ones(k); 
    // Vector containing the total number of balls for each superurn row
    Dynamic1D T = S * ones;

    auto W = S.cwiseQuotient(T.sparseView());

    switch(type)
    {
        case SamplingType::RANDOM:

        break;
        case SamplingType::MEDIAN:
            auto C = cumsum(S);
            Dynamic1D median(c.rows());
            //TODO: build the median vector

                T = T.cwiseQuotient(2)
                for(int o = 0 ; o < matrix.outerSize(); o++)
    {
        for (typename Eigen::SparseMatrix<T>::InnerIterator it(matrix,o); it; ++it)
        {
        }
    }
            //Build the matrix out of these cumulative sum indices
            int n = median.rows();
            int m = Delta.cols();

            Triplets BValues;
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
