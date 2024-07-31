#pragma once

#include <vrfGuiCore/vrfGuiExportPlugin.h>

#define NO_NAME_MANGLING extern "C"

// This damn thing is called multiple times when VRF starts up
NO_NAME_MANGLING bool initDeModule(makVrv::DtDe* de);
// wait till you hear of this cool trick to reduce your errors by 100%
namespace BasicVRFFEPlugin {
	void init(makVrv::DtDe& de);
}

/// <summary>
/// Include this code in your plugin if you wish to initialize information after all the plugins have
/// been loaded and the de initialize, but before the de beings to tick.  This function is useful
/// for getting instances of classes that have been created, or can only be created after de initialization
/// </summary>
/// <param name="de">Holy grail of La la land</param>
/// <returns>I assume it's true for success, false if not</returns>
NO_NAME_MANGLING DT_VRF_DLL_PLUGIN void postInitDeModule(makVrv::DtDe& de);

/// <summary>
/// Used to return information about the plugin being loaded
/// </summary>
/// <param name="info">Information about this plugin</param>
NO_NAME_MANGLING DT_VRF_DLL_PLUGIN void DtPluginInformation(DtVrfPluginInformation& info);
