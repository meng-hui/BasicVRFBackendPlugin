#include "ServerImpl.h"

#include <fmt/core.h>
#include <spdlog/spdlog.h>


ServerImpl::ServerImpl(std::unique_ptr<VRFServerPlugin::CgfHelper> cgfHelper) 
    :
    cgfHelper(std::move(cgfHelper)),
    logger(spdlog::get("logger"))
{
}

ServerImpl::~ServerImpl()
{
	server_->Shutdown();
	// Always shutdown the completion queue after the server.
	cq_->Shutdown();
}

void ServerImpl::Run(uint16_t port)
{
    std::string server_address = fmt::format("0.0.0.0:{}", port);

    grpc::ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service_" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *asynchronous* service.
    builder.RegisterService(&service_);
    // Get hold of the completion queue used for the asynchronous communication
    // with the gRPC runtime.
    cq_ = builder.AddCompletionQueue();
    // Finally assemble the server.
    server_ = builder.BuildAndStart();
    logger->info("Server listening on {}", server_address);

    // Proceed to the server's main loop in a separate thread.
    server_thread_ = std::thread([this]() { HandleRpcs(); });
}

void ServerImpl::Shutdown()
{
    server_->Shutdown();
    // Shutdown the completion queue after the server.
    cq_->Shutdown();
    // Wait for the server thread to finish.
    if (server_thread_.joinable()) {
        server_thread_.join();
    }
}

void ServerImpl::HandleRpcs()
{
    // Spawn a new CallData instance to serve new clients.
    new CallData(&service_, cq_.get(), *cgfHelper.get(), logger);
    void* tag;  // uniquely identifies a request.
    bool ok;
    while (true) {
        // Block waiting to read the next event from the completion queue. The
        // event is uniquely identified by its tag, which in this case is the
        // memory address of a CallData instance.
        // The return value of Next should always be checked. This return value
        // tells us whether there is any kind of event or cq_ is shutting down.
        GPR_ASSERT(cq_->Next(&tag, &ok));
        GPR_ASSERT(ok);
        static_cast<CallData*>(tag)->Proceed();
    }
}

ServerImpl::CallData::CallData(VRFServerPlugin::CgfService::AsyncService* service, grpc::ServerCompletionQueue* cq, VRFServerPlugin::CgfHelper& cgfHelper, std::shared_ptr<spdlog::logger> logger)
    : service_(service), cq_(cq), list_sim_objects_responder_(&ctx_), plan_assignment_responder_(&ctx_), status_(CREATE), cgfHelper(cgfHelper), logger(logger)
{
    Proceed();
}

void ServerImpl::CallData::Proceed()
{
    if (status_ == CREATE) {
        // Make this instance progress to the PROCESS state.
        status_ = PROCESS;

        // As part of the initial CREATE state, we *request* that the system
        // start processing SayHello requests. In this request, "this" acts are
        // the tag uniquely identifying the request (so that different CallData
        // instances can serve different requests concurrently), in this case
        // the memory address of this CallData instance.
        service_->RequestListSimObjects(&ctx_,
            const_cast<google::protobuf::Empty*>(&google::protobuf::Empty::default_instance()), 
            &list_sim_objects_responder_, 
            cq_, 
            cq_, 
            this);

        service_->RequestAssignPlan(&ctx_,
            &plan_assignment_request_,
            &plan_assignment_responder_,
            cq_,
            cq_,
            this);

    }
    else if (status_ == PROCESS) {
        // Spawn a new CallData instance to serve new clients while we process
        // the one for this CallData. The instance will deallocate itself as
        // part of its FINISH state.
        new CallData(service_, cq_, cgfHelper, logger);

        // The actual processing.

        if (plan_assignment_request_.has_plan())
        {
            auto plan_assignment_response = cgfHelper.assignPlan(plan_assignment_request_);

            plan_assignment_responder_.Finish(plan_assignment_response, grpc::Status::OK, this);
        }
        else // assume it was the other request
        {
            auto list_sim_objects_response = cgfHelper.getSimObjects();

            // And we are done! Let the gRPC runtime know we've finished, using the
            // memory address of this instance as the uniquely identifying tag for
            // the event.
            list_sim_objects_responder_.Finish(list_sim_objects_response, grpc::Status::OK, this);
        }

        status_ = FINISH;
    }
    else {
        GPR_ASSERT(status_ == FINISH);
        // Once in the FINISH state, deallocate ourselves (CallData).
        delete this;
    }
}
