#include "MyStartingPoint.h"

#include <boost/filesystem.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace BasicVRFBEPlugin
{
    /// <summary>
    /// Default Constructor
    /// </summary>
    /// <param name="cgf">The all important access into the VRF world</param>
    MyStartingPoint::MyStartingPoint(DtCgf* cgf, unique_ptr<Config> config)
	{
		this->cgf = cgf;
        this->config = move(config);

        AddCallbacks();
        print("[{}]\n", __FUNCTION__);
	}

    /// <summary>
    /// Default destructor
    /// </summary>
    MyStartingPoint::~MyStartingPoint()
	{
        RemoveCallbacks();

        cgf = nullptr;
        config.reset();

        print("[{}]\n", __FUNCTION__);
	}

    /// <summary>
    /// Subscribe to VR-Force callbacks
    /// </summary>
    void MyStartingPoint::AddCallbacks()
    {
        cgf->addPostLoadScenarioCallback(&MyStartingPoint::PostLoadScenarioCallback, this);
        cgf->addPreCloseScenarioCallback(&MyStartingPoint::PreCloseScenarioCallback, this);
        cgf->addPostTickCallback(&MyStartingPoint::PostTickCallback, this);
    }

    /// <summary>
    /// Unsubscribe to VR-Force callbacks
    /// </summary>
    void MyStartingPoint::RemoveCallbacks()
    {
        cgf->removePostTickCallback(&MyStartingPoint::PostTickCallback, this);
        cgf->removePreCloseScenarioCallback(&MyStartingPoint::PreCloseScenarioCallback, this);
        cgf->removePostLoadScenarioCallback(&MyStartingPoint::PostLoadScenarioCallback, this);
    }

    /// <summary>
    /// This callback is triggered after scenario is loaded
    /// </summary>
    /// <param name="scenario">Scenario information</param>
    /// <param name="usr">Commander instance</param>
    void MyStartingPoint::PostLoadScenario(const DtScenario& scenario, void* usr)
    {
        print("[{}]\n", __FUNCTION__);
    }

    /// <summary>
    /// This callback is triggered before a scenario is closed
    /// </summary>
    /// <param name="usr">Commander instance</param>
    void MyStartingPoint::PreCloseScenario(void* usr)
    {
        print("[{}]\n", __FUNCTION__);
    }

    void MyStartingPoint::PostTick(void* usr)
    {
        if (!config.get()->settings.get()->isEnablePostTickLogic)
        {
            return;
        }
        
        print("[{}]\n", __FUNCTION__);
    }
}