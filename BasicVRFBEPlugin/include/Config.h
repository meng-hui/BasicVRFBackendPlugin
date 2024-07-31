#pragma once

#include "Data/Settings.h"
#include <boost/optional.hpp>

namespace BasicVRFBEPlugin
{
	class Config final
	{
		public:
			// Rule of Zero: Default destructor and delete copy constructor and copy assignment operator
			
			Config(std::string loggerName, boost::optional<std::string> configPath = boost::none);
			~Config() = default;
			Config(const Config&) = delete;
			Config& operator=(const Config&) = delete;

			Data::Settings getSettings() const;
		private:
			/// <summary>
			/// Writes a default config file to disk
			/// </summary>
			/// <param name="settings">Default settings</param>
			/// <param name="path">Path to config file</param>
			static void WriteConfig(const BasicVRFBEPlugin::Data::Settings* settings, const std::string& path);

			/// <summary>
			/// Reads config file from disk
			/// </summary>
			/// <param name="path">Path to config file</param>
			void ReadConfig(const std::string& path);

			/// <summary>
			/// Current settings
			/// </summary>
			std::unique_ptr<const BasicVRFBEPlugin::Data::Settings> settings;
	};
}