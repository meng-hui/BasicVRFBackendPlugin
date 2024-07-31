#include "Config.h"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <Windows.h>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/fmt/fmt.h>

namespace BasicVRFBEPlugin
{
    /// <summary>
    /// Default Constructor
    /// </summary>
    /// <returns></returns>
	Config::Config()
	{
        // configure logging
        const auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::warn);

        // 10MB log, 3 rotating files
        const auto rotating_file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/myLog.txt", 1024 * 1024 * 10, 3);
        rotating_file_sink->set_level(spdlog::level::trace);

        // configure default logger
        const spdlog::sinks_init_list sinks = { console_sink, rotating_file_sink };
        const auto logger = std::make_shared<spdlog::logger>("logger", sinks);
        spdlog::register_logger(logger);
        spdlog::set_default_logger(logger);

        spdlog::flush_every(std::chrono::seconds(3));
        // change log pattern
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] [thread %t] %v", spdlog::pattern_time_type::local);

        // configure config file
        std::string simulationProtocol = "DIS";
        std::string plugin = "Plugin";

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
        using namespace std::string_literals;

        std::string dllName = "BasicVRFBE"s + simulationProtocol + plugin + ".dll"s;

        constexpr int pathSize = 2048;
        std::vector<char> dllPath(pathSize);
        GetModuleFileName(GetModuleHandle(dllName.c_str()), &dllPath.at(0), pathSize);
        std::experimental::filesystem::path p(dllPath.begin(), dllPath.end());
        std::string jsonPath = p.parent_path().string() + '/' + p.stem().generic_string() + ".json";

        if (!std::experimental::filesystem::exists(jsonPath))
        {
            spdlog::info("[{}] No config file found. Creating default config file at {}\n", __FUNCTION__, jsonPath);

            //create default settings
            settings = std::make_unique<Data::Settings>(Data::Settings
            {
                true, //isEnable
                true, //isEnablePostTickLogic
                false //isEnableDebugPrint
            });

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

    Data::Settings Config::getSettings() const
    {
        return *(settings.get());
    }

    /// <summary>
    /// Writes a default config file to disk
    /// </summary>
    /// <param name="settings">Default settings</param>
    /// <param name="path">Path to config file</param>
    void Config::WriteConfig(const Data::Settings* settings, const std::string& path)
    {
        //auto-magic serialization
        nlohmann::json j = *settings;

        //write to file
        std::ofstream o(path);
        o << j.dump(4) << std::endl;
    }

    /// <summary>
    /// Reads config file from disk
    /// </summary>
    /// <param name="path">Path to config file</param>
    void Config::ReadConfig(const std::string& path)
    {
        //read file from disk
        std::ifstream i(path);

        //auto-magic deserialization
        nlohmann::json j;
        i >> j;
        settings = std::make_unique<Data::Settings>(j.get<Data::Settings>());
    }
}