#include "MyStartingPoint.h"

namespace BasicVRFBEPlugin
{
    /// <summary>
    /// Default Constructor
    /// </summary>
    /// <param name="cgf">The all important access into the VRF world</param>
    MyStartingPoint::MyStartingPoint(DtCgf* cgf, std::unique_ptr<Config> config)
        : cgf(cgf), config(std::move(config))
	{
        AddCallbacks();
        fmt::print("[{}]\n", __FUNCTION__);
	}

    /// <summary>
    /// Default destructor
    /// </summary>
    MyStartingPoint::~MyStartingPoint()
	{
        RemoveCallbacks();
        fmt::print("[{}]\n", __FUNCTION__);
	}

    void MyStartingPoint::onDataReceived(std::vector<char> data)
    {
        std::string receivedData(data.begin(), data.end());
        fmt::print("[{}] {}\n", __FUNCTION__, receivedData);
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
    void MyStartingPoint::PostLoadScenario(const DtScenario& scenario)
    {
        fmt::print("[{}]\n", __FUNCTION__);
    }

    /// <summary>
    /// This callback is triggered before a scenario is closed
    /// </summary>
    /// <param name="usr">Object that was passed in</param>
    void MyStartingPoint::PreCloseScenario()
    {
        fmt::print("[{}]\n", __FUNCTION__);
    }

    /// <summary>
    /// This callback is triggered every VRF update
    /// </summary>
    /// <param name="usr">Object that was passed in</param>
    void MyStartingPoint::PostTick()
    {
        if (!config.get()->getSettings().isEnablePostTickLogic)
        {
            return;
        }
        
        fmt::print("[{}]\n", __FUNCTION__);
    }
}