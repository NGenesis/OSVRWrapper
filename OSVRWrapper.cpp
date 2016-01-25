#include "stdafx.h"
#include "OSVRWrapper.h"

#include <osvr/ClientKit/Context.h>
#include <osvr/ClientKit/Interface.h>
#include <osvr/ClientKit/InterfaceStateC.h>
#include <osvr/ClientKit/TransformsC.h>
#include <memory>

// OSVR State
std::shared_ptr<osvr::clientkit::ClientContext> Context;
osvr::clientkit::Interface HMD;
OSVR_Quaternion CurrentRotation;
OSVR_Vec3 CurrentPosition;
bool IsHMDInit = false;

// OSVR Helper Functions
// Extract Pitch Angle From Quaternion
double osvrQuatGetPitch(const OSVR_Quaternion *q)
{
	return asin(2.0f * (osvrQuatGetX(q) * osvrQuatGetY(q) - osvrQuatGetW(q) * osvrQuatGetZ(q)));

}

// Extract Yaw Angle From Quaternion
double osvrQuatGetYaw(const OSVR_Quaternion *q)
{
	return atan2(2.0f * osvrQuatGetX(q) * osvrQuatGetZ(q) + 2.0f * osvrQuatGetY(q) * osvrQuatGetW(q), 1.0f - 2.0f * (osvrQuatGetZ(q) * osvrQuatGetZ(q) + osvrQuatGetY(q) * osvrQuatGetY(q)));

}

// Extract Roll Angle From Quaternion
double osvrQuatGetRoll(const OSVR_Quaternion *q)
{
	return atan2(2.0f * osvrQuatGetX(q) * osvrQuatGetW(q) + 2.0f * osvrQuatGetZ(q) * osvrQuatGetY(q), 1.0f - 2.0f * (osvrQuatGetY(q) * osvrQuatGetY(q) + osvrQuatGetW(q) * osvrQuatGetW(q)));

}

// Event Callbacks
void OnOrientationChanged(void *userdata, const OSVR_TimeValue *timestamp, const OSVR_OrientationReport *report)
{
	CurrentRotation = report->rotation;

}

void OnPositionChanged(void *userdata, const OSVR_TimeValue *timestamp, const OSVR_PositionReport *report)
{
	CurrentPosition = report->xyz;

}

// Initialize HMD
void OnInitHMD()
{
	if(HMD.notEmpty()) HMD.free();

	// HMD Interface: /me/head
	HMD = Context->getInterface("/me/head");

	// Subscribe To HMD Rotation & Position Updates
	HMD.registerCallback(&OnOrientationChanged, nullptr);
	HMD.registerCallback(&OnPositionChanged, nullptr);

	IsHMDInit = true;

	// Initialize Orientation
	osvrClientSetRoomRotationUsingHead(Context->get());

}

// Initialize Context
OSVRWRAPPER_API bool OSVR_Init(const char *contextname)
{
	Context = std::make_shared<osvr::clientkit::ClientContext>(contextname);
	if(!Context) return false;

	IsHMDInit = false;
	osvrQuatSetIdentity(&CurrentRotation);
	osvrVec3Zero(&CurrentPosition);

	// Initialize HMD When Context Is Ready
	if(Context->checkStatus()) OnInitHMD();

    return true;

}

// Update Context
OSVRWRAPPER_API void OSVR_Update()
{
	Context->update();

	// Initialize HMD When Context Is Ready
	if(!IsHMDInit && Context->checkStatus()) OnInitHMD();

}

// Simple HMD Detection
OSVRWRAPPER_API bool OSVR_IsHMDDetected()
{
	// Check Validity Of Context & Interface
	if(!IsHMDInit || !Context->checkStatus() || !HMD.notEmpty()) return false;

	// Attempt To Retrieve HMD Rotation
	OSVR_TimeValue timestamp;
	OSVR_OrientationState state;
	auto ret = osvrGetOrientationState(HMD.get(), &timestamp, &state);

	return (ret == OSVR_RETURN_SUCCESS);

}

// Retrieve Current Orientation From HMD
OSVRWRAPPER_API void OSVR_GetHMDRotation(double *w, double *x, double *y, double *z)
{
	*w = osvrQuatGetW(&CurrentRotation);
	*x = osvrQuatGetX(&CurrentRotation);
	*y = osvrQuatGetY(&CurrentRotation);
	*z = osvrQuatGetZ(&CurrentRotation);

}

// Retrieve Current Orientation From HMD As Euler Angle Components
OSVRWRAPPER_API void OSVR_GetHMDRotationPitchYawRoll(double *pitch, double *yaw, double *roll)
{
	*pitch = osvrQuatGetPitch(&CurrentRotation);
	*yaw = osvrQuatGetYaw(&CurrentRotation);
	*roll = osvrQuatGetRoll(&CurrentRotation);

}

// Retrieve Current Position From HMD
OSVRWRAPPER_API void OSVR_GetHMDPosition(double *x, double *y, double *z)
{
	*x = osvrVec3GetX(&CurrentPosition);
	*y = osvrVec3GetY(&CurrentPosition);
	*z = osvrVec3GetZ(&CurrentPosition);

}

// Recenter HMD From Current Yaw Rotation
OSVRWRAPPER_API void OSVR_ResetHMDRotationFromHead()
{
	osvrClientSetRoomRotationUsingHead(Context->get());

}