#pragma once

#include "Data/Settings.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/fmt/fmt.h>

using namespace std;
using namespace fmt;

using namespace BasicVRFBEPlugin::Data;

namespace BasicVRFBEPlugin
{
	class Config
	{
		public:
			/// <summary>
			/// Default Constructor
			/// </summary>
			/// <returns></returns>
			Config();

			/// <summary>
			/// Default Destructor
			/// </summary>
			/// <returns></returns>
			~Config();

			/// <summary>
			/// Current settings
			/// </summary>
			std::unique_ptr<Settings> settings;

		private:
			/// <summary>
			/// Writes a default config file to disk
			/// </summary>
			/// <param name="settings">Default settings</param>
			/// <param name="path">Path to config file</param>
			void WriteConfig(Settings* settings, string path);

			/// <summary>
			/// Reads config file from disk
			/// </summary>
			/// <param name="path">Path to config file</param>
			void ReadConfig(string path);
	};
}