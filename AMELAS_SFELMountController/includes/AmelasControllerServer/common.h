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
 * @file common.h
 * @brief This file contains common elements for the AmelasControllerServer module.
 * @author Degoras Project Team
 * @author AVS AMELAS Team
 * @copyright EUPL License
 * @version 2310.1
***********************************************************************************************************************/

// =====================================================================================================================
#pragma once
// =====================================================================================================================

// ZMQUTILS INCLUDES
// =====================================================================================================================
#include <LibZMQUtils/CommandServer>
// =====================================================================================================================

// AMELAS NAMESPACES
// =====================================================================================================================
namespace amelas{
namespace communication{
namespace common{
// =====================================================================================================================

// Specific subclass commands (0 to 20 are reserved for the base server).
// WARNING: In our approach, the server commands must be always in order.
enum class AmelasServerCommand : zmqutils::common::CommandType
{
    REQ_DO_RESET_STATE           = 33,
    REQ_EN_AVOID_SUN             = 34,
    REQ_GET_PLC_REGISTERS        = 35,
    REQ_GET_MOUNT_LOG            = 36,
    REQ_DO_SYNC_NTP              = 37,
    REQ_DO_SYNC_MANUAL           = 38,
    REQ_GET_MOUNT_STATUS         = 39,
    REQ_GET_MOUNT_INFO           = 40,
    REQ_EN_TRACK_ADJ             = 41,
    REQ_EN_MOUNT_POWER           = 42,
    REQ_SET_SLEW_SPEED           = 43,
    REQ_GET_SLEW_SPEED           = 44,
    REQ_SET_HOME_POSITION        = 45,
    REQ_GET_HOME_POSITION        = 46,
    REQ_SET_IDLE_POS             = 47,
    REQ_GET_IDLE_POS             = 48,
    REQ_SET_PARK_POS             = 49,
    REQ_GET_PARK_POS             = 50,
    REQ_SET_CALIBRATION_POS      = 51,
    REQ_GET_CALIBRATION_POS      = 52,
    REQ_SET_IDLE_POS_HERE        = 53,
    REQ_SET_PARK_POS_HERE        = 54,
    REQ_SET_CALIBRATION_POS_HERE = 55,
    REQ_SET_WAIT_ALT             = 56,
    REQ_GET_WAIT_ALT             = 57,
    REQ_SET_HOMING_OFFSETS       = 58,
    REQ_GET_HOMING_OFFSETS       = 59,
    REQ_EN_MOUNT_MODEL           = 60,
    REQ_SET_MOUNT_MODEL_COEFS    = 61,
    REQ_GET_MOUNT_MODEL_COEFS    = 62,
    REQ_SET_LOCATION             = 63,
    REQ_GET_LOCATION             = 64,
    REQ_SET_METEO_DATA           = 65,
    REQ_GET_METEO_DATA           = 66,
    REQ_EN_SIMULATION_MODE       = 67,
    REQ_GET_SIMULATION_STATE     = 68,
    REQ_SET_SIMULATION_TIME      = 69,
    REQ_GET_MOTION_MODE          = 70,
    REQ_GET_MOTION_STATE         = 71,
    REQ_DO_START_MOTION          = 72,
    REQ_DO_PAUSE_MOTION          = 73,
    REQ_DO_STOP_MOTION           = 74,
    REQ_SET_TRACK_POS_OFFSET     = 75,
    REQ_GET_TRACK_POS_OFFSET     = 76,
    REQ_SET_TRACK_TIME_BIAS      = 77,
    REQ_GET_TRACK_TIME_BIAS      = 78,
    REQ_SET_ABS_ALTAZ_MOTION     = 79,
    REQ_SET_REL_ALTAZ_MOTION     = 80,
    REQ_SET_CON_ALTAZ_MOTION     = 81,
    REQ_SET_HOMING_MOTION        = 82,
    REQ_SET_IDLE_MOTION          = 83,
    REQ_SET_PARK_MOTION          = 84,
    REQ_SET_CALIBRATION_MOTION   = 85,
    REQ_SET_CPF_MOTION           = 86,
    REQ_SET_STAR_MOTION          = 87,
    REQ_DO_CONNECT_PLC           = 88,
    REQ_DO_DISCONNECT_PLC        = 89,
    REQ_DO_PRUEBA_BUCLES         = 100,
    REQ_GET_PLC_PRUEBA           = 101,
    REQ_APP_MOUNT_MODEL_CORRECT  = 102,
    REQ_SET_MOUNT_MODEL_COEFS_FILE = 103,
    END_IMPL_COMMANDS            = 90,
    END_AMELAS_COMMANDS          = 91
};

// Specific subclass errors (0 to 30 are reserved for the base server).
enum class AmelasServerResult : zmqutils::common::ResultType
{
    EMPTY_CALLBACK   = 31,
    INVALID_CALLBACK = 32
};

// Extend the base command strings with those of the subclass.
static constexpr auto AmelasServerCommandStr = zmqutils::utils::joinArraysConstexpr(
    zmqutils::common::ServerCommandStr,
    std::array<const char*, 60>
    {
        "FUTURE_EXAMPLE",
        "FUTURE_EXAMPLE",
        "REQ_DO_RESET_STATE",
        "REQ_EN_AVOID_SUN",
        "REQ_GET_PLC_REGISTERS",
        "REQ_GET_MOUNT_LOG",
        "REQ_DO_SYNC_NTP",
        "REQ_DO_SYNC_MANUAL",
        "REQ_GET_MOUNT_STATUS",
        "REQ_GET_MOUNT_INFO",
        "REQ_EN_TRACK_ADJ",
        "REQ_EN_MOUNT_POWER",
        "REQ_SET_SLEW_SPEED",
        "REQ_GET_SLEW_SPEED",
        "REQ_SET_HOME_POSITION",
        "REQ_GET_HOME_POSITION",
        "REQ_SET_IDLE_POS",
        "REQ_GET_IDLE_POS",
        "REQ_SET_PARK_POS",
        "REQ_GET_PARK_POS",
        "REQ_SET_CALIBRATION_POS",
        "REQ_GET_CALIBRATION_POS",
        "REQ_SET_IDLE_POS_HERE",
        "REQ_SET_PARK_POS_HERE",
        "REQ_SET_CALIBRATION_POS_HERE",
        "REQ_SET_WAIT_ALT",
        "REQ_GET_WAIT_ALT",
        "REQ_SET_HOMING_OFFSETS",
        "REQ_GET_HOMING_OFFSETS",
        "REQ_EN_MOUNT_MODEL",
        "REQ_SET_MOUNT_MODEL_COEFS",
        "REQ_GET_MOUNT_MODEL_COEFS",
        "REQ_SET_LOCATION",
        "REQ_GET_LOCATION",
        "REQ_SET_METEO_DATA",
        "REQ_GET_METEO_DATA",
        "REQ_EN_SIMULATION_MODE",
        "REQ_GET_SIMULATION_STATE",
        "REQ_SET_SIMULATION_TIME",
        "REQ_GET_MOTION_MODE",
        "REQ_GET_MOTION_STATE",
        "REQ_DO_START_MOTION",
        "REQ_DO_PAUSE_MOTION",
        "REQ_DO_STOP_MOTION",
        "REQ_SET_TRACK_POS_OFFSET",
        "REQ_GET_TRACK_POS_OFFSET",
        "REQ_SET_TRACK_TIME_BIAS",
        "REQ_GET_TRACK_TIME_BIAS",
        "REQ_SET_ABS_ALTAZ_MOTION",
        "REQ_SET_REL_ALTAZ_MOTION",
        "REQ_SET_CON_ALTAZ_MOTION",
        "REQ_SET_HOMING_MOTION",
        "REQ_SET_IDLE_MOTION",
        "REQ_SET_PARK_MOTION",
        "REQ_SET_CALIBRATION_MOTION",
        "REQ_SET_CPF_MOTION",
        "REQ_SET_STAR_MOTION",
        "REQ_DO_CONNECT_PLC",
        "REQ_DO_DISCONNECT_PLC",
        "END_DRGG_COMMANDS"
    });

// Extend the base result strings with those of the subclass.
static constexpr auto AmelasServerResultStr = zmqutils::utils::joinArraysConstexpr(
    zmqutils::common::OperationResultStr,
    std::array<const char*, 2>
    {
        "EMPTY_CALLBACK - The external callback for the command is empty.",
        "INVALID_CALLBACK - The external callback for the command is invalid."
    });

// Usefull const expressions.
constexpr int kMinCmdId = static_cast<int>(zmqutils::common::ServerCommand::END_BASE_COMMANDS) + 1;
constexpr int kMaxCmdId = static_cast<int>(AmelasServerCommand::END_AMELAS_COMMANDS) - 1;

}}} // END NAMESPACES.
// =====================================================================================================================
