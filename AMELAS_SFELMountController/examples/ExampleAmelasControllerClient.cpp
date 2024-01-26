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
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_DO_RESET_STATE))
        {
            std::cout << "Sending REQ_DO_RESET_STATE command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_EN_AVOID_SUN)
        || command_id == static_cast<CommandType>(AmelasServerCommand::REQ_EN_TRACK_ADJ)
        || command_id == static_cast<CommandType>(AmelasServerCommand::REQ_EN_MOUNT_POWER)
        || command_id == static_cast<CommandType>(AmelasServerCommand::REQ_EN_MOUNT_MODEL)
        || command_id == static_cast<CommandType>(AmelasServerCommand::REQ_EN_SIMULATION_MODE))
        {
            std::cout << "Sending REQ_ENABLE_X command." << std::endl;

            bool valid_params = true;
            bool enable = false;
            char *param_token = std::strtok(nullptr, " ");

            try
            {
                enable = std::stod(param_token);
            }
            catch (...)
            {
                std::cerr << "Bad parameter.";
                valid_params = false;
            }

            if (valid_params)
            {
                std::cout << "Sending: " << enable << std::endl;

                BinarySerializer serializer;

                serializer.write(enable);

                std::cout<<serializer.toJsonString();

                command_msg.params_size = BinarySerializer::fastSerialization(command_msg.params, enable);

                std::cout<<std::endl;
            }
            else
            {
                std::cout<<"Sending invalid command: "<<std::endl;
                command_msg.params_size = BinarySerializer::fastSerialization(command_msg.params, enable);

                valid_params = true;
            }

            valid = valid_params;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_DO_CONNECT_PLC))
        {
            std::cout << "Sending REQ_DO_CONNECT_PLC command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_DO_DISCONNECT_PLC))
        {
            std::cout << "Sending REQ_DO_CONNECT_PLC command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_PLC_REGISTERS))
        {
            std::cout << "Sending REQ_GET_PLC_REGISTERS command." << std::endl;

            /*bool valid_params = true;
            std::string symbol = "", type = "";
            char *param_token = std::strtok(nullptr, " ");

            try
            {
                symbol = std::stod(param_token);
            }
            catch (...)
            {
                std::cerr << "Bad parameter symbol issued.";
                valid_params = false;
            }

            if (valid_params)
            {
                param_token = std::strtok(nullptr, " ");

                try
                {
                    type = std::stod(param_token);
                }
                catch (...)
                {
                    std::cerr << "Bad parameter type issued.";
                    valid_params = false;
                }
            }

            if (valid_params)
            {
                std::cout << "Sending: " << symbol << " " << type << std::endl;

                PLCAddress address(symbol, type);

                BinarySerializer serializer;

                serializer.write(address);

                std::cout<<serializer.toJsonString();

                command_msg.params_size = BinarySerializer::fastSerialization(command_msg.params, address);

                std::cout<<std::endl;
            }
            else
            {
                std::cout<<"Sending invalid command: "<<std::endl;
                command_msg.params_size = BinarySerializer::fastSerialization(command_msg.params, symbol);

                valid_params = true;
            }

            valid = valid_params;*/
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_MOUNT_LOG))
        {
            std::cout << "Sending REQ_GET_MOUNT_LOG command." << std::endl;

            bool valid_params = true;
            std::string start = "", end = "";
            char *param_token = std::strtok(nullptr, " ");

            try
            {
                start = std::stod(param_token);
            }
            catch (...)
            {
                std::cerr << "Bad parameter start issued.";
                valid_params = false;
            }

            if (valid_params)
            {
                param_token = std::strtok(nullptr, " ");

                try
                {
                    end = std::stod(param_token);
                }
                catch (...)
                {
                    std::cerr << "Bad parameter end issued.";
                    valid_params = false;
                }
            }

            if (valid_params)
            {
                std::cout << "Sending: " << start << " " << end << std::endl;

                BinarySerializer serializer;

                serializer.write(start, end);

                std::cout<<serializer.toJsonString();

                command_msg.params_size = BinarySerializer::fastSerialization(command_msg.params, start, end);

                std::cout<<std::endl;
            }
            else
            {
                std::cout<<"Sending invalid command: "<<std::endl;
                command_msg.params_size = BinarySerializer::fastSerialization(command_msg.params, start, end);

                valid_params = true;
            }

            valid = valid_params;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_DO_SYNC_NTP))
        {
            std::cout << "Sending REQ_DO_SYNC_NTP command." << std::endl;

            bool valid_params = true;
            std::string host = "";
            unsigned port = 0, timeout = 0;
            char *param_token = std::strtok(nullptr, " ");

            try
            {
                host = std::stod(param_token);
            }
            catch (...)
            {
                std::cerr << "Bad parameter host issued.";
                valid_params = false;
            }

            if (valid_params)
            {
                param_token = std::strtok(nullptr, " ");

                try
                {
                    port = std::stod(param_token);
                }
                catch (...)
                {
                    std::cerr << "Bad parameter port issued.";
                    valid_params = false;
                }
            }

            if (valid_params)
            {
                param_token = std::strtok(nullptr, " ");

                try
                {
                    timeout = std::stod(param_token);
                }
                catch (...)
                {
                    std::cerr << "Bad parameter timeout issued.";
                    valid_params = false;
                }
            }

            if (valid_params)
            {
                std::cout << "Sending: " << host << " " << port << " " << timeout << std::endl;

                BinarySerializer serializer;

                serializer.write(host, port, timeout);

                std::cout<<serializer.toJsonString();

                command_msg.params_size = BinarySerializer::fastSerialization(command_msg.params, host, port, timeout);

                std::cout<<std::endl;
            }
            else
            {
                std::cout<<"Sending invalid command: "<<std::endl;
                command_msg.params_size = BinarySerializer::fastSerialization(command_msg.params, host, port, timeout);

                valid_params = true;
            }

            valid = valid_params;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_DO_SYNC_MANUAL)
        || command_id == static_cast<CommandType>(AmelasServerCommand::REQ_SET_SIMULATION_TIME))
        {
            std::cout << "Sending REQ_DO_SYNC_MANUAL or REQ_SET_SIMULATION_TIME command." << std::endl;

            bool valid_params = true;
            std::string datetime = "";
            char *param_token = std::strtok(nullptr, " ");

            try
            {
                datetime = std::stod(param_token);
            }
            catch (...)
            {
                std::cerr << "Bad parameter datetime issued.";
                valid_params = false;
            }

            if (valid_params)
            {
                std::cout << "Sending: " << datetime << std::endl;

                BinarySerializer serializer;

                serializer.write(datetime);

                std::cout<<serializer.toJsonString();

                command_msg.params_size = BinarySerializer::fastSerialization(command_msg.params, datetime);

                std::cout<<std::endl;
            }
            else
            {
                std::cout<<"Sending invalid command: "<<std::endl;
                command_msg.params_size = BinarySerializer::fastSerialization(command_msg.params, datetime);

                valid_params = true;
            }

            valid = valid_params;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_MOUNT_STATUS))
        {
            std::cout << "Sending REQ_GET_MOUNT_STATUS command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_MOUNT_INFO))
        {
            std::cout << "Sending REQ_GET_MOUNT_INFO command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_SET_HOME_POSITION)
        || command_id == static_cast<CommandType>(AmelasServerCommand::REQ_SET_IDLE_POS)
        || command_id == static_cast<CommandType>(AmelasServerCommand::REQ_SET_PARK_POS)
        || command_id == static_cast<CommandType>(AmelasServerCommand::REQ_SET_CALIBRATION_POS)
        || command_id == static_cast<CommandType>(AmelasServerCommand::REQ_SET_HOMING_OFFSETS)
        || command_id == static_cast<CommandType>(AmelasServerCommand::REQ_SET_SLEW_SPEED)
        || command_id == static_cast<CommandType>(AmelasServerCommand::REQ_SET_TRACK_POS_OFFSET))
        {
            std::cout << "Sending REQ_SET_X command." << std::endl;

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
                std::cout << "Sending: " << az << " " << el << std::endl;

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
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_SLEW_SPEED))
        {
            std::cout << "Sending REQ_GET_SLEW_SPEED command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_HOME_POSITION))
        {
            std::cout << "Sending REQ_GET_HOME_POSITION command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_IDLE_POS))
        {
            std::cout << "Sending REQ_GET_IDLE_POS command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_PARK_POS))
        {
            std::cout << "Sending REQ_GET_PARK_POS command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_CALIBRATION_POS))
        {
            std::cout << "Sending REQ_GET_CALIBRATION_POS command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_SET_IDLE_POS_HERE))
        {
            std::cout << "Sending REQ_SET_IDLE_POS_HERE command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_SET_PARK_POS_HERE))
        {
            std::cout << "Sending REQ_SET_PARK_POS_HERE command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_SET_CALIBRATION_POS_HERE))
        {
            std::cout << "Sending REQ_SET_CALIBRATION_POS_HERE command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_SET_WAIT_ALT)
        || command_id == static_cast<CommandType>(AmelasServerCommand::REQ_SET_TRACK_TIME_BIAS))
        {
            std::cout << "Sending REQ_SET_WAIT_ALT or REQ_SET_TRACK_TIME_BIAS command." << std::endl;

            bool valid_params = true;
            double alt = 0.;
            char *param_token = std::strtok(nullptr, " ");

            try
            {
                alt = std::stod(param_token);
            }
            catch (...)
            {
                std::cerr << "Bad parameter.";
                valid_params = false;
            }

            if (valid_params)
            {
                std::cout << "Sending: " << alt << std::endl;

                BinarySerializer serializer;

                serializer.write(alt);

                std::cout<<serializer.toJsonString();

                command_msg.params_size = BinarySerializer::fastSerialization(command_msg.params, alt);

                std::cout<<std::endl;
            }
            else
            {
                std::cout<<"Sending invalid command: "<<std::endl;
                command_msg.params_size = BinarySerializer::fastSerialization(command_msg.params, alt);

                valid_params = true;
            }

            valid = valid_params;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_WAIT_ALT))
        {
            std::cout << "Sending REQ_GET_WAIT_ALT command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_HOMING_OFFSETS))
        {
            std::cout << "Sending REQ_GET_HOMING_OFFSETS command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_MOUNT_MODEL_COEFS))
        {
            std::cout << "Sending REQ_GET_MOUNT_MODEL_COEFS command." << std::endl;
        }
        /*else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_SET_LOCATION))
        {
            std::cout << "Sending set REQ_SET_LOCATION command." << std::endl;

            bool valid_params = true;
            double lat = 0., lon = 0., alt = 0.;
            double x = 0., y = 0., z = 0.;
            char *param_token = std::strtok(nullptr, " ");

            try
            {
                lat = std::stod(param_token);
            }
            catch (...)
            {
                std::cerr << "Bad parameter latitude issued.";
                valid_params = false;
            }

            if (valid_params)
            {
                param_token = std::strtok(nullptr, " ");

                try
                {
                    lon = std::stod(param_token);
                }
                catch (...)
                {
                    std::cerr << "Bad parameter longitude issued.";
                    valid_params = false;
                }
            }

            if (valid_params)
            {
                param_token = std::strtok(nullptr, " ");

                try
                {
                    alt = std::stod(param_token);
                }
                catch (...)
                {
                    std::cerr << "Bad parameter altitude issued.";
                    valid_params = false;
                }
            }

            if (valid_params)
            {
                param_token = std::strtok(nullptr, " ");

                try
                {
                    x = std::stod(param_token);
                }
                catch (...)
                {
                    std::cerr << "Bad parameter x issued.";
                    valid_params = false;
                }
            }

            if (valid_params)
            {
                param_token = std::strtok(nullptr, " ");

                try
                {
                    y = std::stod(param_token);
                }
                catch (...)
                {
                    std::cerr << "Bad parameter y issued.";
                    valid_params = false;
                }
            }

            if (valid_params)
            {
                param_token = std::strtok(nullptr, " ");

                try
                {
                    z = std::stod(param_token);
                }
                catch (...)
                {
                    std::cerr << "Bad parameter z issued.";
                    valid_params = false;
                }
            }

            if (valid_params)
            {
                std::cout << "Sending: " << lat << " " << lon << " " << alt << " " << x << " " << y << " " << z << std::endl;

                WGS84Coords wgs84(lat, lon, alt);
                ECEFCoords ecef(x, y, z);
                StationLocation loc(wgs84, ecef);

                BinarySerializer serializer;

                serializer.write(loc);

                std::cout<<serializer.toJsonString();

                command_msg.params_size = BinarySerializer::fastSerialization(command_msg.params, loc);

                std::cout<<std::endl;
            }
            else
            {
                std::cout<<"Sending invalid command: "<<std::endl;
                command_msg.params_size = BinarySerializer::fastSerialization(command_msg.params, lat);

                valid_params = true;
            }

            valid = valid_params;

        }*/
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_LOCATION))
        {
            std::cout << "Sending REQ_GET_LOCATION command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_SET_METEO_DATA))
        {
            std::cout << "Sending REQ_SET_METEO_DATA command." << std::endl;

            bool valid_params = true;
            double press = 0., temp = 0., hr = 0.;
            char *param_token = std::strtok(nullptr, " ");

            try
            {
                press = std::stod(param_token);
            }
            catch (...)
            {
                std::cerr << "Bad parameter press issued.";
                valid_params = false;
            }

            if (valid_params)
            {
                param_token = std::strtok(nullptr, " ");

                try
                {
                    temp = std::stod(param_token);
                }
                catch (...)
                {
                    std::cerr << "Bad parameter temp issued.";
                    valid_params = false;
                }
            }

            if (valid_params)
            {
                param_token = std::strtok(nullptr, " ");

                try
                {
                    hr = std::stod(param_token);
                }
                catch (...)
                {
                    std::cerr << "Bad parameter hr issued.";
                    valid_params = false;
                }
            }

            if (valid_params)
            {
                std::cout << "Sending: " << press << " " << temp << " " << hr << std::endl;

                MeteoData meteo(press, temp, hr);

                BinarySerializer serializer;

                serializer.write(meteo);

                std::cout<<serializer.toJsonString();

                command_msg.params_size = BinarySerializer::fastSerialization(command_msg.params, meteo);

                std::cout<<std::endl;
            }
            else
            {
                std::cout<<"Sending invalid command: "<<std::endl;
                command_msg.params_size = BinarySerializer::fastSerialization(command_msg.params, press);

                valid_params = true;
            }

            valid = valid_params;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_METEO_DATA))
        {
            std::cout << "Sending REQ_GET_METEO_DATA command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_SIMULATION_STATE))
        {
            std::cout << "Sending REQ_GET_SIMULATION_STATE command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_MOTION_MODE))
        {
            std::cout << "Sending REQ_GET_MOTION_MODE command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_MOTION_STATE))
        {
            std::cout << "Sending REQ_GET_MOTION_STATE command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_DO_START_MOTION))
        {
            std::cout << "Sending REQ_DO_START_MOTION command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_DO_PAUSE_MOTION))
        {
            std::cout << "Sending REQ_DO_PAUSE_MOTION command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_DO_STOP_MOTION))
        {
            std::cout << "Sending REQ_DO_STOP_MOTION command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_TRACK_POS_OFFSET))
        {
            std::cout << "Sending REQ_GET_TRACK_POS_OFFSET command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_TRACK_TIME_BIAS))
        {
            std::cout << "Sending REQ_GET_TRACK_TIME_BIAS command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_SET_ABS_ALTAZ_MOTION)
        || command_id == static_cast<CommandType>(AmelasServerCommand::REQ_SET_REL_ALTAZ_MOTION))
        {
            std::cout << "Sending set pos-vel command." << std::endl;

            bool valid_params = true;
            double az = 0., el = 0.;
            double az_vel = 0., el_vel = 0.;
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
                param_token = std::strtok(nullptr, " ");

                try
                {
                    az_vel = std::stod(param_token);
                }
                catch (...)
                {
                    std::cerr << "Bad parameter 3 issued.";
                    valid_params = false;
                }
            }

            if (valid_params)
            {
                param_token = std::strtok(nullptr, " ");

                try
                {
                    el_vel = std::stod(param_token);
                }
                catch (...)
                {
                    std::cerr << "Bad parameter elevation issued.";
                    valid_params = false;
                }
            }

            if (valid_params)
            {
                std::cout << "Sending: " << az << " " << el << " " << az_vel << " " << el_vel << std::endl;

                AltAzPos pos(az, el);
                AltAzVel vel(az_vel, el_vel);

                BinarySerializer serializer;

                serializer.write(pos, vel);

                std::cout<<serializer.toJsonString();

                command_msg.params_size = BinarySerializer::fastSerialization(command_msg.params, pos, vel);

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
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_SET_CON_ALTAZ_MOTION))
        {
            std::cout << "Sending REQ_SET_CON_ALTAZ_MOTION command." << std::endl;

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
                std::cout << "Sending: " << az << " " << el << std::endl;

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
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_SET_HOMING_MOTION))
        {
            std::cout << "Sending REQ_SET_HOMING_MOTION command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_SET_IDLE_MOTION))
        {
            std::cout << "Sending REQ_SET_IDLE_MOTION command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_SET_PARK_MOTION))
        {
            std::cout << "Sending REQ_SET_PARK_MOTION command." << std::endl;
        }
        else if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_SET_CALIBRATION_MOTION))
        {
            std::cout << "Sending REQ_SET_CALIBRATION_MOTION command." << std::endl;
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
                client_result = client.doConnect(true);

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

                if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_HOME_POSITION)
                    || command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_IDLE_POS)
                    || command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_PARK_POS)
                    || command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_CALIBRATION_POS)
                    || command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_TRACK_POS_OFFSET)
                    || command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_HOMING_OFFSETS)
                    || command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_SLEW_SPEED))
                {
                    try
                    {
                        AmelasError error;   // Trash. The controller error must be checked.
                        double az;
                        double el;

                        // Deserialize the parameters.
                        BinarySerializer::fastDeserialization(reply.params.get(), reply.params_size, error, az, el);

                        // Generate the struct.
                        std::cout << "Az: " << az << std::endl;
                        std::cout << "El: " << el << std::endl;
                    }
                    catch(...)
                    {
                        std::cout<<"BAD PARAMS"<<std::endl;
                        // RETURN BAD PARAMS
                        //result = ClientResult::
                    }
                }

                /*if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_PLC_REGISTERS))
                {
                    try
                    {
                        AmelasError error;   // Trash. The controller error must be checked.
                        std::string symbol, type, value;

                        // Deserialize the parameters.
                        BinarySerializer::fastDeserialization(reply.params.get(), reply.params_size, error, symbol, type, value);

                        // Generate the struct.
                        std::cout << "Symbol: " << symbol << " <" << type << "> = " << value << std::endl;
                    }
                    catch(...)
                    {
                        std::cout<<"BAD PARAMS"<<std::endl;
                        // RETURN BAD PARAMS
                        //result = ClientResult::
                    }
                }*/

                if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_METEO_DATA))
                {
                    try
                    {
                        AmelasError error;   // Trash. The controller error must be checked.
                        double press;
                        double temp;
                        double hr;

                        // Deserialize the parameters.
                        BinarySerializer::fastDeserialization(reply.params.get(), reply.params_size, error, press, temp, hr);

                        // Generate the struct.
                        std::cout << "Press: " << press << std::endl;
                        std::cout << "Temp: " << temp << std::endl;
                        std::cout << "Hr: " << hr << std::endl;
                    }
                    catch(...)
                    {
                        std::cout<<"BAD PARAMS"<<std::endl;
                        // RETURN BAD PARAMS
                        //result = ClientResult::
                    }
                }

                if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_LOCATION))
                {
                    try
                    {
                        AmelasError error;   // Trash. The controller error must be checked.
                        double lat;
                        double lon;
                        double alt;
                        double x;
                        double y;
                        double z;

                        // Deserialize the parameters.
                        BinarySerializer::fastDeserialization(reply.params.get(), reply.params_size, error, lat, lon, alt, x, y, z);

                        // Generate the struct.
                        std::cout << "Lat: " << lat << std::endl;
                        std::cout << "Lon: " << lon << std::endl;
                        std::cout << "Alt: " << alt << std::endl;
                        std::cout << "X: " << x << std::endl;
                        std::cout << "Y: " << y << std::endl;
                        std::cout << "Z: " << z << std::endl;
                    }
                    catch(...)
                    {
                        std::cout<<"BAD PARAMS"<<std::endl;
                        // RETURN BAD PARAMS
                        //result = ClientResult::
                    }
                }

                if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_MOTION_MODE))
                {
                    try
                    {
                        AmelasError error;   // Trash. The controller error must be checked.
                        AmelasMotionMode mode;

                        // Deserialize the parameters.
                        BinarySerializer::fastDeserialization(reply.params.get(), reply.params_size, error, mode);

                        // Generate the struct.
                        std::string motion_str = MotionModeStr[static_cast<size_t>(mode)];
                        std::cout << "Motion mode: " << motion_str << std::endl;
                    }
                    catch(...)
                    {
                        std::cout<<"BAD PARAMS"<<std::endl;
                        // RETURN BAD PARAMS
                        //result = ClientResult::
                    }
                }

                if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_MOTION_STATE))
                {
                    try
                    {
                        AmelasError error;   // Trash. The controller error must be checked.
                        AmelasMotionState state;

                        // Deserialize the parameters.
                        BinarySerializer::fastDeserialization(reply.params.get(), reply.params_size, error, state);

                        // Generate the struct.
                        std::string motion_str = MotionStateStr[static_cast<size_t>(state)];
                        std::cout << "Motion state: " << motion_str << std::endl;
                    }
                    catch(...)
                    {
                        std::cout<<"BAD PARAMS"<<std::endl;
                        // RETURN BAD PARAMS
                        //result = ClientResult::
                    }
                }

                if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_WAIT_ALT))
                {
                    try
                    {
                        AmelasError error;   // Trash. The controller error must be checked.
                        double waitAlt;

                        // Deserialize the parameters.
                        BinarySerializer::fastDeserialization(reply.params.get(), reply.params_size, error, waitAlt);

                        // Generate the struct.
                        std::cout << "Wait alt: " << waitAlt << std::endl;
                    }
                    catch(...)
                    {
                        std::cout<<"BAD PARAMS"<<std::endl;
                        // RETURN BAD PARAMS
                        //result = ClientResult::
                    }
                }

                if (command_id == static_cast<CommandType>(AmelasServerCommand::REQ_GET_TRACK_TIME_BIAS))
                {
                    try
                    {
                        AmelasError error;   // Trash. The controller error must be checked.
                        double timeBias;

                        // Deserialize the parameters.
                        BinarySerializer::fastDeserialization(reply.params.get(), reply.params_size, error, timeBias);

                        // Generate the struct.
                        std::cout << "Track time bias: " << timeBias << " ms" << std::endl;
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
