//
// MATLAB Compiler: 6.5 (R2017b)
// Date: Fri Mar  2 11:13:27 2018
// Arguments:
// "-B""macro_default""-W""cpplib:get_sparse_adj""-T""link:lib""-d""C:\Users\Own
// er\Desktop\Polya_Restoration\c_plus_plus\external\get_sparse_adj\for_testing"
// "-v""C:\Users\Owner\OneDrive\Documents\MTHE\MTHE
// 493\Polya_Restoration\matlab\scripts\get_sparse_adj.m"
//

#ifndef __get_sparse_adj_h
#define __get_sparse_adj_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" {
#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_get_sparse_adj_C_API 
#define LIB_get_sparse_adj_C_API /* No special import/export declaration */
#endif

/* GENERAL LIBRARY FUNCTIONS -- START */

extern LIB_get_sparse_adj_C_API 
bool MW_CALL_CONV get_sparse_adjInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_get_sparse_adj_C_API 
bool MW_CALL_CONV get_sparse_adjInitialize(void);

extern LIB_get_sparse_adj_C_API 
void MW_CALL_CONV get_sparse_adjTerminate(void);

extern LIB_get_sparse_adj_C_API 
void MW_CALL_CONV get_sparse_adjPrintStackTrace(void);

/* GENERAL LIBRARY FUNCTIONS -- END */

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_get_sparse_adj_C_API 
bool MW_CALL_CONV mlxGet_sparse_adj(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

#ifdef __cplusplus
}
#endif


/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__MINGW64__)

#ifdef EXPORTING_get_sparse_adj
#define PUBLIC_get_sparse_adj_CPP_API __declspec(dllexport)
#else
#define PUBLIC_get_sparse_adj_CPP_API __declspec(dllimport)
#endif

#define LIB_get_sparse_adj_CPP_API PUBLIC_get_sparse_adj_CPP_API

#else

#if !defined(LIB_get_sparse_adj_CPP_API)
#if defined(LIB_get_sparse_adj_C_API)
#define LIB_get_sparse_adj_CPP_API LIB_get_sparse_adj_C_API
#else
#define LIB_get_sparse_adj_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_get_sparse_adj_CPP_API void MW_CALL_CONV get_sparse_adj(int nargout, mwArray& adj, const mwArray& rc, const mwArray& radius, const mwArray& p_norm);

/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */
#endif

#endif
