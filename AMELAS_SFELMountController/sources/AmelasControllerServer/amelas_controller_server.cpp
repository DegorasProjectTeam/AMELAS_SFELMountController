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
 * @file amelas_controller_server.cpp
 * @brief This file contains the implementation of the AmelasControllerServer example class.
 * @author Degoras Project Team
 * @author AVS AMELAS Team
 * @copyright EUPL License
 * @version 2309.5
***********************************************************************************************************************/

// PROJECT INCLUDES
// =====================================================================================================================
#include "AmelasControllerServer/amelas_controller_server.h"
// =====================================================================================================================

// AMELAS NAMESPACES
// =====================================================================================================================
namespace amelas{
namespace communication{
// =====================================================================================================================

// ---------------------------------------------------------------------------------------------------------------------
using zmqutils::common::ServerCommand;
using zmqutils::common::OperationResult;
using zmqutils::common::ResultType;
using zmqutils::utils::BinarySerializer;
// ---------------------------------------------------------------------------------------------------------------------

AmelasControllerServer::AmelasControllerServer(const std::shared_ptr<spdlog::logger> logger, unsigned int port, 
                                                const std::string &local_addr) :
    ClbkCommandServerBase(port, local_addr),
    _logger(logger)
{
    // Register each internal specific process function in the base server.

    // REQ_SET_HOME_POSITION
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_HOME_POSITION,
                                  &AmelasControllerServer::processSetPosition<controller::SetHomePositionCallback>);

    // REQ_GET_HOME_POSITION.
    this->registerRequestProcFunc(AmelasServerCommand::REQ_GET_HOME_POSITION,
                                  &AmelasControllerServer::processGetPosition<controller::GetHomePositionCallback>);

    // REQ_SET_IDLE_POS.
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_IDLE_POS,
                                  &AmelasControllerServer::processSetPosition<controller::SetIdlePositionCallback>);

    // REQ_GET_IDLE_POS.
    this->registerRequestProcFunc(AmelasServerCommand::REQ_GET_IDLE_POS,
                                  &AmelasControllerServer::processGetPosition<controller::GetIdlePositionCallback>);

    // REQ_SET_PARK_POS.
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_PARK_POS,
                                  &AmelasControllerServer::processSetPosition<controller::SetParkPositionCallback>);

    // REQ_GET_PARK_POS.
    this->registerRequestProcFunc(AmelasServerCommand::REQ_GET_PARK_POS,
                                  &AmelasControllerServer::processGetPosition<controller::GetParkPositionCallback>);

    // REQ_SET_CALIBRATION_POS.
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_CALIBRATION_POS,
                                  &AmelasControllerServer::processSetPosition<controller::SetCalibrationPositionCallback>);

    // REQ_GET_CALIBRATION_POS.
    this->registerRequestProcFunc(AmelasServerCommand::REQ_GET_CALIBRATION_POS,
                                  &AmelasControllerServer::processGetPosition<controller::GetCalibrationPositionCallback>);

    // REQ_SET_HOMING_OFFSETS.
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_HOMING_OFFSETS,
                                  &AmelasControllerServer::processSetPosition<controller::SetHomingOffsetsCallback>);

    // REQ_GET_HOMING_OFFSETS.
    this->registerRequestProcFunc(AmelasServerCommand::REQ_GET_HOMING_OFFSETS,
                                  &AmelasControllerServer::processGetPosition<controller::GetHomingOffsetsCallback>);

    // REQ_SET_SLEW_SPEED.
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_SLEW_SPEED,
                                  &AmelasControllerServer::processSetPosition<controller::SetSlewSpeedCallback>);

    // REQ_GET_SLEW_SPEED.
    this->registerRequestProcFunc(AmelasServerCommand::REQ_GET_SLEW_SPEED,
                                  &AmelasControllerServer::processGetPosition<controller::GetSlewSpeedCallback>);

    // REQ_SET_TRACK_POS_OFFSET.
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_TRACK_POS_OFFSET,
                                  &AmelasControllerServer::processSetPosition<controller::SetTrackPosOffsetCallback>);

    // REQ_GET_TRACK_POS_OFFSET.
    this->registerRequestProcFunc(AmelasServerCommand::REQ_GET_TRACK_POS_OFFSET,
                                  &AmelasControllerServer::processGetPosition<controller::GetTrackPosOffsetCallback>);

    // REQ_SET_ABS_ALTAZ_MOTION.
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_ABS_ALTAZ_MOTION,
                                   &AmelasControllerServer::processSetAbsoluteAltAzMotion);

    // REQ_DO_CONNECT_PLC.
    this->registerRequestProcFunc(AmelasServerCommand::REQ_DO_CONNECT_PLC,
                                  &AmelasControllerServer::processGetPosition<controller::DoConnectPLC>);
}

AmelasControllerServer::~AmelasControllerServer() {}

// void AmelasControllerServer::processSetHomePosition(const CommandRequest& request, CommandReply& reply)
// {
//     // Auxiliar variables and containers.
//     controller::AmelasError ctrl_err;
// 
//     // Position struct.
//     controller::AltAzPos pos;
// 
//     // Check the request parameters size.
//     if (request.params_size == 0 || !request.params)
//     {
//         reply.server_result = OperationResult::EMPTY_PARAMS;
//         return;
//     }
// 
//     // Try to read the parameters data.
//     try
//     {
//         BinarySerializer::fastDeserialization(request.params.get(), request.params_size, pos);
//     }
//     catch(...)
//     {
//         reply.server_result = OperationResult::BAD_PARAMETERS;
//         return;
//     }
// 
//     // Now we will process the command in the controller.
//     ctrl_err = this->invokeCallback<controller::SetHomePositionCallback>(request, reply, pos);
// 
//     // Serialize parameters if all ok.
//     if(reply.server_result == OperationResult::COMMAND_OK)
//         reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err);
// }

void AmelasControllerServer::processSetAbsoluteAltAzMotion(const CommandRequest &request, CommandReply &reply)
{
    // Auxiliar variables and containers.
    controller::AmelasError ctrl_err;

    // Position struct.
    controller::AltAzPos pos;
    controller::AltAzVel vel;

    // Check the request parameters size.
    if (request.params_size == 0 || !request.params)
    {
        reply.server_result = OperationResult::EMPTY_PARAMS;
        return;
    }

    // Try to read the parameters data.
    try
    {
        BinarySerializer::fastDeserialization(request.params.get(), request.params_size, pos, vel);
    }
    catch(...)
    {
        reply.server_result = OperationResult::BAD_PARAMETERS;
        return;
    }

    // Now we will process the command in the controller.
    ctrl_err = this->invokeCallback<controller::SetAbsoluteAltAzMotionCallback>(request, reply, pos, vel);

    // Serialize parameters if all ok.
    if(reply.server_result == OperationResult::COMMAND_OK)
        reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err);
}

void AmelasControllerServer::registerRequestProcFunc(AmelasServerCommand command, AmelasRequestProcFunc func)
{
    CommandServerBase::registerRequestProcFunc(static_cast<ServerCommand>(command), this, func);
}

bool AmelasControllerServer::validateCustomCommand(ServerCommand command)
{
    // Auxiliar variables.
    bool result = false;
    zmqutils::common::CommandType cmd = static_cast<zmqutils::common::CommandType>(command);
    // Check if the command is within the range of implemented custom commands.
    if (cmd >= common::kMinCmdId && cmd <= common::kMaxCmdId)
        result = true;
    return result;
}

void AmelasControllerServer::onCustomCommandReceived(CommandRequest& request, CommandReply& reply)
{
    // Get the command string.
    std::string cmd_str;
    std::uint32_t cmd_uint = static_cast<std::uint32_t>(request.command);
    cmd_str = (cmd_uint < AmelasServerCommandStr.size()) ? AmelasServerCommandStr[cmd_uint] : "Unknown command";

    // Log the command.
    std::cout << std::string(100, '-') << std::endl;
    std::cout<<"ON CUSTOM COMMAND RECEIVED: "<<std::endl;
    std::cout<<"Time: "<<zmqutils::utils::currentISO8601Date()<<std::endl;
    std::cout<<"Client UUID: "<<request.client_uuid.toRFC4122String()<<std::endl;
    std::cout<<"Command: "<<cmd_uint<<" ("<<cmd_str<<")"<<std::endl;
    std::cout << std::string(100, '-') << std::endl;

    // Call to the base function for process the custom command with the registered process functions.
    CommandServerBase::onCustomCommandReceived(request, reply);
}

void AmelasControllerServer::onServerStart()
{
    // Ips.
    std::string ips;

    // Get listen interfaces ips.
    for(const auto& intrfc : this->getServerAddresses())
    {
        ips.append(intrfc.ip);
        ips.append(" - ");
    }
    ips.pop_back();
    ips.pop_back();

    // Log.
    std::cout << std::string(100, '-') << std::endl;
    std::cout<<"<AMELAS SERVER>"<<std::endl;
    std::cout<<"-> ON SERVER START: "<<std::endl;
    std::cout<<"Time: "<<zmqutils::utils::currentISO8601Date()<<std::endl;
    std::cout<<"Addresses: "<<ips<<std::endl;
    std::cout<<"Port: "<<this->getServerPort()<<std::endl;
    std::cout << std::string(100, '-') << std::endl;
}

void AmelasControllerServer::onServerStop()
{
    // Log.
    std::cout << std::string(100, '-') << std::endl;
    std::cout<<"<AMELAS SERVER>"<<std::endl;
    std::cout<<"-> ON SERVER CLOSE: "<<std::endl;
    std::cout<<"Time: "<<zmqutils::utils::currentISO8601Date()<<std::endl;
    std::cout << std::string(100, '-') << std::endl;
}

void AmelasControllerServer::onWaitingCommand()
{
    // Log.
    std::cout << std::string(100, '-') << std::endl;
    std::cout<<"<AMELAS SERVER>"<<std::endl;
    std::cout<<"-> ON WAITING COMMAND: "<<std::endl;
    std::cout<<"Time: "<<zmqutils::utils::currentISO8601Date()<<std::endl;
    std::cout << std::string(100, '-') << std::endl;
}

void AmelasControllerServer::onDeadClient(const HostInfo& client)
{
    // Log.
    std::cout << std::string(100, '-') << std::endl;
    std::cout<<"<AMELAS SERVER>"<<std::endl;
    std::cout<<"-> ON DEAD CLIENT: "<<std::endl;
    std::cout<<"Time: "<<zmqutils::utils::currentISO8601Date()<<std::endl;
    std::cout<<"Current Clients: "<<this->getConnectedClients().size()<<std::endl;
    std::cout<<"Client UUID: "<<client.uuid.toRFC4122String()<<std::endl;
    std::cout<<"Client Ip: "<<client.ip<<std::endl;
    std::cout<<"Client Host: "<<client.hostname<<std::endl;
    std::cout<<"Client Process: "<<client.pid<<std::endl;
    std::cout << std::string(100, '-') << std::endl;
}

void AmelasControllerServer::onConnected(const HostInfo& client)
{
    // Log.
    std::cout << std::string(100, '-') << std::endl;
    std::cout<<"<AMELAS SERVER>"<<std::endl;
    std::cout<<"-> ON CONNECTED: "<<std::endl;
    std::cout<<"Time: "<<zmqutils::utils::currentISO8601Date()<<std::endl;
    std::cout<<"Current Clients: "<<this->getConnectedClients().size()<<std::endl;
    std::cout<<"Client UUID: "<<client.uuid.toRFC4122String()<<std::endl;
    std::cout<<"Client Name: "<<client.name<<std::endl;
    std::cout<<"Client Ip: "<<client.ip<<std::endl;
    std::cout<<"Client Host: "<<client.hostname<<std::endl;
    std::cout<<"Client Process: "<<client.pid<<std::endl;
    std::cout << std::string(100, '-') << std::endl;
}

void AmelasControllerServer::onDisconnected(const HostInfo& client)
{
    // Log.
    std::cout << std::string(100, '-') << std::endl;
    std::cout<<"<AMELAS SERVER>"<<std::endl;
    std::cout<<"-> ON DISCONNECTED: "<<std::endl;
    std::cout<<"Time: "<<zmqutils::utils::currentISO8601Date()<<std::endl;
    std::cout<<"Current Clients: "<<this->getConnectedClients().size()<<std::endl;
    std::cout<<"Client UUID: "<<client.uuid.toRFC4122String()<<std::endl;
    std::cout<<"Client Name: "<<client.name<<std::endl;
    std::cout<<"Client Ip: "<<client.ip<<std::endl;
    std::cout<<"Client Host: "<<client.hostname<<std::endl;
    std::cout<<"Client Process: "<<client.pid<<std::endl;
    std::cout << std::string(100, '-') << std::endl;
}

void AmelasControllerServer::onServerError(const zmq::error_t &error, const std::string &ext_info)
{
    // Log.
    std::cout << std::string(100, '-') << std::endl;
    std::cout<<"<AMELAS SERVER>"<<std::endl;
    std::cout<<"-> ON SERVER ERROR: "<<std::endl;
    std::cout<<"Time: "<<zmqutils::utils::currentISO8601Date()<<std::endl;
    std::cout<<"Code: "<<error.num()<<std::endl;
    std::cout<<"Error: "<<error.what()<<std::endl;
    std::cout<<"Info: "<<ext_info<<std::endl;
    std::cout << std::string(100, '-') << std::endl;
}

void AmelasControllerServer::onCommandReceived(const CommandRequest &request)
{
    // Get the command string.
    std::string cmd_str;
    std::uint32_t command = static_cast<std::uint32_t>(request.command);
    cmd_str = (command < AmelasServerCommandStr.size()) ? AmelasServerCommandStr[command] : "Unknown command";
    // Log.
    BinarySerializer serializer(request.params.get(), request.params_size);
    std::cout << std::string(100, '-') << std::endl;
    std::cout<<"<AMELAS SERVER>"<<std::endl;
    std::cout<<"-> ON COMMAND RECEIVED: "<<std::endl;
    std::cout<<"Time: "<<zmqutils::utils::currentISO8601Date()<<std::endl;
    std::cout<<"Client UUID: "<<request.client_uuid.toRFC4122String()<<std::endl;
    std::cout<<"Command: "<<command<<" ("<<cmd_str<<")"<<std::endl;
    std::cout<<"Params Size: "<<request.params_size<<std::endl;
    std::cout<<"Params Hex: "<<serializer.getDataHexString()<<std::endl;
    std::cout << std::string(100, '-') << std::endl;
}

void AmelasControllerServer::onInvalidMsgReceived(const CommandRequest &request)
{
    // Log.
    BinarySerializer serializer(request.params.get(), request.params_size);
    std::cout << std::string(100, '-') << std::endl;
    std::cout<<"<AMELAS SERVER>"<<std::endl;
    std::cout<<"-> ON BAD COMMAND RECEIVED: "<<std::endl;
    std::cout<<"Time: "<<zmqutils::utils::currentISO8601Date()<<std::endl;
    std::cout<<"Client UUID: "<<request.client_uuid.toRFC4122String()<<std::endl;
    std::cout<<"Command: "<<static_cast<int>(request.command)<<std::endl;
    std::cout<<"Params Size: "<<request.params_size<<std::endl;
    std::cout<<"Params Hex: "<<serializer.getDataHexString()<<std::endl;
    std::cout << std::string(100, '-') << std::endl;
}

void AmelasControllerServer::onSendingResponse(const CommandReply &reply)
{
    // Log.
    BinarySerializer serializer(reply.params.get(), reply.params_size);
    size_t result = static_cast<size_t>(reply.server_result);
    std::cout << std::string(100, '-') << std::endl;
    std::cout<<"<AMELAS SERVER>"<<std::endl;
    std::cout<<"-> ON SENDING RESPONSE: "<<std::endl;
    std::cout<<"Time: "<<zmqutils::utils::currentISO8601Date()<<std::endl;
    std::cout<<"Result: "<<result<<" ("<<AmelasServerResultStr[result]<<")"<<std::endl;
    std::cout<<"Params Size: "<<reply.params_size<<std::endl;
    std::cout<<"Params Hex: "<<serializer.getDataHexString()<<std::endl;
    std::cout << std::string(100, '-') << std::endl;
}

}} // END NAMESPACES.
// =====================================================================================================================

