//
// MATLAB Compiler: 6.5 (R2017b)
// Date: Sun Mar  4 11:11:25 2018
// Arguments:
// "-B""macro_default""-W""cpplib:polyaTools""-T""link:lib""-d""C:\Users\Owner\D
// esktop\Polya_Restoration\c_plus_plus\external\polyaTools\for_testing""-v""C:\
// Users\Owner\OneDrive\Documents\MTHE\MTHE
// 493\Polya_Restoration\matlab\scripts\add_noise.m""C:\Users\Owner\OneDrive\Doc
// uments\MTHE\MTHE
// 493\Polya_Restoration\matlab\scripts\adjacency_minus_edge_d.m""C:\Users\Owner
// \OneDrive\Documents\MTHE\MTHE
// 493\Polya_Restoration\matlab\scripts\image_from_urns.m""C:\Users\Owner\OneDri
// ve\Documents\MTHE\MTHE
// 493\Polya_Restoration\matlab\scripts\initialize_polya_urns.m""C:\Users\Owner\
// OneDrive\Documents\MTHE\MTHE
// 493\Polya_Restoration\matlab\scripts\inverse_quantize_image.m""C:\Users\Owner
// \OneDrive\Documents\MTHE\MTHE
// 493\Polya_Restoration\matlab\scripts\quantize_image.m"
//

#ifndef __polyaTools_h
#define __polyaTools_h 1

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
#ifndef LIB_polyaTools_C_API 
#define LIB_polyaTools_C_API /* No special import/export declaration */
#endif

/* GENERAL LIBRARY FUNCTIONS -- START */

extern LIB_polyaTools_C_API 
bool MW_CALL_CONV polyaToolsInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_polyaTools_C_API 
bool MW_CALL_CONV polyaToolsInitialize(void);

extern LIB_polyaTools_C_API 
void MW_CALL_CONV polyaToolsTerminate(void);

extern LIB_polyaTools_C_API 
void MW_CALL_CONV polyaToolsPrintStackTrace(void);

/* GENERAL LIBRARY FUNCTIONS -- END */

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_polyaTools_C_API 
bool MW_CALL_CONV mlxAdd_noise(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_polyaTools_C_API 
bool MW_CALL_CONV mlxAdjacency_minus_edge_d(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                            *prhs[]);

extern LIB_polyaTools_C_API 
bool MW_CALL_CONV mlxImage_from_urns(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                     *prhs[]);

extern LIB_polyaTools_C_API 
bool MW_CALL_CONV mlxInitialize_polya_urns(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                           *prhs[]);

extern LIB_polyaTools_C_API 
bool MW_CALL_CONV mlxInverse_quantize_image(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                            *prhs[]);

extern LIB_polyaTools_C_API 
bool MW_CALL_CONV mlxQuantize_image(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

#ifdef __cplusplus
}
#endif


/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__MINGW64__)

#ifdef EXPORTING_polyaTools
#define PUBLIC_polyaTools_CPP_API __declspec(dllexport)
#else
#define PUBLIC_polyaTools_CPP_API __declspec(dllimport)
#endif

#define LIB_polyaTools_CPP_API PUBLIC_polyaTools_CPP_API

#else

#if !defined(LIB_polyaTools_CPP_API)
#if defined(LIB_polyaTools_C_API)
#define LIB_polyaTools_CPP_API LIB_polyaTools_C_API
#else
#define LIB_polyaTools_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_polyaTools_CPP_API void MW_CALL_CONV add_noise(int nargout, mwArray& noisy_image, const mwArray& image, const mwArray& prefs, const mwArray& noise);

extern LIB_polyaTools_CPP_API void MW_CALL_CONV adjacency_minus_edge_d(int nargout, mwArray& adj, const mwArray& adjacency, const mwArray& edges, const mwArray& d);

extern LIB_polyaTools_CPP_API void MW_CALL_CONV image_from_urns(int nargout, mwArray& image, const mwArray& image_size, const mwArray& urns);

extern LIB_polyaTools_CPP_API void MW_CALL_CONV initialize_polya_urns(int nargout, mwArray& urns, const mwArray& image, const mwArray& initial_balls, const mwArray& num_ball_types);

extern LIB_polyaTools_CPP_API void MW_CALL_CONV inverse_quantize_image(int nargout, mwArray& image, const mwArray& q_image, const mwArray& inverse_quantization, const mwArray& partition, const mwArray& codebook);

extern LIB_polyaTools_CPP_API void MW_CALL_CONV quantize_image(int nargout, mwArray& q_image, mwArray& partition, mwArray& codebook, const mwArray& image, const mwArray& num_ball_types, const mwArray& q_type);

/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */
#endif

#endif
