#pragma once

#include "Data/Settings.h"
#include <optional>

namespace BasicVRFBEPlugin
{
	class Config final
	{
		public:
			Config(std::string loggerName, std::optional<std::string> configPath = std::nullopt);

			Data::Settings getSettings() const;
		private:
			/// <summary>
			/// Writes a default config file to disk
			/// </summary>
			/// <param name="settings">Default settings</param>
			/// <param name="path">Path to config file</param>
			static void WriteConfig(const Data::Settings& settings, const std::string& path);

			/// <summary>
			/// Reads config file from disk
			/// </summary>
			/// <param name="path">Path to config file</param>
			void ReadConfig(const std::string& path);

			/// <summary>
			/// Current settings
			/// </summary>
			Data::Settings settings;
	};
}