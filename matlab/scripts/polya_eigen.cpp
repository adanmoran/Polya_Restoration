//////////////////////////////
// Copyright MTHE 493, 2018 //
//////////////////////////////
#include "mex.h"
#include "matrix.h"

#include <iostream>
#include <chrono>
#include "common/EigenTypes.h"
#include "common/EigenHelpers.h"
#include "polya/polya.h"

/*
Helper function to convert a mxArray of a sparse matrix to a C++ Triplets<int>
object.
*/
auto sparse_mxarray_to_triplets(const mxArray *in) -> Triplets<int>
{
    double *sparse_array;
    mwSize nzmax, rows, columns;
    mwIndex nnz;
    mwIndex *irs, *jcs;
    int nrow;
    Triplets<int> ints;

    sparse_array = mxGetPr(in); // pointer to values in sparse array
    columns = mxGetN(in); // # columns
    rows = mxGetM(in); // # rows
    nzmax = mxGetNzmax(in); // max # of non-zero elements
    nnz = *(mxGetJc(in) + columns); // # non-zero elements in sparse array

    // Each element in the ir array indicates a row at which a
    // nonzero element can be found.
    irs = mxGetIr(in);

    // the jth column of the sparse mxArray, jc[j] is the total number of
    // nonzero elements in all preceding columns. The last element of the jc
    // array, jc[number of columns], is equal to nnz, which is the number of
    // nonzero elements in the entire sparse mxArray.
    jcs = mxGetJc(in);

    ints.reserve(nnz);
    for(int y = 0; y < columns; y++)
    {
        nrow = jcs[y + 1] - jcs[y]; // number of elements in current column
        for(int x = 0; x < nrow; x++)
        {
//             mexPrintf("   (%d,%d)    %g\n",*irs,y,*sparse_array);
            ints.push_back(Triplet<int>(*irs++, y, *sparse_array++));
        }
    }

    return ints;
}


void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[])
{
    if (!(nrhs == 5)) {
        mexErrMsgIdAndTxt("polya_eigen:NotEnoughInputs",
                "Not enough input parameters.");
    }
    if (!mxIsSparse(prhs[0]))  {
        mexErrMsgIdAndTxt("polya_eigen:NonSparseInput1",
                "Input argument 1 must be a sparse array.");
    }
    if (!mxIsSparse(prhs[1]))  {
        mexErrMsgIdAndTxt("polya_eigen:NonSparseInput2",
                "Input argument 2 must be a sparse array.");
    }
    if (!mxIsSparse(prhs[2]))  {
        mexErrMsgIdAndTxt("polya_eigen:NonSparseInput3",
                "Input argument 3 must be a sparse array.");
    }
    if (mxIsChar(prhs[3]) != 1)  {
        mexErrMsgIdAndTxt("polya_eigen:NonCharInput4",
                "Input argument 4 must be a char array. Use '' not "".");
    }
    if (!mxIsScalar(prhs[4]))  {
        mexErrMsgIdAndTxt("polya_eigen:NonScalarInput4",
                "Input argument 3 must be a scalar");
    }

    using namespace std::chrono;
    using tp = high_resolution_clock::time_point;
    using millis = duration<double, std::milli>;

    const mxArray *V; // V = urn matrix
    const mxArray *A; // A = adjacency matrix
    const mxArray *D; // D = delta matrix
    mwSize V_cols, V_rows;
    mwSize A_cols, A_rows;
    mwSize D_cols, D_rows;
    Triplets<int> ints;

    V = prhs[0];
    V_cols = mxGetN(V);
    V_rows = mxGetM(V);
    ints = sparse_mxarray_to_triplets(V);
    UrnMatrix urn = createSparseMatrix(V_rows, V_cols, ints);

    A = prhs[1];
    A_cols = mxGetN(A);
    A_rows = mxGetM(A);
    ints = sparse_mxarray_to_triplets(A);
    AdjacencyMatrix adj = createSparseMatrix(A_rows, A_cols, ints);

    D = prhs[2];
    D_cols = mxGetN(D);
    D_rows = mxGetM(D);
    ints = sparse_mxarray_to_triplets(D);
    DeltaMatrix del = createSparseMatrix(D_rows, D_cols, ints);

    char *chars_in = mxArrayToString(prhs[3]);

    SamplingType stype;
    if(strcmp(chars_in, "random") == 0)
    {
        stype = SamplingType::RANDOM;
    }
    else
    {
        stype = SamplingType::MEDIAN;
    }

    int iterations = mxGetScalar(prhs[4]);
    double average = 0.0;

    for (int i = 0 ; i < iterations; ++i)
    {
        tp t2 = high_resolution_clock::now();
        urn = polya(urn, adj, del, stype);
        tp t1 = high_resolution_clock::now();
        duration<double, std::milli> time_span = t1 - t2;
        average += time_span.count();
    }
//     mexPrintf("Average iteration time:%f\n", average / iterations);

    // Copy eigen matrix results out to matlab
    mwSize rows = urn.rows();
    mwSize cols = urn.cols();
    mwSize nnz = urn.nonZeros();
//     mexPrintf("cols: %d, rows: %d, nnz: %d\n", cols, rows, nnz);

    mwIndex *irs,*jcs;
    double *sr;

    plhs[0] = mxCreateSparse(rows, cols, nnz, mxREAL);
    sr  = mxGetPr(plhs[0]);
    irs = mxGetIr(plhs[0]);
    jcs = mxGetJc(plhs[0]);

    mwIndex k = 0;
    for (int j=0; j < urn.outerSize(); ++j)
    {
        jcs[j] = k;
        for (UrnMatrix::InnerIterator it(urn,j); it; ++it)
        {
            double val = static_cast<double>(it.value());
            mwIndex row = it.row();
            sr[k] = val;
            irs[k] = row;
            k++;
        }
    }
    jcs[urn.outerSize()] = k;
}
