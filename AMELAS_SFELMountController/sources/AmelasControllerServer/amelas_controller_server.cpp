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
    // -- SAFETY RELATED FUNCTIONS
    this->registerRequestProcFunc(AmelasServerCommand::REQ_DO_RESET_STATE,
                                  &AmelasControllerServer::processEmptyArguments<controller::DoResetStateCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_EN_AVOID_SUN,
                                  &AmelasControllerServer::processSetBool<controller::EnableAvoidSunCallback>);

    // -- LOW LEVEL PLC REGISTERS RELATED FUNCTIONS
    this->registerRequestProcFunc(AmelasServerCommand::REQ_DO_CONNECT_PLC,
                                  &AmelasControllerServer::processEmptyArguments<controller::DoConnectPLCCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_DO_DISCONNECT_PLC,
                                  &AmelasControllerServer::processEmptyArguments<controller::DoDisconnectPLCCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_GET_PLC_REGISTERS,
                                  &AmelasControllerServer::processGetPLCRegister<controller::GetPLCRegisterCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_GET_PLC_PRUEBA,
                                  &AmelasControllerServer::processGetPLCprueba<controller::GetPLCpruebaCallback>); // PRUEBA

    // -- STATUS AND CONFIGURATION RELATED FUNCTIONS
    this->registerRequestProcFunc(AmelasServerCommand::REQ_GET_MOUNT_LOG,
                                  &AmelasControllerServer::processSetString<controller::GetMountLogCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_DO_SYNC_NTP,
                                  &AmelasControllerServer::processSetNTPserver<controller::DoSyncTimeNTPCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_GET_MOUNT_STATUS,
                                  &AmelasControllerServer::processGetString<controller::GetMountStatusCallback>); // PRUEBA
    this->registerRequestProcFunc(AmelasServerCommand::REQ_GET_MOUNT_INFO,
                                  &AmelasControllerServer::processEmptyArguments<controller::GetDeviceInfoCallback>); // PRUEBA
    this->registerRequestProcFunc(AmelasServerCommand::REQ_EN_TRACK_ADJ,
                                  &AmelasControllerServer::processSetBool<controller::EnableTrackingAdjustsCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_EN_MOUNT_POWER,
                                  &AmelasControllerServer::processSetBool<controller::EnableMountPowerCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_SLEW_SPEED,
                                  &AmelasControllerServer::processSetPositionOrSpeed<controller::SetSlewSpeedCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_GET_SLEW_SPEED,
                                  &AmelasControllerServer::processGetPositionOrSpeed<controller::GetSlewSpeedCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_HOME_POSITION,
                                  &AmelasControllerServer::processSetPositionOrSpeed<controller::SetHomePositionCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_GET_HOME_POSITION,
                                  &AmelasControllerServer::processGetPositionOrSpeed<controller::GetHomePositionCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_IDLE_POS,
                                  &AmelasControllerServer::processSetPositionOrSpeed<controller::SetIdlePositionCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_GET_IDLE_POS,
                                  &AmelasControllerServer::processGetPositionOrSpeed<controller::GetIdlePositionCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_PARK_POS,
                                  &AmelasControllerServer::processSetPositionOrSpeed<controller::SetParkPositionCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_GET_PARK_POS,
                                  &AmelasControllerServer::processGetPositionOrSpeed<controller::GetParkPositionCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_CALIBRATION_POS,
                                  &AmelasControllerServer::processSetPositionOrSpeed<controller::SetCalibrationPositionCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_GET_CALIBRATION_POS,
                                  &AmelasControllerServer::processGetPositionOrSpeed<controller::GetCalibrationPositionCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_IDLE_POS_HERE,
                                  &AmelasControllerServer::processEmptyArguments<controller::SetIdlePositionHereCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_PARK_POS_HERE,
                                  &AmelasControllerServer::processEmptyArguments<controller::SetParkPositionHereCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_CALIBRATION_POS_HERE,
                                  &AmelasControllerServer::processEmptyArguments<controller::SetCalibrationPositionHereCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_WAIT_ALT,
                                  &AmelasControllerServer::processSetDouble<controller::SetWaitAltCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_GET_WAIT_ALT,
                                  &AmelasControllerServer::processGetDouble<controller::GetWaitAltCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_HOMING_OFFSETS,
                                  &AmelasControllerServer::processSetPositionOrSpeed<controller::SetHomingOffsetsCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_GET_HOMING_OFFSETS,
                                  &AmelasControllerServer::processGetPositionOrSpeed<controller::GetHomingOffsetsCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_EN_MOUNT_MODEL,
                                  &AmelasControllerServer::processSetBool<controller::EnableMountModelCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_MOUNT_MODEL_COEFS,
                                  &AmelasControllerServer::processSetLocation<controller::SetMountModelCoefsCallback>); // PRUEBA
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_MOUNT_MODEL_COEFS_FILE,
                                  &AmelasControllerServer::processSetString<controller::SetMountModelCoefsFileCallback>); // PRUEBA
    this->registerRequestProcFunc(AmelasServerCommand::REQ_GET_MOUNT_MODEL_COEFS,
                                  &AmelasControllerServer::processGetSixDoubles<controller::GetMountModelCoefsCallback>); // PRUEBA
    this->registerRequestProcFunc(AmelasServerCommand::REQ_APP_MOUNT_MODEL_CORRECT,
                                  &AmelasControllerServer::processSetSixBool<controller::ApplyMountModelCorrectionsCallback>); // PRUEBA
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_LOCATION,
                                  &AmelasControllerServer::processSetLocation<controller::SetLocationCallback>); // PRUEBA
    this->registerRequestProcFunc(AmelasServerCommand::REQ_GET_LOCATION,
                                  &AmelasControllerServer::processGetLocation<controller::GetLocationCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_METEO_DATA,
                                  &AmelasControllerServer::processSetMeteoData<controller::SetMeteoDataCallback>); // PRUEBA
    this->registerRequestProcFunc(AmelasServerCommand::REQ_GET_METEO_DATA,
                                  &AmelasControllerServer::processGetMeteoData<controller::GetMeteoDataCallback>);

    // -- MOTION RELATED FUNCTIONS
    this->registerRequestProcFunc(AmelasServerCommand::REQ_GET_MOTION_MODE,
                                  &AmelasControllerServer::processGetMotionMode<controller::GetMotionModeCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_GET_MOTION_STATE,
                                  &AmelasControllerServer::processGetMotionState<controller::GetMotionStateCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_DO_START_MOTION,
                                  &AmelasControllerServer::processEmptyArguments<controller::SetDoStartMotionCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_DO_PAUSE_MOTION,
                                  &AmelasControllerServer::processEmptyArguments<controller::SetDoPauseMotionCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_DO_STOP_MOTION,
                                  &AmelasControllerServer::processEmptyArguments<controller::SetDoStopMotionCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_TRACK_POS_OFFSET,
                                  &AmelasControllerServer::processSetPositionOrSpeed<controller::SetTrackPosOffsetCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_GET_TRACK_POS_OFFSET,
                                  &AmelasControllerServer::processGetPositionOrSpeed<controller::GetTrackPosOffsetCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_TRACK_TIME_BIAS,
                                  &AmelasControllerServer::processSetDouble<controller::SetTrackTimeBiasCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_GET_TRACK_TIME_BIAS,
                                  &AmelasControllerServer::processGetDouble<controller::GetTrackTimeBiasCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_ABS_ALTAZ_MOTION,
                                   &AmelasControllerServer::processSetAbsRelAltAzMotion<controller::SetAbsoluteAltAzMotionCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_REL_ALTAZ_MOTION,
                                   &AmelasControllerServer::processSetAbsRelAltAzMotion<controller::SetRelativeAltAzMotionCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_CON_ALTAZ_MOTION,
                                   &AmelasControllerServer::processSetPositionOrSpeed<controller::SetContAltAzMotionCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_IDLE_MOTION,
                                  &AmelasControllerServer::processEmptyArguments<controller::SetIdleMotionCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_PARK_MOTION,
                                  &AmelasControllerServer::processEmptyArguments<controller::SetParkMotionCallback>);
    this->registerRequestProcFunc(AmelasServerCommand::REQ_SET_CALIBRATION_MOTION,
                                  &AmelasControllerServer::processEmptyArguments<controller::SetCalibrationMotionCallback>);

    // -- OTHER FUNCTIONS
    this->registerRequestProcFunc(AmelasServerCommand::REQ_DO_PRUEBA_BUCLES,
                                  &AmelasControllerServer::processEmptyArguments<controller::DoPruebaBucles>);
}

AmelasControllerServer::~AmelasControllerServer() {}

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

