#include "plugin.h"

void DtPluginInformation(DtVrfPluginInformation& info)
{
    print("[{}]\n", __FUNCTION__);

    info.pluginName = "My new VRF plugin";
    info.pluginVersion = "0.1";
    info.pluginCreator = "My name";
    info.pluginCreatorEmail = "myemail@email.com";
    info.pluginDescription = "What my plugin does";
}

bool DtInitializeVrfPlugin(DtCgf* cgf)
{
    print("[{}]\n", __FUNCTION__);

    VRFCore::cgf = cgf;

    unique_ptr<Config> config = std::make_unique<Config>();
    if (config.get()->settings.get()->isEnable)
    {
        myStartingPoint = std::make_unique<MyStartingPoint>(cgf, move(config));
    }
    else
    {
        print("[{}] Plugin disabled\n", __FUNCTION__);
    }

    return true;
}

bool DtPostInitializeVrfPlugin(DtCgf* cgf)
{
    print("[{}]\n", __FUNCTION__);

    return true;
}

void DtUnloadVrfPlugin()
{
    print("[{}]\n", __FUNCTION__);

    myStartingPoint.reset();
}