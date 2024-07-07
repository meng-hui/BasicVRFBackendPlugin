#include "CgfHelper.h"

#include <spdlog/spdlog.h>

#include <vrfobjcore/simObjectManager.h>
#include <vlpi/entityType.h>

#include <vrfutil/ceEntInArea.h>
#include <vrfutil/ceEntUnderFire.h>
#include <vrfutil/ceAnd.h>
#include <vrfutil/ceOr.h>

#include <vrftasks/fireAtTargetTask.h>
#include <vrftasks/moveAlongTasks.h>
#include <vrftasks/moveToTask.h>
#include <vrftasks/patrolRouteTask.h>
#include <vrftasks/patrolTwoPointsTask.h>
#include <vrftasks/waitDurationTask.h>

#include <vrftasks/setSpeedRequest.h>
#include <vrftasks/setEngagementRulesRequest.h>

VRFServerPlugin::CgfHelper::CgfHelper(DtCgf& cgf) : cgf(cgf), logger(spdlog::get("logger"))
{
}

VRFServerPlugin::ListSimObjectsResponse VRFServerPlugin::CgfHelper::getSimObjects()
{
	auto list_sim_objects_response = VRFServerPlugin::ListSimObjectsResponse();

    auto simObjects = cgf.simObjectManager()->simObjects();
    for (auto& kvp : simObjects)
    {
        if (!kvp.second->isEntityOrUnit()) { continue; }
        auto new_sim_object = list_sim_objects_response.add_sim_objects();

        new_sim_object->set_name(std::string(kvp.second->objectName().c_str()));
        new_sim_object->set_entity_type(std::string(kvp.second->entityType().string()));
        new_sim_object->set_force_type((VRFServerPlugin::DtForceType)kvp.second->forceType());
    }

    return list_sim_objects_response;
}

VRFServerPlugin::PlanAssignmentResponse VRFServerPlugin::CgfHelper::assignPlan(const VRFServerPlugin::PlanAssignmentRequest& plan_assignment_request)
{
    if (!plan_assignment_request.has_plan()) 
    {
        VRFServerPlugin::PlanAssignmentResponse response;
        response.set_success(false);
        return response;
    }

    auto entity = getSimObject(plan_assignment_request.entity_id());

    DtPlanBuilder planBuilder;

    auto& plan = plan_assignment_request.plan();
    planBuilder.setPlanName(plan.name());
    for (auto& statement : plan.statements())
    {
        addStatement(planBuilder, statement);
    }
    
    cgf.assignPlanByUUID(entity->uuid(), planBuilder);
    VRFServerPlugin::PlanAssignmentResponse response;
    response.set_success(true);
    return response;
}

DtSimCondExpr* VRFServerPlugin::CgfHelper::getCondition(const VRFServerPlugin::Condition& condition)
{
    auto simObjectManager = cgf.simObjectManager();

    switch (condition.condition_type_case())
    {
    case VRFServerPlugin::Condition::kEntityInArea: {
        auto ceEntInArea = new DtCeEntInArea();
        ceEntInArea->setArea(simObjectManager->lookup(DtUUID(condition.entity_in_area().area_name())));
        ceEntInArea->setEntity(simObjectManager->lookup(DtUUID(condition.entity_in_area().entity_name())));
        return ceEntInArea;
        break;
    }
    case VRFServerPlugin::Condition::kEntityUnderFire: {
        auto ceEntUnderFire = new DtCeEntUnderFire();
        ceEntUnderFire->setEntity(simObjectManager->lookup(DtUUID(condition.entity_under_fire().entity_name())));
        return ceEntUnderFire;
        break;
    }
    case VRFServerPlugin::Condition::kAndCondition: {
        auto ceAnd = new DtCeAnd();
        for (auto& c : condition.and_condition().conditions())
        {
            ceAnd->addCondition(*getCondition(c));
        }
        return ceAnd;
        break;
    }
    case VRFServerPlugin::Condition::kOrCondition: {
        auto ceOr = new DtCeOr();
        for (auto& c : condition.or_condition().conditions())
        {
            ceOr->addCondition(*getCondition(c));
        }
        return ceOr;
        break;
    }
    case VRFServerPlugin::Condition::CONDITION_TYPE_NOT_SET:
        throw std::invalid_argument("Condition type not set");
        break;
    default:
        throw std::invalid_argument("Unknown condition type");
        break;
    }
}

DtSimTask* VRFServerPlugin::CgfHelper::getTask(const VRFServerPlugin::Task& task)
{
    switch (task.task_type_case()) 
    {
    case VRFServerPlugin::Task::kFireAtTarget: {
        auto vrfTask = new DtFireAtTargetTask();
        vrfTask->setTarget(getSimObject(task.fire_at_target().entity_name()));
        return vrfTask;
        break;
    }
    case VRFServerPlugin::Task::kMoveAlongRoute: {
        auto vrfTask = new DtMoveAlongTask();
        vrfTask->setRoute(getSimObject(task.move_along_route().entity_name()));
        return vrfTask;
        break;
    }
    case VRFServerPlugin::Task::kMoveToLocation: {
        auto vrfTask = new DtMoveToTask();
        vrfTask->setControlPoint(getSimObject(task.move_to_location().entity_name()));
        return vrfTask;
        break;
    }
    case VRFServerPlugin::Task::kPatrolRoute: {
        auto vrfTask = new DtPatrolRouteTask();
        vrfTask->setRoute(getSimObject(task.patrol_route().entity_name()));
        return vrfTask;
        break;
    }
    case VRFServerPlugin::Task::kPatrolWaypoints: {
        auto vrfTask = new DtPatrolTwoPointsTask();
        vrfTask->setFirstControlPoint(getSimObject(task.patrol_waypoints().entity_name_1()));
        vrfTask->setSecondControlPoint(getSimObject(task.patrol_waypoints().entity_name_2()));
        return vrfTask;
        break;
    }
    case VRFServerPlugin::Task::kWaitDuration: {
        auto vrfTask = new DtWaitDurationTask();
        vrfTask->setSecondsToWait(task.wait_duration().duration_seconds());
        return vrfTask;
        break;
    }
    case VRFServerPlugin::Task::TASK_TYPE_NOT_SET:
        throw std::invalid_argument("Task type not set");
        break;
    default:
        throw std::invalid_argument("Unknown task type");
        break;
    }
}

DtSetDataRequest* VRFServerPlugin::CgfHelper::getSetRequest(const VRFServerPlugin::SetRequest& setRequest)
{
    switch (setRequest.set_request_type_case()) 
    {
    case VRFServerPlugin::SetRequest::kSetOrderedSpeed: {
        auto vrfSet = new DtSetSpeedRequest();
        vrfSet->setSpeed(setRequest.set_ordered_speed().speed());
        return vrfSet;
        break;
    }
    case VRFServerPlugin::SetRequest::kSetRulesOfEngagement: {
        auto vrfSet = new DtSetEngagementRulesRequest();
        vrfSet->setEngagementRules(getRulesOfEngagement(setRequest.set_rules_of_engagement().type()));
        return vrfSet;
        break;
    }
    case VRFServerPlugin::SetRequest::SET_REQUEST_TYPE_NOT_SET:
        throw std::invalid_argument("Set Request type not set");
        break;
    default:
        throw std::invalid_argument("Unknown set request type");
        break;
    }
}

void VRFServerPlugin::CgfHelper::addStatement(DtPlanBuilder& planBuilder, const VRFServerPlugin::Statement& statement)
{
    if (statement.statement_type_case() == VRFServerPlugin::Statement::kTriggerStatement)
    {
        addTriggerStatement(planBuilder, statement.trigger_statement());
    }
    else 
    {
        addStatementWithoutTrigger(planBuilder, statement);
    }
}

void VRFServerPlugin::CgfHelper::addTriggerStatement(DtPlanBuilder& planBuilder, const VRFServerPlugin::TriggerStatement triggerStatement)
{
    auto condition = getCondition(triggerStatement.condition());
    auto &description = triggerStatement.description();
    DtUUID uuid;
    uuid.generateUUID();
    auto blockBuilder = planBuilder.addTriggerStatement(*condition, uuid, description, true, "ReTask", false);
    for (auto& statement : triggerStatement.statements())
    {
        addStatementWithoutTrigger(*blockBuilder.get(), statement);
    }
}

void VRFServerPlugin::CgfHelper::addStatementWithoutTrigger(DtPlanBlockBuilder& planBuilder, const VRFServerPlugin::Statement& statement)
{
    switch (statement.statement_type_case())
    {
    case VRFServerPlugin::Statement::kTask: {
        auto task = getTask(statement.task());
        planBuilder.addStatement(*task);
        break;
    }
    case VRFServerPlugin::Statement::kSetRequest: {
        auto setRequest = getSetRequest(statement.set_request());
        planBuilder.addStatement(*setRequest);
        break;
    }
    case VRFServerPlugin::Statement::kIfStatement: {
        addIfStatement(planBuilder, statement.if_statement());
        break;
    }
    case VRFServerPlugin::Statement::kWhileStatement: {
        addWhileStatement(planBuilder, statement.while_statement());
        break;
    }
    case VRFServerPlugin::Statement::kTriggerStatement:
        throw std::invalid_argument("Cannot handle triggers in this plan block. Triggers can only be top level statements");
        break;
    case VRFServerPlugin::Statement::STATEMENT_TYPE_NOT_SET:
        throw std::invalid_argument("Statement type not set");
        break;
    default:
        throw std::invalid_argument("Unknown statement type");
        break;
    }
}

void VRFServerPlugin::CgfHelper::addIfStatement(DtPlanBlockBuilder& planBuilder, const VRFServerPlugin::IfStatement& ifStatement)
{
    DtPlanBlockBuilder::SharedPlanBlockBuilder thenBlockBuilder, elseBlockBuilder;
    auto condition = getCondition(ifStatement.condition());
    planBuilder.addIfCondition(*condition, thenBlockBuilder, elseBlockBuilder);
    for (auto& statement : ifStatement.then_statements())
    {
        addStatementWithoutTrigger(*thenBlockBuilder.get(), statement);
    }
    for (auto& statement : ifStatement.else_statements())
    {
        addStatementWithoutTrigger(*elseBlockBuilder.get(), statement);
    }
}

void VRFServerPlugin::CgfHelper::addWhileStatement(DtPlanBlockBuilder& planBuilder, const VRFServerPlugin::WhileStatement& whileStatement)
{
    auto condition = getCondition(whileStatement.condition());
    auto blockBuilder = planBuilder.addWhileCondition(*condition);
    for (auto& statement : whileStatement.statements())
    {
        addStatementWithoutTrigger(*blockBuilder.get(), statement);
    }
}

DtSimObjectReference VRFServerPlugin::CgfHelper::getSimObject(const std::string& object_name)
{
    auto simObjectManager = cgf.simObjectManager();
    auto entity = simObjectManager->lookup(DtUUID(object_name));
    if (!entity.isValid()) 
    {
        logger->warn("Entity is invalid. Name: {}", object_name);
    }

    return entity;
}

std::string VRFServerPlugin::CgfHelper::getRulesOfEngagement(const VRFServerPlugin::RulesOfEngagementType& type)
{
    switch (type)
    {
    case VRFServerPlugin::RulesOfEngagementType::FireAtWill: {
        return "fire-at-will";
        break;
    }
    case VRFServerPlugin::RulesOfEngagementType::FireWhenFiredUpon: {
        return "fire-when-fired-upon";
        break;
    }
    case VRFServerPlugin::RulesOfEngagementType::HoldFire: {
        return "hold-fire";
        break;
    }
    default:
        throw std::invalid_argument("Unknown rules of engagement type");
        break;
    }
}
