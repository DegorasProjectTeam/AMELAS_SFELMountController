/***********************************************************************************************************************
 *   AMELAS_SFELMountController: [...].
 *
 *   Copyright (C) 2023 ROA Team (Royal Institute and Observatory of the Spanish Navy)
 *                      < Ángel Vera Herrera, avera@roa.es - angeldelaveracruz@gmail.com >
 *                      < Jesús Relinque Madroñal >
 *                      AVS AMELAS Team
 *                      <>
 *
 *   This file is part of AMELAS_SFELMountController.
 *
 *   Licensed under [...]
 **********************************************************************************************************************/

/** ********************************************************************************************************************
 * @file amelas_controller_server.h
 * @brief This file contains the declaration of the AmelasControllerServer class.
 * @author Degoras Project Team
 * @author AVS AMELAS Team
 * @copyright EUPL License
 * @version 2309.5
***********************************************************************************************************************/

// =====================================================================================================================
#pragma once
// =====================================================================================================================

// C++ INCLUDES
// =====================================================================================================================
#include <unordered_map>
#include <string>
#include <any>
#include <variant>
// =====================================================================================================================

// ZMQUTILS INCLUDES
// =====================================================================================================================
#include <LibZMQUtils/CallbackCommandServer>
#include <LibZMQUtils/Utils>
// =====================================================================================================================

// PROJECT INCLUDES
// =====================================================================================================================
#include "AmelasController/amelas_controller.h"
#include "AmelasController/common.h"
#include "AmelasControllerServer/common.h"
#include "libamelas_global.h"
// =====================================================================================================================

// AMELAS NAMESPACES
// =====================================================================================================================
namespace amelas{
namespace communication{
// =====================================================================================================================

// ---------------------------------------------------------------------------------------------------------------------
using namespace amelas::communication::common;
using zmqutils::common::CommandReply;
using zmqutils::common::CommandRequest;
using zmqutils::common::OperationResult;
using zmqutils::common::ServerCommand;
using zmqutils::common::HostInfo;
using zmqutils::utils::CallbackHandler;
// ---------------------------------------------------------------------------------------------------------------------

// Example of creating a command server from the base.
class AmelasControllerServer final : public zmqutils::ClbkCommandServerBase
{
public:

    LIBAMELAS_EXPORT AmelasControllerServer(unsigned port, const std::string& local_addr = "*");

    // Register callback function helper.
    template<typename... Args>
    void registerControllerCallback(AmelasServerCommand command,
                                    controller::AmelasController* object,
                                    controller::AmelasControllerCallback<Args...> callback)
    {
        CallbackHandler::registerCallback(static_cast<CallbackHandler::CallbackId>(command), object, callback);
    }

    LIBAMELAS_EXPORT ~AmelasControllerServer() final;

private:

    // -----------------------------------------------------------------------------------------------------------------
    using CommandServerBase::registerRequestProcFunc;
    using CallbackHandler::registerCallback;
    using AmelasRequestProcFunc = void(AmelasControllerServer::*)(const CommandRequest&, CommandReply&);
    // -----------------------------------------------------------------------------------------------------------------

    // Process functions for all the specific commands.
    void processSetHomePosition(const CommandRequest&, CommandReply&);
    void processGetHomePosition(const CommandRequest&, CommandReply&);

    // Subclass register process function helper.
    void registerRequestProcFunc(AmelasServerCommand command, AmelasRequestProcFunc func);

    // Subclass invoke callback helper.
    template <typename ClbkT, typename... Args>
    controller::AmelasError invokeCallback(const CommandRequest& request, CommandReply& reply, Args&&... args)
    {
        return ClbkCommandServerBase::invokeCallback<ClbkT>(request, reply,
                                                            controller::AmelasError::INVALID_ERROR,
                                                            std::forward<Args>(args)...);
    }

    // Internal overrided command validation function.
    virtual bool validateCustomCommand(ServerCommand command) final;

    // Internal overrided custom command received callback.
    virtual void onCustomCommandReceived(CommandRequest&, CommandReply&) final;

    // Internal overrided start callback.
    virtual void onServerStart() final;

    // Internal overrided close callback.
    virtual void onServerStop() final;

    // Internal waiting command callback.
    virtual void onWaitingCommand() final;

    // Internal dead client callback.
    virtual void onDeadClient(const HostInfo&) final;

    // Internal overrided connect callback.
    virtual void onConnected(const HostInfo&) final;

    // Internal overrided disconnect callback.
    virtual void onDisconnected(const HostInfo&) final;

    // Internal overrided command received callback.
    virtual void onCommandReceived(const CommandRequest&) final;

    // Internal overrided bad command received callback.
    virtual void onInvalidMsgReceived(const CommandRequest&) final;

    // Internal overrided sending response callback.
    virtual void onSendingResponse(const CommandReply&) final;

    // Internal overrided server error callback.
    virtual void onServerError(const zmq::error_t&, const std::string& ext_info) final;
};

}} // END NAMESPACES.
// =====================================================================================================================
