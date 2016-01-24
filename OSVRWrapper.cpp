#include "stdafx.h"
#include "OSVRWrapper.h"

#include <osvr/ClientKit/Context.h>
#include <osvr/ClientKit/Interface.h>
#include <osvr/ClientKit/InterfaceStateC.h>
#include <memory>

std::shared_ptr<osvr::clientkit::ClientContext> context;
osvr::clientkit::Interface hmd;
OSVR_OrientationReport orientation;
OSVR_PositionReport position;

void OnOrientationChanged(void *userdata, const OSVR_TimeValue *timestamp, const OSVR_OrientationReport *report)
{
	orientation = *report;

}

void OnPositionChanged(void *userdata, const OSVR_TimeValue *timestamp, const OSVR_PositionReport *report)
{
	position = *report;

}

// Initialize OSVR Context
OSVRWRAPPER_API int OSVR_Init(const char *contextname)
{
	context = std::make_shared<osvr::clientkit::ClientContext>(contextname);
	if(!context) return -1;

	// HMD Interface: /me/head
	hmd = context->getInterface("/me/head");

	// Subscribe To HMD Rotation & Position Updates
	hmd.registerCallback(&OnOrientationChanged, nullptr);
	hmd.registerCallback(&OnPositionChanged, nullptr);

    return 0;

}

// Retrieve Current Orientation From HMD
OSVRWRAPPER_API int OSVR_GetHMDRotation(float *w, float *x, float *y, float *z)
{
	if(!context) return -1;

	// Update HMD Tracker State
	context->update();

	*w = static_cast<float>(orientation.rotation.data[0]);
	*x = static_cast<float>(orientation.rotation.data[1]);
	*y = static_cast<float>(orientation.rotation.data[2]);
	*z = static_cast<float>(orientation.rotation.data[3]);

	return 0;

}

// Retrieve Current Position From HMD
OSVRWRAPPER_API int OSVR_GetHMDPosition(float *x, float *y, float *z)
{
	if(!context) return -1;

	// Update HMD Tracker State
	context->update();

	*x = static_cast<float>(position.xyz.data[0]);
	*y = static_cast<float>(position.xyz.data[1]);
	*z = static_cast<float>(position.xyz.data[2]);

	return 0;

}
