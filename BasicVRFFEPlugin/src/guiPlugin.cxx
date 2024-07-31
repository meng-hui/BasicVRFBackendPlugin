#include "guiPlugin.h"
#include "Config.h"

#include <vrvCore/DtDe.h>

#include <spdlog/fmt/fmt.h>

bool initDeModule(makVrv::DtDe* de)
{
    BasicVRFFEPlugin::init(*de);
    return true;
}

void BasicVRFFEPlugin::init(makVrv::DtDe& de)
{
    DT_DE_INIT_ONCE(de);

    fmt::print("[{}]\n", __FUNCTION__);

    try 
    {
        // in the FE, the config cannot be auto found like BE. 
        // Pass in the path to the config file used by the BE.
        std::string path = "..\\plugins64\\release\\BasicVRFBEHLA1516ePlugin.json";
        std::unique_ptr<BasicVRFBEPlugin::Config> config = std::make_unique<BasicVRFBEPlugin::Config>("guiLogger", path);
        if (config.get()->getSettings().isEnable)
        {
            fmt::print("[{}] Plugin enabled\n", __FUNCTION__);
            // This is where you would have to do what is needed for your FE plugin
            // e.g. DtScriptEditor::registerInstance
            // e.g. get instance, DtInformationDialogManager::instance
            // e.g. DtQtPageAssembler::instance(de).registerPageCreator
            // it's complicated so there is no basic FE example to share.
        }
        else
        {
            fmt::print("[{}] Plugin disabled\n", __FUNCTION__);
        }
    }
    DtCATCH_AND_PROPAGATE(DtCorruptedState)
}

void postInitDeModule(makVrv::DtDe& de)
{
    return void();
}

void DtPluginInformation(DtVrfPluginInformation& info)
{
    fmt::print("[{}]\n", __FUNCTION__);

    info.pluginName = "My new VRF plugin";
    info.pluginVersion = "0.1";
    info.pluginCreator = "My name";
    info.pluginCreatorEmail = "myemail@email.com";
    info.pluginDescription = "What my plugin does";
}
