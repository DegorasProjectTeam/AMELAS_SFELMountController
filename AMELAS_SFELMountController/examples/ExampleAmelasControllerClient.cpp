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
 * @example ExampleAmelasControllerClient.cpp
 *
 * @brief EXAMPLE FILE - This file serves as a program example of how to use the `AmelasControllerClient` class.
 *
 * This basic example program initializes an instance of the `AmelasControllerClient` class to interact with a running
 * instance of `AmelasControllerServer`.
 *
 * WARNING In development.
 *
 * @author Degoras Project Team
 * @copyright EUPL License
 * @version 2309.5
***********************************************************************************************************************/

// C++ INCLUDES
// =====================================================================================================================
#include <iostream>
#include <cstring>
// =====================================================================================================================

// ZMQUTILS INCLUDES
// =====================================================================================================================
#include <LibZMQUtils/Utils>
// =====================================================================================================================

// AMELAS INTERFACE INCLUDES
// =====================================================================================================================
#include <AmelasClientInterface>
// =====================================================================================================================

// TODO Remove
using namespace zmqutils;
using namespace amelas;
using namespace amelas::communication;
using namespace amelas::communication::common;
using namespace amelas::controller;


using zmqutils::common::CommandType;
using zmqutils::common::CommandReply;
using zmqutils::common::ServerCommand;
using zmqutils::utils::BinarySerializer;

void parseCommand(CommandClientBase &client, const std::string &command)
{
    zmqutils::common::OperationResult client_result = OperationResult::COMMAND_OK;

    char *command_str = new char[command.size()];
    std::copy(command.begin(), command.end(), command_str);

    char *token = std::strtok(command_str, " ");

    if (token)
    {
        CommandType command_id;

        try
        {
            command_id = static_cast<CommandType>(std::stoi(token));
        }
        catch (...)
        {
            std::cerr << "Failed at sending command." << std::endl;
            delete[] command_str;
            return;
        }

        RequestData command_msg(static_cast<ServerCommand>(command_id));

        bool valid = true;

        if (command_id == static_cast<CommandType>(ServerCommand::REQ_CONNECT))
        {
            std::cout << "Sending REQ_CONNECT command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(ServerCommand::REQ_DISCONNECT))
        {
            std::cout << "Sending REQ_DISCONNECT command" << std::endl;
        }
        else if (command_id == static_cast<CommandType>(ServerCommand::REQ_ALIVE))
        {
            std::cout << "Sending REQ_ALIVE command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(ServerCommand::REQ_GET_SERVER_TIME))
        {
            std::cout << "Sending REQ_GET_SERVER_TIME command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_HOME_POSITION))
        {
            std::cout << "Sending get home position command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_SET_HOME_POSITION))
        {
            std::cout << "Sending set home position command." << std::endl;

            bool valid_params = true;
            double az = 0., el = 0.;
            char *param_token = std::strtok(nullptr, " ");

            try
            {
                az = std::stod(param_token);
            }
            catch (...)
            {
                std::cerr << "Bad parameter azimuth issued.";
                valid_params = false;
            }

            if (valid_params)
            {
                param_token = std::strtok(nullptr, " ");

                try
                {
                    el = std::stod(param_token);
                }
                catch (...)
                {
                    std::cerr << "Bad parameter elevation issued.";
                    valid_params = false;
                }
            }

            if (valid_params)
            {
                std::cout<<"Sending: " << az <<" "<<el<<std::endl;

                AltAzPos pos(az, el);

                BinarySerializer serializer;

                serializer.write(pos);

                std::cout<<serializer.toJsonString();

                command_msg.params_size = BinarySerializer::fastSerialization(command_msg.params, pos);

                std::cout<<std::endl;
            }
            else
            {
                std::cout<<"Sending invalid command: "<<std::endl;
                command_msg.params_size = BinarySerializer::fastSerialization(command_msg.params, az);

                valid_params = true;
            }

            valid = valid_params;

        }
        else
        {
            valid = false;
        }

        // TODO MOVE ALL OF THIS TO A SUBCLASS IN A PURE VIRTUAL. THE FUNCTION WILL RETURN ClientResult
        // TODO THE ERROR CONTROL MUST BE IN THE BASE CLIENT. THE SUBCLASS MUST CONTROL THE OUTPUT DATA AND CUSTOM ERRORS ONLY.
        // TODO DISABLE SEND WITH THIS WAY THE RESERVED COMMANDS.
        // TODO CREATE doConnect, doDisconnect, checkServerAlive
        // TODO CREATE IN THE CLIENT THE INTERNAL CALLBACKS LIKE THE SERVER.
        // TODO MOVE THE PROCESSING OF EACH COMPLEX RESPONSE TO A FUNCTION.

        if (valid)
        {
            // TODO MOVE ALL
            CommandReply reply;

            if(command_msg.command == ServerCommand::REQ_CONNECT)
            {
                client_result = client.doConnect();

                if (client_result == OperationResult::CLIENT_STOPPED)
                {
                    delete[] command_str;
                    return;
                }


            }
            else if(command_msg.command == ServerCommand::REQ_GET_SERVER_TIME)
            {
                std::string datetime;
                client_result = client.doGetServerTime(datetime);

                if (client_result == OperationResult::COMMAND_OK)
                {
                    std::cout<<"Server time: "<<datetime<<std::endl;

                    delete[] command_str;
                    return;
                }

            }
            else
                client_result = client.sendCommand(command_msg, reply);

            std::cerr << "Client Result: " << static_cast<int>(client_result)<<std::endl;

            if (client_result != OperationResult::COMMAND_OK)
            {
            }
            else
            {


                std::cout<<"Server result: "<<static_cast<int>(reply.server_result)<<std::endl;

                if(reply.server_result != OperationResult::COMMAND_OK)
                {
                    delete[] command_str;
                    return;
                }

                // Get the controller result.
                // TODO ERROR CONTROL

                if(command_id > static_cast<CommandType>(ServerCommand::END_BASE_COMMANDS))
                {
                    AmelasError error;

                    BinarySerializer ser(reply.params.get(), reply.params_size);
                    std::cout<<ser.toJsonString()<<std::endl;

                    ser.read(error);

                    std::cout<<"Controller error: "<<static_cast<int>(error)<<std::endl;
                }

                if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_HOME_POSITION))
                {
                    try
                    {
                        AmelasError error;   // Trash. The controller error must be checked.
                        double az;
                        double el;

                        // Deserialize the parameters.
                        BinarySerializer::fastDeserialization(reply.params.get(), reply.params_size, error, az, el);

                        // Generate the struct.
                        std::cout<<"Az: "<<az<<std::endl;
                        std::cout<<"El: "<<el<<std::endl;
                    }
                    catch(...)
                    {
                        std::cout<<"BAD PARAMS"<<std::endl;
                        // RETURN BAD PARAMS
                        //result = ClientResult::
                    }
                }
            }
        }
        else
        {
            std::cerr << "Command is not implemented or valid" << std::endl;
        }

    }
    else
    {
        std::cerr << "Not a valid command" << std::endl;
    }


    delete[] command_str;
}

/**
 * @brief Main entry point of the program `ExampleClientAmelas`.
 */
int main(int, char**)
{
    // Configure the console.
    zmqutils::utils::ConsoleConfig& console_cfg = zmqutils::utils::ConsoleConfig::getInstance();
    console_cfg.configureConsole(true, false, false);

    // Configuration variables.
    unsigned port = 9999;
    std::string ip = "127.0.0.1";

    std::string endpoint = "tcp://" + ip + ":" + std::to_string(port);
    
    AmelasControllerClient client(endpoint, "AMELAS EXAMPLE CLIENT");

    // Configure the client.
    client.setAliveCallbacksEnabled(false);

    // Set the exit callback to the console handler for safety.
    console_cfg.setExitCallback(
            [&client]()
            {
                std::cout << std::endl;
                std::cout << "Stopping the client..." << std::endl;
                client.stopClient();
            });

    bool started = client.startClient();

    if(!started)
    {
        std::cout<<"Unable to start the client.";
        return 1;
    }

    //client.startAutoAlive();
    std::string command;

    // Infinite loop for test.
    while(!console_cfg.closeStatus())
    {
        // Get the command and parameters.
        std::cout<<"------------------------------------------------------"<<std::endl;
        std::cout<<"-- Commands --"<<std::endl;
        std::cout<<"- REQ_CONNECT:          0"<<std::endl;
        std::cout<<"- REQ_DISCONNECT:       1"<<std::endl;
        std::cout<<"- REQ_ALIVE:            2"<<std::endl;
        std::cout<<"- REQ_GET_SERVER_TIME:  3"<<std::endl;
        std::cout<<"- CUSTOM:         cmd param1 param2 ..."<<std::endl;
        std::cout<<"-- Other --"<<std::endl;
        std::cout<<"- Client exit:             exit"<<std::endl;
        std::cout<<"- Enable auto-alive:       auto_alive_en"<<std::endl;
        std::cout<<"- Disable auto-alive:      auto_alive_ds"<<std::endl;
        std::cout<<"- Enable auto-alive clbk:  auto_alive_clbk_en"<<std::endl;
        std::cout<<"- Disable auto-alive clbk: auto_alive_clbk_ds"<<std::endl;
        std::cout<<"------------------------------------------------------"<<std::endl;
        std::cout<<"Write a command: ";
        std::getline(std::cin, command);

        // Check for other commands.
        if(command == "exit")
        {
            std::cout << "Stopping the client..." << std::endl;
            client.stopClient();
            break;
        }
        else if(command == "auto_alive_en")
        {
            std::cout << "Enabling auto-alive..." << std::endl;
            client.doDisconnect();
            client.doConnect(true);
            continue;
        }
        else if(command == "auto_alive_ds")
        {
            std::cout << "Disabling auto-alive..." << std::endl;
            client.disableAutoAlive();
            continue;
        }
        else if(command == "auto_alive_clbk_en")
        {
            std::cout << "Enabling auto-alive clbk..." << std::endl;
            client.setAliveCallbacksEnabled(true);
            continue;
        }
        else if(command == "auto_alive_clbk_ds")
        {
            std::cout << "Disabling auto-alive clbk..." << std::endl;
            client.setAliveCallbacksEnabled(false);
            continue;
        }

        // Break if we want to close the example program.
        if(console_cfg.closeStatus() || std::cin.eof())
        {
            console_cfg.waitForClose();
            break;
        }

        // Parse the command.
        parseCommand(client, command);
    }

    // Final log.
    std::cout << "Client stoped. All ok!!" << std::endl;

    // Restore the console.
    console_cfg.restoreConsole();

	return 0;
}

// ---------------------------------------------------------------------------------------------------------------------
