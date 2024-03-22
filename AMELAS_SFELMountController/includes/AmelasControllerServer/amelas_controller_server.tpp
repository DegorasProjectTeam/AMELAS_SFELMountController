#ifndef AMELAS_CONTROLLER_SERVER_TPP_
#define AMELAS_CONTROLLER_SERVER_TPP_

template <typename ClbkT>
void AmelasControllerServer::processGetMotionMode(const CommandRequest &request, CommandReply &reply)
{
    // Auxiliar variables and containers.
    controller::AmelasError ctrl_err;
    controller::AmelasMotionMode mode;

    // Now we will process the command in the controller.
    ctrl_err = this->invokeCallback<ClbkT>(request, reply, mode);

    // Serialize parameters if all ok.
    if(reply.server_result == OperationResult::COMMAND_OK)
        reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err, mode);
}

template <typename ClbkT>
void AmelasControllerServer::processGetMotionState(const CommandRequest &request, CommandReply &reply)
{
    // Auxiliar variables and containers.
    controller::AmelasError ctrl_err;
    controller::AmelasMotionState state;
    controller::AltAzPos pos;

    // Now we will process the command in the controller.
    ctrl_err = this->invokeCallback<ClbkT>(request, reply, state, pos);

    // Serialize parameters if all ok.
    if(reply.server_result == OperationResult::COMMAND_OK)
        reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err, state, pos.az, pos.el);
}

template <typename ClbkT>
void AmelasControllerServer::processGetPositionOrSpeed(const CommandRequest &request, CommandReply &reply)
{
    // Auxiliar variables and containers.
    controller::AmelasError ctrl_err;
    controller::AltAzPos pos;

    // Now we will process the command in the controller.
    ctrl_err = this->invokeCallback<ClbkT>(request, reply, pos);

    // Serialize parameters if all ok.
    if(reply.server_result == OperationResult::COMMAND_OK)
        reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err, pos.az, pos.el);
}

template <typename ClbkT>
void AmelasControllerServer::processEmptyArguments(const CommandRequest &request, CommandReply &reply)
{
    // Auxiliar variables and containers.
    controller::AmelasError ctrl_err;

    // Now we will process the command in the controller.
    ctrl_err = this->invokeCallback<ClbkT>(request, reply);

    // Serialize parameters if all ok.
    if(reply.server_result == OperationResult::COMMAND_OK)
        reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err);
}

template <typename ClbkT>
void AmelasControllerServer::processSetPositionOrSpeed(const CommandRequest& request, CommandReply& reply)
{
    // Auxiliar variables and containers.
    controller::AmelasError ctrl_err;

    // Position struct.
    controller::AltAzPos pos;

    // Check the request parameters size.
    if (request.params_size == 0 || !request.params)
    {
        reply.server_result = OperationResult::EMPTY_PARAMS;
        return;
    }

    // Try to read the parameters data.
    try
    {
        BinarySerializer::fastDeserialization(request.params.get(), request.params_size, pos);
    }
    catch(...)
    {
        reply.server_result = OperationResult::BAD_PARAMETERS;
        return;
    }

    // Now we will process the command in the controller.
    ctrl_err = this->invokeCallback<ClbkT>(request, reply, pos);

    // Serialize parameters if all ok.
    if(reply.server_result == OperationResult::COMMAND_OK)
        reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err);
}

/*template <typename ClbkT>
void AmelasControllerServer::processSetLocation(const CommandRequest& request, CommandReply& reply)
{
    // Auxiliar variables and containers.
    controller::AmelasError ctrl_err;

    // Position struct.
    controller::StationLocation loc;

    // Check the request parameters size.
    if (request.params_size == 0 || !request.params)
    {
        reply.server_result = OperationResult::EMPTY_PARAMS;
        return;
    }

    // Try to read the parameters data.
    try
    {
        BinarySerializer::fastDeserialization(request.params.get(), request.params_size, loc);
    }
    catch(...)
    {
        reply.server_result = OperationResult::BAD_PARAMETERS;
        return;
    }

    // Now we will process the command in the controller.
    ctrl_err = this->invokeCallback<ClbkT>(request, reply, loc);

    // Serialize parameters if all ok.
    if(reply.server_result == OperationResult::COMMAND_OK)
        reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err);
}*/

template <typename ClbkT>
void AmelasControllerServer::processSetAbsRelAltAzMotion(const CommandRequest &request, CommandReply &reply)
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
    ctrl_err = this->invokeCallback<ClbkT>(request, reply, pos, vel);

    // Serialize parameters if all ok.
    if(reply.server_result == OperationResult::COMMAND_OK)
        reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err);
}

template <typename ClbkT>
void AmelasControllerServer::processSetBool(const CommandRequest& request, CommandReply& reply)
{
    // Auxiliar variables and containers.
    controller::AmelasError ctrl_err;

    bool enable;

    // Check the request parameters size.
    if (request.params_size == 0 || !request.params)
    {
        reply.server_result = OperationResult::EMPTY_PARAMS;
        return;
    }

    // Try to read the parameters data.
    try
    {
        BinarySerializer::fastDeserialization(request.params.get(), request.params_size, enable);
    }
    catch(...)
    {
        reply.server_result = OperationResult::BAD_PARAMETERS;
        return;
    }

    // Now we will process the command in the controller.
    ctrl_err = this->invokeCallback<ClbkT>(request, reply, enable);

    // Serialize parameters if all ok.
    if(reply.server_result == OperationResult::COMMAND_OK)
        reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err);
}

template <typename ClbkT>
void AmelasControllerServer::processSetUShort(const CommandRequest& request, CommandReply& reply)
{
    // Auxiliar variables and containers.
    controller::AmelasError ctrl_err;

    unsigned short int enable;

    // Check the request parameters size.
    if (request.params_size == 0 || !request.params)
    {
        reply.server_result = OperationResult::EMPTY_PARAMS;
        return;
    }

    // Try to read the parameters data.
    try
    {
        BinarySerializer::fastDeserialization(request.params.get(), request.params_size, enable);
    }
    catch(...)
    {
        reply.server_result = OperationResult::BAD_PARAMETERS;
        return;
    }

    // Now we will process the command in the controller.
    ctrl_err = this->invokeCallback<ClbkT>(request, reply, enable);

    // Serialize parameters if all ok.
    if(reply.server_result == OperationResult::COMMAND_OK)
        reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err);
}

template <typename ClbkT>
void AmelasControllerServer::processSetDouble(const CommandRequest& request, CommandReply& reply)
{
    // Auxiliar variables and containers.
    controller::AmelasError ctrl_err;

    double doubleValue;

    // Check the request parameters size.
    if (request.params_size == 0 || !request.params)
    {
        reply.server_result = OperationResult::EMPTY_PARAMS;
        return;
    }

    // Try to read the parameters data.
    try
    {
        BinarySerializer::fastDeserialization(request.params.get(), request.params_size, doubleValue);
    }
    catch(...)
    {
        reply.server_result = OperationResult::BAD_PARAMETERS;
        return;
    }

    // Now we will process the command in the controller.
    ctrl_err = this->invokeCallback<ClbkT>(request, reply, doubleValue);

    // Serialize parameters if all ok.
    if(reply.server_result == OperationResult::COMMAND_OK)
        reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err);
}

template <typename ClbkT>
void AmelasControllerServer::processGetDouble(const CommandRequest &request, CommandReply &reply)
{
    // Auxiliar variables and containers.
    controller::AmelasError ctrl_err;
    double doubleValue;

    // Now we will process the command in the controller.
    ctrl_err = this->invokeCallback<ClbkT>(request, reply, doubleValue);

    // Serialize parameters if all ok.
    if(reply.server_result == OperationResult::COMMAND_OK)
        reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err, doubleValue);
}

template <typename ClbkT>
void AmelasControllerServer::processGetUShort(const CommandRequest &request, CommandReply &reply)
{
    // Auxiliar variables and containers.
    controller::AmelasError ctrl_err;
    unsigned short int doubleValue;

    // Now we will process the command in the controller.
    ctrl_err = this->invokeCallback<ClbkT>(request, reply, doubleValue);

    // Serialize parameters if all ok.
    if(reply.server_result == OperationResult::COMMAND_OK)
        reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err, doubleValue);
}

/*template <typename ClbkT>
void AmelasControllerServer::processSetMeteoData(const CommandRequest& request, CommandReply& reply)
{
    // Auxiliar variables and containers.
    controller::AmelasError ctrl_err;

    // Position struct.
    controller::MeteoData meteo;

    // Check the request parameters size.
    if (request.params_size == 0 || !request.params)
    {
        reply.server_result = OperationResult::EMPTY_PARAMS;
        return;
    }

    // Try to read the parameters data.
    try
    {
        BinarySerializer::fastDeserialization(request.params.get(), request.params_size, meteo);
    }
    catch(...)
    {
        reply.server_result = OperationResult::BAD_PARAMETERS;
        return;
    }

    // Now we will process the command in the controller.
    ctrl_err = this->invokeCallback<ClbkT>(request, reply, meteo);

    // Serialize parameters if all ok.
    if(reply.server_result == OperationResult::COMMAND_OK)
        reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err);
}*/

template <typename ClbkT>
void AmelasControllerServer::processGetMeteoData(const CommandRequest &request, CommandReply &reply)
{
    // Auxiliar variables and containers.
    controller::AmelasError ctrl_err;
    controller::MeteoData meteo;

    // Now we will process the command in the controller.
    ctrl_err = this->invokeCallback<ClbkT>(request, reply, meteo);

    // Serialize parameters if all ok.
    if(reply.server_result == OperationResult::COMMAND_OK)
        reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err, meteo.press, meteo.temp, meteo.hr);
}

template <typename ClbkT>
void AmelasControllerServer::processGetLocation(const CommandRequest &request, CommandReply &reply)
{
    // Auxiliar variables and containers.
    controller::AmelasError ctrl_err;
    controller::StationLocation loc;

    // Now we will process the command in the controller.
    ctrl_err = this->invokeCallback<ClbkT>(request, reply, loc);

    // Serialize parameters if all ok.
    if(reply.server_result == OperationResult::COMMAND_OK)
        reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err, loc.wgs84.lat, loc.wgs84.lon, loc.wgs84.alt, loc.ecef.x, loc.ecef.y, loc.ecef.z);
}

template <typename ClbkT>
void AmelasControllerServer::processGetPLCRegister(const CommandRequest& request, CommandReply& reply)
{
    // Auxiliar variables and containers.
    controller::AmelasError ctrl_err;

    // Position struct.
    controller::PLCAddress add;
    controller::PLCRegisterValue pos;

    // Check the request parameters size.
    if (request.params_size == 0 || !request.params)
    {
        reply.server_result = OperationResult::EMPTY_PARAMS;
        return;
    }

    // Try to read the parameters data.
    try
    {
        BinarySerializer::fastDeserialization(request.params.get(), request.params_size, add, pos);
    }
    catch(...)
    {
        reply.server_result = OperationResult::BAD_PARAMETERS;
        return;
    }

    // Now we will process the command in the controller.
    ctrl_err = this->invokeCallback<ClbkT>(request, reply, add, pos);

    // Serialize parameters if all ok.
    if(reply.server_result == OperationResult::COMMAND_OK)
        reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err, add.symbol, add.type, pos.symbol, pos.type, pos.value);
}

template <typename ClbkT>
void AmelasControllerServer::processSetNTPserver(const CommandRequest& request, CommandReply& reply)
{
    // Auxiliar variables and containers.
    controller::AmelasError ctrl_err;

    std::string server;
    unsigned port;

    // Check the request parameters size.
    if (request.params_size == 0 || !request.params)
    {
        reply.server_result = OperationResult::EMPTY_PARAMS;
        return;
    }

    // Try to read the parameters data.
    try
    {
        BinarySerializer::fastDeserialization(request.params.get(), request.params_size, server, port);
    }
    catch(...)
    {
        reply.server_result = OperationResult::BAD_PARAMETERS;
        return;
    }

    // Now we will process the command in the controller.
    ctrl_err = this->invokeCallback<ClbkT>(request, reply, server, port);

    // Serialize parameters if all ok.
    if(reply.server_result == OperationResult::COMMAND_OK)
        reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err);
}

template <typename ClbkT>
void AmelasControllerServer::processGetPLCprueba(const CommandRequest &request, CommandReply &reply)
{
    
    // Auxiliar variables and containers.
    controller::AmelasError ctrl_err;

    std::string name, type;

    // Check the request parameters size.
    if (request.params_size == 0 || !request.params)
    {
        reply.server_result = OperationResult::EMPTY_PARAMS;
        return;
    }

    // Try to read the parameters data.
    try
    {
        BinarySerializer::fastDeserialization(request.params.get(), request.params_size, name, type);
    }
    catch(...)
    {
        reply.server_result = OperationResult::BAD_PARAMETERS;
        return;
    }

    // Now we will process the command in the controller.
    ctrl_err = this->invokeCallback<ClbkT>(request, reply, name, type);

    // Serialize parameters if all ok.
    if(reply.server_result == OperationResult::COMMAND_OK)
        reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err);
}

template <typename ClbkT>
void AmelasControllerServer::processSetLocation(const CommandRequest& request, CommandReply& reply)
{
    // Auxiliar variables and containers.
    controller::AmelasError ctrl_err;

    double lat, lon, alt, x, y, z;

    // Check the request parameters size.
    if (request.params_size == 0 || !request.params)
    {
        reply.server_result = OperationResult::EMPTY_PARAMS;
        return;
    }

    // Try to read the parameters data.
    try
    {
        BinarySerializer::fastDeserialization(request.params.get(), request.params_size, lat, lon, alt, x, y, z);
    }
    catch(...)
    {
        reply.server_result = OperationResult::BAD_PARAMETERS;
        return;
    }

    // Now we will process the command in the controller.
    ctrl_err = this->invokeCallback<ClbkT>(request, reply, lat, lon, alt, x, y, z);

    // Serialize parameters if all ok.
    if(reply.server_result == OperationResult::COMMAND_OK)
        reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err);
}

template <typename ClbkT>
void AmelasControllerServer::processSetMeteoData(const CommandRequest &request, CommandReply &reply)
{
    // Auxiliar variables and containers.
    controller::AmelasError ctrl_err;

    double press, temp, hr;

    // Check the request parameters size.
    if (request.params_size == 0 || !request.params)
    {
        reply.server_result = OperationResult::EMPTY_PARAMS;
        return;
    }

    // Try to read the parameters data.
    try
    {
        BinarySerializer::fastDeserialization(request.params.get(), request.params_size, press, temp, hr);
    }
    catch(...)
    {
        reply.server_result = OperationResult::BAD_PARAMETERS;
        return;
    }

    // Now we will process the command in the controller.
    ctrl_err = this->invokeCallback<ClbkT>(request, reply, press, temp, hr);

    // Serialize parameters if all ok.
    if(reply.server_result == OperationResult::COMMAND_OK)
        reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err);
}

template <typename ClbkT>
void AmelasControllerServer::processSetString(const CommandRequest& request, CommandReply& reply)
{
    // Auxiliar variables and containers.
    controller::AmelasError ctrl_err;

    std::string strVal;

    // Check the request parameters size.
    if (request.params_size == 0 || !request.params)
    {
        reply.server_result = OperationResult::EMPTY_PARAMS;
        return;
    }

    // Try to read the parameters data.
    try
    {
        BinarySerializer::fastDeserialization(request.params.get(), request.params_size, strVal);
    }
    catch(...)
    {
        reply.server_result = OperationResult::BAD_PARAMETERS;
        return;
    }

    // Now we will process the command in the controller.
    ctrl_err = this->invokeCallback<ClbkT>(request, reply, strVal);

    // Serialize parameters if all ok.
    if(reply.server_result == OperationResult::COMMAND_OK)
        reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err);
}

template <typename ClbkT>
void AmelasControllerServer::processGetSixDoubles(const CommandRequest &request, CommandReply &reply)
{
    // Auxiliar variables and containers.
    controller::AmelasError ctrl_err;
    double val1, val2, val3, val4, val5, val6;

    // Now we will process the command in the controller.
    ctrl_err = this->invokeCallback<ClbkT>(request, reply, val1, val2, val3, val4, val5, val6);

    // Serialize parameters if all ok.
    if(reply.server_result == OperationResult::COMMAND_OK)
        reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err, val1, val2, val3, val4, val5, val6);
}

template <typename ClbkT>
void AmelasControllerServer::processSetSixBool(const CommandRequest& request, CommandReply& reply)
{
    // Auxiliar variables and containers.
    controller::AmelasError ctrl_err;

    bool bo1, bo2, bo3, bo4, bo5, bo6;

    // Check the request parameters size.
    if (request.params_size == 0 || !request.params)
    {
        reply.server_result = OperationResult::EMPTY_PARAMS;
        return;
    }

    // Try to read the parameters data.
    try
    {
        BinarySerializer::fastDeserialization(request.params.get(), request.params_size, bo1, bo2, bo3, bo4, bo5, bo6);
    }
    catch(...)
    {
        reply.server_result = OperationResult::BAD_PARAMETERS;
        return;
    }

    // Now we will process the command in the controller.
    ctrl_err = this->invokeCallback<ClbkT>(request, reply, bo1, bo2, bo3, bo4, bo5, bo6);

    // Serialize parameters if all ok.
    if(reply.server_result == OperationResult::COMMAND_OK)
        reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err);
}

template <typename ClbkT>
void AmelasControllerServer::processGetString(const CommandRequest &request, CommandReply &reply)
{
    // Auxiliar variables and containers.
    controller::AmelasError ctrl_err;
    std::string strValue;

    // Now we will process the command in the controller.
    ctrl_err = this->invokeCallback<ClbkT>(request, reply, strValue);

    // Serialize parameters if all ok.
    if(reply.server_result == OperationResult::COMMAND_OK)
        reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err, strValue);
}

#endif