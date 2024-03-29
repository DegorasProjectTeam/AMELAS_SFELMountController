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
    REQ_SET_HOME_POSITION = 33,
    REQ_GET_HOME_POSITION = 34,
    END_IMPL_COMMANDS     = 35,
    END_AMELAS_COMMANDS   = 50
};

// Specific subclass errors (0 to 30 are reserved for the base server).
enum class AmelasServerResult : zmqutils::common::ResultType
{
    EMPTY_CALLBACK = 31,
    INVALID_CALLBACK = 32
};

// Extend the base command strings with those of the subclass.
static constexpr auto AmelasServerCommandStr = zmqutils::utils::joinArraysConstexpr(
    zmqutils::common::ServerCommandStr,
    std::array<const char*, 5>
    {
        "FUTURE_EXAMPLE",
        "FUTURE_EXAMPLE",
        "REQ_SET_HOME_POSITION",
        "REQ_GET_HOME_POSITION",
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
