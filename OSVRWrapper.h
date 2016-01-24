// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the OSVRWRAPPER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// OSVRWRAPPER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef OSVRWRAPPER_EXPORTS
#define OSVRWRAPPER_API extern "C" __declspec(dllexport)
#else
#define OSVRWRAPPER_API __declspec(dllimport)
#endif

OSVRWRAPPER_API int OSVR_Init(const char *contextname);
OSVRWRAPPER_API int OSVR_GetHMDRotation(float *w, float *x, float *y, float *z);
OSVRWRAPPER_API int OSVR_GetHMDPosition(float *x, float *y, float *z);