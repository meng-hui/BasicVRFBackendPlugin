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

				friend void to_json(nlohmann::json& j, const Settings& cs)
				{
					j = nlohmann::json
					{
						{ "IsEnable", cs.isEnable},
						{ "IsEnablePostTickLogic", cs.isEnablePostTickLogic},
						{ "IsEnableDebugPrint", cs.isEnableDebugPrint}
					};
				}

				friend void from_json(const nlohmann::json& j, Settings& cs)
				{
					j.at("IsEnable").get_to(cs.isEnable);
					j.at("IsEnablePostTickLogic").get_to(cs.isEnablePostTickLogic);
					j.at("IsEnableDebugPrint").get_to(cs.isEnableDebugPrint);
				}
		};
	}
}