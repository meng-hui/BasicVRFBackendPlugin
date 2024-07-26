#pragma once

#include <nlohmann/json.hpp>

namespace BasicVRFBEPlugin
{
	namespace Data
	{
		class Settings final
		{
			public:
				bool isEnable = true;
				bool isEnablePostTickLogic = true;
				bool isEnableDebugPrint = false;
				std::string listenAddress = "0.0.0.0";
				short listenPort = 12345;

				friend void to_json(nlohmann::json& j, const Settings& cs)
				{
					j = nlohmann::json
					{
						{ "IsEnable", cs.isEnable},
						{ "IsEnablePostTickLogic", cs.isEnablePostTickLogic},
						{ "IsEnableDebugPrint", cs.isEnableDebugPrint},
						{ "ListenAddress", cs.listenAddress},
						{ "ListenPort", cs.listenPort}
					};
				}

				friend void from_json(const nlohmann::json& j, Settings& cs)
				{
					j.at("IsEnable").get_to(cs.isEnable);
					j.at("IsEnablePostTickLogic").get_to(cs.isEnablePostTickLogic);
					j.at("IsEnableDebugPrint").get_to(cs.isEnableDebugPrint);
					j.at("ListenAddress").get_to(cs.listenAddress);
					j.at("ListenPort").get_to(cs.listenPort);
				}
		};
	}
}