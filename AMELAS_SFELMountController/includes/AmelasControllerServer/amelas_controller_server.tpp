#ifndef AMELAS_CONTROLLER_SERVER_TPP_
#define AMELAS_CONTROLLER_SERVER_TPP_

template <typename ClbkT>
void AmelasControllerServer::processGetPosition(const CommandRequest &request, CommandReply &reply)
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
void AmelasControllerServer::processSetPosition(const CommandRequest& request, CommandReply& reply)
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

// template <typename ClbkT>
// void AmelasControllerServer::processSetAbsRelAltAzMotion(const CommandRequest& request, CommandReply& reply)
// {
//     // Auxiliar variables and containers.
//     controller::AmelasError ctrl_err;
// 
//     // Position struct.
//     controller::AltAzPos pos;
// 
//     // Check the request parameters size.
//     if (request.params_size == 0 || !request.params)
//     {
//         reply.server_result = OperationResult::EMPTY_PARAMS;
//         return;
//     }
// 
//     // Try to read the parameters data.
//     try
//     {
//         BinarySerializer::fastDeserialization(request.params.get(), request.params_size, pos);
//     }
//     catch(...)
//     {
//         reply.server_result = OperationResult::BAD_PARAMETERS;
//         return;
//     }
// 
//     // Now we will process the command in the controller.
//     ctrl_err = this->invokeCallback<ClbkT>(request, reply, pos);
// 
//     // Serialize parameters if all ok.
//     if(reply.server_result == OperationResult::COMMAND_OK)
//         reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err);
// }

#endif