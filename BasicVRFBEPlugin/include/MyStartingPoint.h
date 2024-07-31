#pragma once

#include "Config.h"

#include <vrfcgf/cgf.h>

namespace BasicVRFBEPlugin
{
	class MyStartingPoint final
	{
		public:
			/// <summary>
			/// Constructor
			/// </summary>
			/// <param name="cgf">The all important access into the VRF world</param>
			MyStartingPoint(DtCgf* cgf, std::unique_ptr<Config> config);

			~MyStartingPoint();
			MyStartingPoint(const MyStartingPoint&) = delete;
			MyStartingPoint& operator=(const MyStartingPoint&) = delete;

		private:
			
			/// <summary>
			/// Subscribe to VR-Force callbacks
			/// </summary>
			void AddCallbacks();

			/// <summary>
			/// Unsubscribe to VR-Force callbacks
			/// </summary>
			void RemoveCallbacks();

			/// <summary>
			/// This callback is triggered after scenario is loaded
			/// Functor workaround
			/// </summary>
			/// <param name="scenario">Scenario information</param>
			/// <param name="usr">Object that was passed in</param>
			static void PostLoadScenarioCallback(const DtScenario& scenario, void* usr)
			{
				MyStartingPoint* myStartingPoint = static_cast<MyStartingPoint*>(usr);
				myStartingPoint->PostLoadScenario(scenario);
			}

			/// <summary>
			/// This callback is triggered after scenario is loaded
			/// </summary>
			/// <param name="scenario">Scenario information</param>
			/// <param name="usr">Object that was passed in</param>
			void PostLoadScenario(const DtScenario& scenario);

			/// <summary>
			/// This callback is triggered before a scenario is closed
			/// </summary>
			/// <param name="usr">Object that was passed in</param>
			static void PreCloseScenarioCallback(void* usr)
			{
				MyStartingPoint* myStartingPoint = static_cast<MyStartingPoint*>(usr);
				myStartingPoint->PreCloseScenario();
			}

			/// <summary>
			/// This callback is triggered before a scenario is closed
			/// </summary>
			/// <param name="usr">Object that was passed in</param>
			void PreCloseScenario();

			/// <summary>
			/// This callback is triggered every VRF update
			/// Functor workaround
			/// </summary>
			/// <param name="usr">Object that was passed in</param>
			static void PostTickCallback(void* usr)
			{
				MyStartingPoint* myStartingPoint = static_cast<MyStartingPoint*>(usr);
				myStartingPoint->PostTick();
			}

			/// <summary>
			/// This callback is triggered every VRF update
			/// </summary>
			/// <param name="usr">Object that was passed in</param>
			void PostTick();

			/// <summary>
			/// The all important access into the VRF world
			/// </summary>
			DtCgf* cgf = nullptr;

			/// <summary>
			/// Configuration file
			/// </summary>
			std::unique_ptr<const Config> config = nullptr;
	};
}