#include "Config.h"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <Windows.h>

#include <nlohmann/json.hpp>

using namespace std::experimental::filesystem;
using json = nlohmann::json;

namespace BasicVRFBEPlugin
{
    /// <summary>
    /// Default Constructor
    /// </summary>
    /// <returns></returns>
	Config::Config()
	{
        string simulationProtocol = "";
        string plugin = "Plugin";
#ifdef DIS
        simulationProtocol = "DIS";
#endif
#ifdef HLA13
        simulationProtocol = "HLA13";
#endif
#ifdef HLA1516
        simulationProtocol = "HLA1516";
#endif
#ifdef HLA1516e
        simulationProtocol = "HLA1516e";
#endif

#ifndef NDEBUG
        plugin = "Plugind";
#endif

        string dllName = "BasicVRFBE"s + simulationProtocol + plugin + ".dll"s;

        char dllPath[2048];
        GetModuleFileNameA(GetModuleHandle(dllName.c_str()), dllPath, 2048);
        path p(dllPath);
        string jsonPath = p.parent_path().string() + '/' + p.stem().generic_string() + ".json";

        if (!exists(jsonPath))
        {
            print("[{}] No config file found. Creating default config file at {}\n", __FUNCTION__, jsonPath);

            //create default settings
            settings = make_unique<Settings>();
            settings->isEnable = true;
            settings->isEnableDebugPrint = false;
            settings->isEnablePostTickLogic = true;

            //write config
            WriteConfig(settings.get(), jsonPath);
        }
        else
        {
            print("[{}] Config file found at {}\n", __FUNCTION__, jsonPath);

            //read config
            ReadConfig(jsonPath);
        }
	}

    /// <summary>
    /// Default Destructor
    /// </summary>
    /// <returns></returns>
    Config::~Config() {}

    /// <summary>
    /// Writes a default config file to disk
    /// </summary>
    /// <param name="settings">Default settings</param>
    /// <param name="path">Path to config file</param>
    void Config::WriteConfig(Settings* settings, string path)
    {
        //auto-magic serialization
        json j = *settings;

        //write to file
        std::ofstream o(path);
        o << j.dump(4) << std::endl;
    }

    /// <summary>
    /// Reads config file from disk
    /// </summary>
    /// <param name="path">Path to config file</param>
    void Config::ReadConfig(string path)
    {
        //read file from disk
        std::ifstream i(path);

        //auto-magic deserialization
        json j;
        i >> j;
        settings = make_unique<Settings>(j.get<Settings>());
    }
}