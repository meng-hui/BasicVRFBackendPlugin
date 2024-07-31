#include "plugin.h"

void DtPluginInformation(DtVrfPluginInformation& info)
{
    fmt::print("[{}]\n", __FUNCTION__);

    info.pluginName = "My new VRF plugin";
    info.pluginVersion = "0.1";
    info.pluginCreator = "My name";
    info.pluginCreatorEmail = "myemail@email.com";
    info.pluginDescription = "What my plugin does";
}

bool DtInitializeVrfPlugin(DtCgf* cgf)
{
    using namespace BasicVRFBEPlugin;

    fmt::print("[{}]\n", __FUNCTION__);

    VRFCore::cgf = cgf;

    std::unique_ptr<Config> config = std::make_unique<Config>("simLogger");
    if (config.get()->getSettings().isEnable)
    {
        myStartingPoint = std::make_unique<MyStartingPoint>(cgf, move(config));
    }
    else
    {
        fmt::print("[{}] Plugin disabled\n", __FUNCTION__);
    }

    g_grpc_server = std::make_unique<ServerImpl>(std::make_unique<VRFServerPlugin::CgfHelper>(*cgf));
    g_grpc_server->Run(50051u);

    return true;
}

bool DtPostInitializeVrfPlugin(DtCgf* cgf)
{
    fmt::print("[{}]\n", __FUNCTION__);

    return true;
}

void DtUnloadVrfPlugin()
{
    fmt::print("[{}]\n", __FUNCTION__);
    g_grpc_server->Shutdown();
    g_grpc_server.reset();
    myStartingPoint.reset();
}