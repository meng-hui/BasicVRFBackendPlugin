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
        // configure logging
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::warn);

        // 10MB log, 3 rotating files
        auto rotating_file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/myLog.txt", 1024 * 1024 * 10, 3);
        rotating_file_sink->set_level(spdlog::level::trace);

        // configure default logger
        std::shared_ptr<spdlog::logger> logger(new spdlog::logger("logger", { console_sink, rotating_file_sink }));
        spdlog::register_logger(logger);
        spdlog::set_default_logger(logger);

        spdlog::flush_every(std::chrono::seconds(3));
        // change log pattern
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] [thread %t] %v");

        // configure config file
        string simulationProtocol = "DIS";
        string plugin = "Plugin";

#ifdef DtDIS
        simulationProtocol = "DIS";
#elif DtHLA_1516_EVOLVED
        simulationProtocol = "HLA1516e";
#elif DtHLA_1516
        simulationProtocol = "HLA1516";
#elif DtHLA
        simulationProtocol = "HLA13";
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
            spdlog::info("[{}] No config file found. Creating default config file at {}\n", __FUNCTION__, jsonPath);

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
            spdlog::info("[{}] Config file found at {}\n", __FUNCTION__, jsonPath);

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