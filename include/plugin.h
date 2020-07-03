#pragma once

#include "Config.h"
#include "MyStartingPoint.h"
#include "VRFCore.h"

#include <memory>
#include <vrfcgf/vrfPluginExtension.h>				//mandatory VRF plugin callbacks

using namespace BasicVRFBEPlugin;

#ifdef EXPORT_FUNCTIONS
#define DLL_EXPORT __declspec(dllexport)
#else	
#define DLL_IMPORT __declspec(dllimport)
#endif

#define NO_NAME_MANGLING extern "C"

/// <summary>
/// Implement this plugin routine in order to place your object subclasses into the factory.This method
/// should be used for assigning controller, main objects(DtVrfObjectManager subclasses, etc.), actuators
/// sensors  This function should be implemented if you wish to override or replace existing functionality
/// that is defined by default within the system.
/// </summary>
/// <param name="cgf">Holy grail of La la land</param>
/// <returns>I assume it's true for success, false if not</returns>
NO_NAME_MANGLING DT_VRF_DLL_PLUGIN bool DtInitializeVrfPlugin(DtCgf* cgf);

/// <summary>
/// Implement this call if you want to get access to those objects created by the DtCgf.  This
///	is called as the last step of the DtCgf init() method  This function should be implemented if
///	you wish to work with objects that have been created from the factory as part of the DtCgf
///	init() method call(for example, DtVrfObjectManager)
/// </summary>
/// <param name="cgf">Holy grail of La la land</param>
/// <returns>I assume it's true for success, false if not</returns>
NO_NAME_MANGLING DT_VRF_DLL_PLUGIN bool DtPostInitializeVrfPlugin(DtCgf* cgf);

/// <summary>
/// Implement this method to be notified when the plugin is about to be unloaded so you can clean up
/// statically allocated memory
/// </summary>
NO_NAME_MANGLING DT_VRF_DLL_PLUGIN void DtUnloadVrfPlugin();

/// <summary>
/// Information about this plugin that is passed to VR-Force
/// </summary>
/// <param name="info">Information about this plugin</param>
NO_NAME_MANGLING DT_VRF_DLL_PLUGIN void DtPluginInformation(DtVrfPluginInformation& info);

std::unique_ptr<MyStartingPoint> myStartingPoint = nullptr;