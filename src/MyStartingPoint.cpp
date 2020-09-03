#include "MyStartingPoint.h"

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
        cgf->addPostLoadScenarioCallback(PostLoadScenarioCallback, this);
        cgf->addPreCloseScenarioCallback(PreCloseScenarioCallback, this);
        cgf->addPostTickCallback(PostTickCallback, this);
    }

    /// <summary>
    /// Unsubscribe to VR-Force callbacks
    /// </summary>
    void MyStartingPoint::RemoveCallbacks()
    {
        cgf->removePostTickCallback(PostTickCallback, this);
        cgf->removePreCloseScenarioCallback(PreCloseScenarioCallback, this);
        cgf->removePostLoadScenarioCallback(PostLoadScenarioCallback, this);
    }

    /// <summary>
    /// This callback is triggered after scenario is loaded
    /// </summary>
    /// <param name="scenario">Scenario information</param>
    /// <param name="usr">Object that was passed in</param>
    void MyStartingPoint::PostLoadScenario(const DtScenario& scenario, void* usr)
    {
        print("[{}]\n", __FUNCTION__);
    }

    /// <summary>
    /// This callback is triggered before a scenario is closed
    /// </summary>
    /// <param name="usr">Object that was passed in</param>
    void MyStartingPoint::PreCloseScenario(void* usr)
    {
        print("[{}]\n", __FUNCTION__);
    }

    /// <summary>
    /// This callback is triggered every VRF update
    /// </summary>
    /// <param name="usr">Object that was passed in</param>
    void MyStartingPoint::PostTick(void* usr)
    {
        if (!config.get()->settings.get()->isEnablePostTickLogic)
        {
            return;
        }
        
        print("[{}]\n", __FUNCTION__);
    }
}