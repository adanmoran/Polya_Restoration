/*
*
* Borrowed and adapted from Aaron Johnsy, February 2017
* Source: https://angeljohnsy.blogspot.com/2014/08/lee-filter.html
*
*
*/

#include "mex.h"
#include "matrix.h"

/* Find variance and mean of the pixels in the window */
void arrayProduct(double v, double In, double *y, double *z, mwSize n)
{
    double VarW,MeanW,W;
    mwSize i;

    MeanW=0;
    for (i=0; i < sizeof(y); i++)
    {
        MeanW=MeanW+y[i];
    }
    MeanW=MeanW/n;
    VarW=0;
    for(i=0; i < sizeof(y); i++)
    {
        VarW=VarW+((y[i]-MeanW)*(y[i]-MeanW));
    }
    VarW=VarW/n;

    W=VarW/(VarW+v);
    z[0]=MeanW+W*(In-MeanW);
}

void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[])
{
    double Rvariance,CIndx,*inMatrix,*out;
    size_t ncols;


    Rvariance = mxGetScalar(prhs[0]);
    CIndx  = mxGetScalar(prhs[1]);
    inMatrix = mxGetPr(prhs[2]);
    ncols = mxGetN(prhs[2]);

    plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
    out = mxGetPr(plhs[0]);

    /* Call the function arrayProduct */
    arrayProduct(Rvariance,CIndx,inMatrix,out,(mwSize)ncols);
}
