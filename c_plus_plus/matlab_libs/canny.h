/*
 * MATLAB Compiler: 6.5 (R2017b)
 * Date: Sun Mar  4 11:14:03 2018
 * Arguments:
 * "-B""macro_default""-W""lib:canny""-T""link:lib""-d""C:\Users\Owner\Desktop\P
 * olya_Restoration\c_plus_plus\external\canny\for_testing""-v""C:\Users\Owner\O
 * neDrive\Documents\MTHE\MTHE 493\Polya_Restoration\matlab\scripts\canny.m"
 */

#ifndef __canny_h
#define __canny_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#ifdef __cplusplus
extern "C" {
#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_canny_C_API 
#define LIB_canny_C_API /* No special import/export declaration */
#endif

/* GENERAL LIBRARY FUNCTIONS -- START */

extern LIB_canny_C_API 
bool MW_CALL_CONV cannyInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_canny_C_API 
bool MW_CALL_CONV cannyInitialize(void);

extern LIB_canny_C_API 
void MW_CALL_CONV cannyTerminate(void);

extern LIB_canny_C_API 
void MW_CALL_CONV cannyPrintStackTrace(void);

/* GENERAL LIBRARY FUNCTIONS -- END */

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_canny_C_API 
bool MW_CALL_CONV mlxCanny(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

/* C INTERFACE -- MLF WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_canny_C_API bool MW_CALL_CONV mlfCanny(int nargout, mxArray** edges, mxArray* noisy_image, mxArray* threshold, mxArray* sigma);

#ifdef __cplusplus
}
#endif
/* C INTERFACE -- MLF WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

#endif
