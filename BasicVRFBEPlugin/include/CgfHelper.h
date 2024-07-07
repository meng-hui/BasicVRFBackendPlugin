#pragma once
#include "dt_cgf.grpc.pb.h"

#include <spdlog/logger.h>

#include <vrfcgf/cgf.h>
#include <vrfplan/planBuilder.h>

#include <vrfutil/simCondExpr.h>

namespace VRFServerPlugin
{
	class CgfHelper final 
	{
	public:
		CgfHelper(DtCgf& cgf);
		VRFServerPlugin::ListSimObjectsResponse getSimObjects();
		VRFServerPlugin::PlanAssignmentResponse assignPlan(const VRFServerPlugin::PlanAssignmentRequest& plan_assignment_request);
	private:
		DtSimCondExpr* getCondition(const VRFServerPlugin::Condition& condition);
		DtSimTask* getTask(const VRFServerPlugin::Task& task);
		DtSetDataRequest* getSetRequest(const VRFServerPlugin::SetRequest& setRequest);
		void addStatement(DtPlanBuilder& planBuilder, const VRFServerPlugin::Statement& statement);
		void addTriggerStatement(DtPlanBuilder& planBuilder, const VRFServerPlugin::TriggerStatement triggerStatement);
		void addStatementWithoutTrigger(DtPlanBlockBuilder& planBuilder, const VRFServerPlugin::Statement& statement);
		void addIfStatement(DtPlanBlockBuilder& planBuilder, const VRFServerPlugin::IfStatement& ifStatement);
		void addWhileStatement(DtPlanBlockBuilder& planBuilder, const VRFServerPlugin::WhileStatement& whileStatement);
		DtSimObjectReference getSimObject(const std::string& object_name);
		std::string getRulesOfEngagement(const VRFServerPlugin::RulesOfEngagementType& type);

		DtCgf& cgf;
		std::shared_ptr<spdlog::logger> logger;
	};
}
