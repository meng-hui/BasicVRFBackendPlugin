#pragma once

#include <CgfHelper.h>

#include <grpcpp/grpcpp.h>
#include <spdlog/logger.h>

class ServerImpl final {
public:
    ServerImpl(std::unique_ptr<VRFServerPlugin::CgfHelper> cgfHelper);
    ~ServerImpl();

    void Run(uint16_t port);
    void Shutdown();

private:
    class CallData;

    void HandleRpcs();

    std::unique_ptr<grpc::ServerCompletionQueue> cq_;
    VRFServerPlugin::CgfService::AsyncService service_;
    std::unique_ptr<grpc::Server> server_;
    std::thread server_thread_;
    std::unique_ptr<VRFServerPlugin::CgfHelper> cgfHelper;
    std::shared_ptr<spdlog::logger> logger;
};

class ServerImpl::CallData {
public:
    CallData(VRFServerPlugin::CgfService::AsyncService* service, grpc::ServerCompletionQueue* cq, VRFServerPlugin::CgfHelper& cgfHelper, std::shared_ptr<spdlog::logger> logger);

    void Proceed();

private:
    VRFServerPlugin::CgfService::AsyncService* service_;
    grpc::ServerCompletionQueue* cq_;
    grpc::ServerContext ctx_;
    grpc::ServerAsyncResponseWriter<VRFServerPlugin::ListSimObjectsResponse> list_sim_objects_responder_;
    VRFServerPlugin::PlanAssignmentRequest plan_assignment_request_;
    grpc::ServerAsyncResponseWriter<VRFServerPlugin::PlanAssignmentResponse> plan_assignment_responder_;
    enum CallStatus { CREATE, PROCESS, FINISH };
    CallStatus status_;
    VRFServerPlugin::CgfHelper& cgfHelper;
    std::shared_ptr<spdlog::logger> logger;
};
