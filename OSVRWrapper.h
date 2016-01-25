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

OSVRWRAPPER_API bool OSVR_Init(const char *contextname);
OSVRWRAPPER_API void OSVR_Update();
OSVRWRAPPER_API bool OSVR_IsHMDDetected();
OSVRWRAPPER_API void OSVR_GetHMDRotation(double *w, double *x, double *y, double *z);
OSVRWRAPPER_API void OSVR_GetHMDRotationPitchYawRoll(double *pitch, double *yaw, double *roll);
OSVRWRAPPER_API void OSVR_GetHMDPosition(double *x, double *y, double *z);
OSVRWRAPPER_API void OSVR_ResetHMDRotationFromHead();