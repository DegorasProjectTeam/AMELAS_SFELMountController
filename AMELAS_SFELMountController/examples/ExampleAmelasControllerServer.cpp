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
 * @example ExampleAmelasControllerServer.cpp
 *
 * @brief EXAMPLE FILE - This file serves as a program example of how to use the class `AmelasControllerServer` and
 * the `AmelasController` class.
 *
 * This program initializes an instance of the AmelasServer class and sets it up to interact with an instance of
 * the AmelasController class. The server is set up to respond to client requests by invoking callback methods on the
 * controller. The program will run indefinitely until the user hits ctrl-c.
 *
 * @author Degoras Project Team
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
#include "spdlog/sinks/stdout_color_sinks.h"
// =====================================================================================================================

/**
 * @brief Main entry point of the program ExampleAmelasControllerServer.
 *
 * Initializes an AmelasController and AmelasServer, then enters an infinite loop where it listens for client requests
 * and processes them using the server. If the user hits ctrl-c, the server is shut down and the program exits.
 */
int main(int, char**)
{
    // Nampesaces.
    using amelas::communication::AmelasControllerServer;
    using amelas::communication::AmelasServerCommand;
    using amelas::controller::AmelasController;
    using amelas::controller::AmelasControllerConfig;

    // Configure the console.
    zmqutils::utils::ConsoleConfig& console_cfg = zmqutils::utils::ConsoleConfig::getInstance();
    console_cfg.configureConsole(true, true, true);

    // Configure the logging
    auto logger = spdlog::stdout_color_mt("console");

    // Configuration variables.
    unsigned port = 9999;
    bool client_status_check = true;

    // Instantiate the Amelas controller.
    AmelasControllerConfig amemlas_controller_config;
    AmelasController amelas_controller(amemlas_controller_config, logger);

    // Instantiate the server.
    AmelasControllerServer amelas_server(logger, port);

    // Disable or enables the client status checking.
    amelas_server.setClientStatusCheck(client_status_check);

    // ---------------------------------------

    // Set the controller callbacks in the server.

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
