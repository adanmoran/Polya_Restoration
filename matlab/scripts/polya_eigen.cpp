#include "mex.h"
#include "matrix.h"

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
    // Compute the superurn of each pixel. As a matrix, this is A*V
    // create a vector of 1's that has the length of the number of available types
    Dynamic1D_i ones = Dynamic1D_i::Ones(k); 
    // Vector containing the total number of balls for each superurn row
    Dynamic1D_i totals = S * ones;

    switch(type)
    {
        case SamplingType::RANDOM:
        {
            auto W = S.cwiseQuotient(totals.sparseView());
            break;
        }
        case SamplingType::MEDIAN:
        {

            // Divide the totals by 2 to be able to find the median
            Dynamic1D_d T = totals.cast<double>();
            T *= 0.5;
            // Get the indices where the median ball type exists
            auto median = cumsumFind(S, T);

            //Build the matrix out of these median indices
            int n = median.rows();
            int m = Delta.cols();

            Eigen::SparseMatrix<int> B(V.rows(),V.cols());
            B.reserve(n);

            for(int i = 0; i < n; ++i)
            {
                B.insert(i,median[i]) = 1;
            }

            // Create a matrix of balls to add to the original urn
            // Multiply by Delta to add that many balls to the original urn
            B = B * Delta;
            // Return the original urn plus the new balls
            return V + B;
        }
        default:
        break;
    }
    return V;
}

// *******************************************************
// Include eigen stuff before running mex command assuming root directory is Polya_Restoration
// ipath = ['-I' fullfile(pwd, 'c_plus_plus', 'include')];
// ipath2 = ['-I' fullfile(pwd, 'c_plus_plus', 'external', 'eigen')];
// mex('-v',ipath,ipath2,"matlab/scripts/polya_eigen.cpp",'-outdir',"matlab/scripts/")

void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[])
{
//     double *V, *A, *Delta, *sampling_type;
//     
//     V = mxGetPr(prhs[0]);
//     A = mxGetPr(prhs[1]);
//     Delta = mxGetPr(prhs[2]);
//     sampling_type = mxGetChars(prhs[3]);
    
//     Need to convert input types to eigen matrices before passing into polya function
// 
//     https://stackoverflow.com/questions/43163426/pass-c-eigen-matrix-to-matlab-mex-output
//     // Eigen matrix with some result (non NULL!)
//     UrnMatrix resultEigen = polya(V, A, Delta, sampling_type); 
//     mwSize rows = resultEigen.rows();
//     mwSize cols = resultEigen.cols();
//     plhs[0] = mxCreateDoubleMatrix(rows, cols, mxREAL); // Create MATLAB array of same size
//     Eigen::Map<Eigen::MatrixXd> map(mxGetPr(plhs[0], rows, cols); // Map the array
//     map = resultEigen; // Copy
}

/* vim: set ts=4 sw=4 et :*/
