#include "plugin.h"
#include "UDPNetwork.h"

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

    std::unique_ptr<Config> config = std::make_unique<Config>();
    if (config.get()->getSettings().isEnable)
    {
        const boost::asio::ip::address listen_address = boost::asio::ip::address::from_string(config.get()->getSettings().listenAddress);
        const auto port = config.get()->getSettings().listenPort;

        myStartingPoint = std::make_unique<MyStartingPoint>(cgf, move(config));

        t = boost::thread(boost::bind(&NetworkUpdate, listen_address, port));
    }
    else
    {
        fmt::print("[{}] Plugin disabled\n", __FUNCTION__);
    }

    return true;
}

void NetworkUpdate(const boost::asio::ip::address& listenAddress, short port)
{
    using namespace BasicVRFBEPlugin;

    UDPNetwork udpNetwork(io_service, listenAddress, port);

    udpNetwork.registerCallback([](std::vector<char> data) {
        myStartingPoint->onDataReceived(data);
    });

    io_service.run();
}

bool DtPostInitializeVrfPlugin(DtCgf* cgf)
{
    fmt::print("[{}]\n", __FUNCTION__);

    return true;
}

void DtUnloadVrfPlugin()
{
    fmt::print("[{}]\n", __FUNCTION__);

    myStartingPoint.reset();
}