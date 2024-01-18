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
    REQ_SET_HOME_POSITION        = 33,
    REQ_GET_HOME_POSITION        = 34,
    REQ_SET_IDLE_POS             = 36,
    REQ_GET_IDLE_POS             = 37,
    REQ_SET_PARK_POS             = 38,
    REQ_GET_PARK_POS             = 39,
    REQ_SET_CALIBRATION_POS      = 40,
    REQ_GET_CALIBRATION_POS      = 41,
    REQ_SET_IDLE_POS_HERE        = 42,
    REQ_SET_PARK_POS_HERE        = 43,
    REQ_SET_CALIBRATION_POS_HERE = 44,
    REQ_SET_HOMING_OFFSETS       = 45,
    REQ_GET_HOMING_OFFSETS       = 46,
    REQ_SET_WAIT_ALT             = 47,
    REQ_GET_WAIT_ALT             = 48,
    REQ_SET_SLEW_SPEED           = 49,
    REQ_GET_SLEW_SPEED           = 50,
    REQ_EN_TRACK_ADJ             = 51,
    REQ_EN_MOUNT_POWER           = 52,
    REQ_EN_MOUNT_MODEL           = 53,
    REQ_SET_METEO_DATA           = 54,
    REQ_GET_METEO_DATA           = 55,
    REQ_DO_START_MOTION          = 56,
    REQ_DO_PAUSE_MOTION          = 57,
    REQ_DO_STOP_MOTION           = 58,
    REQ_SET_TRACK_POS_OFFSET     = 59,
    REQ_GET_TRACK_POS_OFFSET     = 60,
    REQ_SET_TRACK_TIME_BIAS      = 61,
    REQ_GET_TRACK_TIME_BIAS      = 62,
    REQ_SET_ABS_ALTAZ_MOTION     = 63,
    REQ_SET_REL_ALTAZ_MOTION     = 64,
    REQ_SET_IDLE_MOTION          = 65,
    REQ_SET_PARK_MOTION          = 66,
    REQ_SET_CALIBRATION_MOTION   = 67,
    END_IMPL_COMMANDS            = 35,
    END_AMELAS_COMMANDS          = 69
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
    std::array<const char*, 38>
    {
        "FUTURE_EXAMPLE",
        "FUTURE_EXAMPLE",
        "REQ_SET_HOME_POSITION",
        "REQ_GET_HOME_POSITION",
        "FUTURE_EXAMPLE",
        "REQ_SET_IDLE_POS",
        "REQ_GET_IDLE_POS",
        "REQ_SET_PARK_POS",
        "REQ_GET_PARK_POS",
        "REQ_SET_CALIBRATION_POS",
        "REQ_GET_CALIBRATION_POS",
        "REQ_SET_IDLE_POS_HERE",
        "REQ_SET_PARK_POS_HERE",
        "REQ_SET_CALIBRATION_POS_HERE",
        "REQ_SET_HOMING_OFFSETS",
        "REQ_GET_HOMING_OFFSETS",
        "REQ_SET_WAIT_ALT",
        "REQ_GET_WAIT_ALT",
        "REQ_SET_SLEW_SPEED",
        "REQ_GET_SLEW_SPEED",
        "REQ_EN_TRACK_ADJ",
        "REQ_EN_MOUNT_POWER",
        "REQ_EN_MOUNT_MODEL",
        "REQ_SET_METEO_DATA",
        "REQ_GET_METEO_DATA",
        "REQ_DO_START_MOTION",
        "REQ_DO_PAUSE_MOTION",
        "REQ_DO_STOP_MOTION",
        "REQ_SET_TRACK_POS_OFFSET",
        "REQ_GET_TRACK_POS_OFFSET",
        "REQ_SET_TRACK_TIME_BIAS",
        "REQ_GET_TRACK_TIME_BIAS",
        "REQ_SET_ABS_ALTAZ_MOTION",
        "REQ_SET_REL_ALTAZ_MOTION",
        "REQ_SET_IDLE_MOTION",
        "REQ_SET_PARK_MOTION",
        "REQ_SET_CALIBRATION_MOTION",
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
