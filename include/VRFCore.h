#pragma once

#include <vrfcgf/cgf.h>

#include <fmt/core.h>

using namespace fmt;

/// <summary>
/// Essential VR-Force classes
/// </summary>
namespace VRFCore
{
	/// <summary>
	/// You finally found me
	/// </summary>
	extern DtCgf* cgf;

	inline DtCgf* VRFCgf()
	{
		if (cgf == nullptr)
		{
			print("[ERROR] Attempt to use DtCgf when it is uninitialised\n");
		}

		return cgf;
	}

	/// <summary>
	/// VR-Force's invisible hands to help me find stuff
	/// It's just like a Yellow Book, you can find everyone here
	/// </summary>
	/// <returns>DtVrfObjectManager</returns>
	inline DtVrfObjectManager& VRFObjectManager()
	{
		return *(VRFCgf()->vrfObjectManager());
	}

	/// <summary>
	/// VR-Force's physical world in a virtual world in our physical world
	/// Are we in the Matrix?
	/// </summary>
	/// <returns>DtPhysicalWorld</returns>
	inline DtPhysicalWorld& VRFPhysicalWorld()
	{
		return *(VRFCgf()->physicalWorld());
	}

	/// <summary>
	/// VR-Force's manager of the simulation
	/// I not too sure how much I need to use it
	/// </summary>
	/// <returns>DtSimManager</returns>
	inline DtSimManager& VRFSimManager()
	{
		return *(VRFCgf()->simManager());
	}

	/// <summary>
	/// VR-Force's way of creating/managing a kaboomz!
	/// </summary>
	/// <returns>DtPhysicalWorld</returns>
	inline DtDetonationManager& VRFDetonationManager()
	{
		return *(VRFCgf()->detonationManager());
	}

	/// <summary>
	/// VR-Force's way of creating/managing a FireInteraction
	/// </summary>
	/// <returns>DtPhysicalWorld</returns>
	inline DtFireManager& VRFFireManager()
	{
		return *(VRFCgf()->fireManager());
	}
}