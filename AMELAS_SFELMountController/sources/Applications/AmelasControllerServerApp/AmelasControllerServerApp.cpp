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
 * @example AmelasControllerServerApp.cpp
 *
 * @brief TODO
 *
 * TODO
 *
 * @author Degoras Project Team
 * @author AVS AMELAS Team
 * @copyright EUPL License
 * @version 2309.5
***********************************************************************************************************************/

// C++ INCLUDES
// =====================================================================================================================
#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#endif
#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>
#include <limits>
#include <any>
#include <sstream>
#include <fstream>
// =====================================================================================================================

// ZMQUTILS INCLUDES
// =====================================================================================================================
#include <LibZMQUtils/Utils>
// =====================================================================================================================

// AMELAS INTERFACE INCLUDES
// =====================================================================================================================
#include <AmelasServerInterface>
// =====================================================================================================================

// LOG LIBRARY INCLUDES
// =====================================================================================================================
#include "spdlog/spdlog.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
// =====================================================================================================================

// JSON INCLUDES
// =====================================================================================================================
#include "nlohmann/json.hpp"
// =====================================================================================================================

enum AmelasLogLevel
{
    critical = spdlog::level::critical,
    error = spdlog::level::err,
    warning = spdlog::level::warn,
    info = spdlog::level::info,
    debug = spdlog::level::debug,
    trace = spdlog::level::trace
};

NLOHMANN_JSON_SERIALIZE_ENUM(AmelasLogLevel, {
    {critical, "critical"},
    {error, "error"},
    {warning, "warning"},
    {info, "info"},
    {debug, "debug"},
    {trace, "trace"},
})

struct AmelasControllerServerLogConsoleConfig
{
    AmelasLogLevel level;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AmelasControllerServerLogConsoleConfig, level)

struct AmelasControllerServerLogFileConfig
{
    AmelasLogLevel level;
    std::string path;
    int newFileHour;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AmelasControllerServerLogFileConfig, level, path, newFileHour)

struct AmelasControllerServerLogConfig
{
    AmelasControllerServerLogConsoleConfig console;
    AmelasControllerServerLogFileConfig file;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AmelasControllerServerLogConfig, console, file)

struct AmelasControllerServerConfig
{
    amelas::controller::AmelasControllerConfig controller;
    AmelasControllerServerLogConfig log;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AmelasControllerServerConfig, controller, log)

/**
 * @brief Main entry point of the program AmelasControllerServerApp.
 *
 * TODO
 *
 * WARNING This now is a simple example. This must be replaced by the real server program.
 *
 */
int main(int, char**)
{

    // MAIN SERVER APP
    // GLOBAL CONFIG
    // AMELAS SERVER
    // ETC
    // WARNING: WINDOWS PROCESS MODE


    // Nampesaces.
    using amelas::communication::AmelasControllerServer;
    using amelas::communication::AmelasServerCommand;
    using amelas::controller::AmelasController;

    // Configure the console.
    zmqutils::utils::ConsoleConfig& console_cfg = zmqutils::utils::ConsoleConfig::getInstance();
    console_cfg.configureConsole(true, true, true);

    // TODO: Load the configuration
    std::ifstream f("config.json");
    nlohmann::json json_config = nlohmann::json::parse(f);
    auto config = json_config.template get<AmelasControllerServerConfig>();

    // Configure the logging
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level((spdlog::level::level_enum)config.log.console.level);
    console_sink->set_pattern("[%^%l%$] %v");
    auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(config.log.file.path, 
                                                                            config.log.file.newFileHour, 0);
    file_sink->set_level((spdlog::level::level_enum)config.log.file.level);
    file_sink->set_pattern("[%^%l%$] %v");
    spdlog::sinks_init_list sinks = {console_sink, file_sink};
    auto logger = std::make_shared<spdlog::logger>("AmelasLogger", sinks);

    // Configuration variables.
    unsigned port = 9999;
    bool client_status_check = true;

    // Instantiate the Amelas controller.
    AmelasController amelas_controller(config.controller, logger);

    // Instantiate the server.
    AmelasControllerServer amelas_server(logger, port);

    // Disable or enables the client status checking.
    amelas_server.setClientStatusCheck(client_status_check);

    // ---------------------------------------

    // Set the controller callbacks in the server.
    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_DO_RESET_STATE,
                                             &amelas_controller,
                                             &AmelasController::doResetState);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_EN_TRACK_ADJ,
                                             &amelas_controller,
                                             &AmelasController::enableTrackingAdjusts);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_EN_MOUNT_POWER,
                                             &amelas_controller,
                                             &AmelasController::enableMountPower);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_SET_SLEW_SPEED,
                                             &amelas_controller,
                                             &AmelasController::setSlewSpeed);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_GET_SLEW_SPEED,
                                             &amelas_controller,
                                             &AmelasController::getSlewSpeed);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_SET_HOME_POSITION,
                                             &amelas_controller,
                                             &AmelasController::setHomePosition);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_GET_HOME_POSITION,
                                             &amelas_controller,
                                             &AmelasController::getHomePosition);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_SET_IDLE_POS,
                                             &amelas_controller,
                                             &AmelasController::setIdlePosition);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_GET_IDLE_POS,
                                             &amelas_controller,
                                             &AmelasController::getIdlePosition);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_SET_PARK_POS,
                                             &amelas_controller,
                                             &AmelasController::setParkPosition);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_GET_PARK_POS,
                                             &amelas_controller,
                                             &AmelasController::getParkPosition);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_SET_CALIBRATION_POS,
                                             &amelas_controller,
                                             &AmelasController::setCalibrationPosition);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_GET_CALIBRATION_POS,
                                             &amelas_controller,
                                             &AmelasController::getCalibrationPosition);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_SET_IDLE_POS_HERE,
                                             &amelas_controller,
                                             &AmelasController::setIdlePositionHere);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_SET_PARK_POS_HERE,
                                             &amelas_controller,
                                             &AmelasController::setParkPositionHere);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_SET_CALIBRATION_POS_HERE,
                                             &amelas_controller,
                                             &AmelasController::setCalibrationPositionHere);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_SET_WAIT_ALT,
                                             &amelas_controller,
                                             &AmelasController::setWaitAlt);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_GET_WAIT_ALT,
                                             &amelas_controller,
                                             &AmelasController::getWaitAlt);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_SET_HOMING_OFFSETS,
                                             &amelas_controller,
                                             &AmelasController::setHomingOffsets);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_GET_HOMING_OFFSETS,
                                             &amelas_controller,
                                             &AmelasController::getHomingOffsets);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_EN_MOUNT_MODEL,
                                             &amelas_controller,
                                             &AmelasController::enableMountModel);

    /*amelas_server.registerControllerCallback(AmelasServerCommand::REQ_SET_LOCATION,
                                             &amelas_controller,
                                             &AmelasController::setLocation);*/

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_SET_METEO_DATA,
                                             &amelas_controller,
                                             &AmelasController::setMeteoData);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_GET_METEO_DATA,
                                             &amelas_controller,
                                             &AmelasController::getMeteoData);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_DO_START_MOTION,
                                             &amelas_controller,
                                             &AmelasController::doStartMotion);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_DO_PAUSE_MOTION,
                                             &amelas_controller,
                                             &AmelasController::doPauseMotion);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_DO_STOP_MOTION,
                                             &amelas_controller,
                                             &AmelasController::doStopMotion);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_SET_TRACK_POS_OFFSET,
                                             &amelas_controller,
                                             &AmelasController::setTrackPosOffset);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_GET_TRACK_POS_OFFSET,
                                             &amelas_controller,
                                             &AmelasController::getTrackPosOffset);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_SET_TRACK_TIME_BIAS,
                                             &amelas_controller,
                                             &AmelasController::setTrackTimeBias);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_GET_TRACK_TIME_BIAS,
                                             &amelas_controller,
                                             &AmelasController::getTrackTimeBias);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_SET_ABS_ALTAZ_MOTION,
                                             &amelas_controller,
                                             &AmelasController::setAbsoluteAltAzMotion);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_SET_REL_ALTAZ_MOTION,
                                             &amelas_controller,
                                             &AmelasController::setRelativeAltAzMotion);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_SET_CON_ALTAZ_MOTION,
                                             &amelas_controller,
                                             &AmelasController::setContAltAzMotion);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_SET_IDLE_MOTION,
                                             &amelas_controller,
                                             &AmelasController::setIdleMotion);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_SET_PARK_MOTION,
                                             &amelas_controller,
                                             &AmelasController::setParkMotion);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_SET_CALIBRATION_MOTION,
                                             &amelas_controller,
                                             &AmelasController::setCalibrationMotion);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_DO_CONNECT_PLC,
                                             &amelas_controller,
                                             &AmelasController::doConnectPLC);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_DO_DISCONNECT_PLC,
                                             &amelas_controller,
                                             &AmelasController::doDisconnectPLC);

    // ---------------------------------------

    // Start the server.
    bool started = amelas_server.startServer();

    // Check if the server starts ok.
    if(!started)
    {
        // Log.
        std::cout << "Server start failed!! Press Enter to exit!" << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.clear();
        return 1;
    }

    // Wait for closing as an infinite loop until ctrl-c.
    console_cfg.waitForClose();

    // Log.
    std::cout << "Stopping the server..." << std::endl;

    // Stop the server.
    amelas_server.stopServer();

    // Final log.
    std::cout << "Server stoped. All ok!!" << std::endl;

    // Restore the console.
    console_cfg.restoreConsole();

    // Return.
	return 0;
}

// ---------------------------------------------------------------------------------------------------------------------
