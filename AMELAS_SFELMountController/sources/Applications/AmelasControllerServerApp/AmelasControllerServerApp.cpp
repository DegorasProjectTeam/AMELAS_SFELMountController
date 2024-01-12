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

    // Configure the logging
    // TODO: Cargar el nivel de log de la consola desde la configuración
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::info);
    console_sink->set_pattern("[%^%l%$] %v");
    // TODO: Cargar el path del log desde la configuración
    // TODO: Cargar la hora de salto de fichero de log mediante configuración
    // TODO: Nivel de log desde archivo de configuración
    auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>("logs/daily.txt", 8, 0);
    file_sink->set_level(spdlog::level::debug);
    file_sink->set_pattern("[%^%l%$] %v");
    spdlog::sinks_init_list sinks = {console_sink, file_sink};
    auto logger = std::make_shared<spdlog::logger>("AmelasLogger", sinks);

    // Configuration variables.
    unsigned port = 9999;
    bool client_status_check = true;

    // Instantiate the Amelas controller.
    AmelasController amelas_controller;

    // Instantiate the server.
    AmelasControllerServer amelas_server(port);

    // Disable or enables the client status checking.
    amelas_server.setClientStatusCheck(client_status_check);

    // ---------------------------------------

    // Set the controller callbacks in the server.

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_SET_HOME_POSITION,
                                             &amelas_controller,
                                             &AmelasController::setHomePosition);

    amelas_server.registerControllerCallback(AmelasServerCommand::REQ_GET_HOME_POSITION,
                                             &amelas_controller,
                                             &AmelasController::getHomePosition);

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
