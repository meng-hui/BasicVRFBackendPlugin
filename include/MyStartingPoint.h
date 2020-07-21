#pragma once

#include "Config.h"
#include "VRFCore.h"

using namespace fmt;
using namespace std;

namespace BasicVRFBEPlugin
{
	class MyStartingPoint
	{
		public:
			/// <summary>
			/// Default Constructor
			/// </summary>
			/// <param name="cgf">The all important access into the VRF world</param>
			MyStartingPoint(DtCgf* cgf, unique_ptr<Config> config);

			/// <summary>
			/// Default Destructor
			/// </summary>
			~MyStartingPoint();

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
				myStartingPoint->PostLoadScenario(scenario, usr);
			}

			/// <summary>
			/// This callback is triggered after scenario is loaded
			/// </summary>
			/// <param name="scenario">Scenario information</param>
			/// <param name="usr">Object that was passed in</param>
			void PostLoadScenario(const DtScenario& scenario, void* usr);

			/// <summary>
			/// This callback is triggered before a scenario is closed
			/// </summary>
			/// <param name="usr">Object that was passed in</param>
			static void PreCloseScenarioCallback(void* usr)
			{
				MyStartingPoint* myStartingPoint = static_cast<MyStartingPoint*>(usr);
				myStartingPoint->PreCloseScenario(usr);
			}

			/// <summary>
			/// This callback is triggered before a scenario is closed
			/// </summary>
			/// <param name="usr">Object that was passed in</param>
			void PreCloseScenario(void* usr);

			/// <summary>
			/// This callback is triggered every VRF update
			/// Functor workaround
			/// </summary>
			/// <param name="usr">Object that was passed in</param>
			static void PostTickCallback(void* usr)
			{
				MyStartingPoint* myStartingPoint = static_cast<MyStartingPoint*>(usr);
				myStartingPoint->PostTick(usr);
			}

			/// <summary>
			/// This callback is triggered every VRF update
			/// </summary>
			/// <param name="usr">Object that was passed in</param>
			void PostTick(void* usr);

			/// <summary>
			/// The all important access into the VRF world
			/// </summary>
			DtCgf* cgf = nullptr;

			/// <summary>
			/// Configuration file
			/// </summary>
			unique_ptr<Config> config = nullptr;
	};
}