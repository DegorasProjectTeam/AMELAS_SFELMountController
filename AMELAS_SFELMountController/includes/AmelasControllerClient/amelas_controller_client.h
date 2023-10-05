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
 * @file amelas_controller_client.h
 * @brief This file contains the declaration of the AmelasControllerClient class.
 * @author Degoras Project Team
 * @author AVS AMELAS Team
 * @copyright EUPL License
 * @version 2309.5
***********************************************************************************************************************/

// =====================================================================================================================
#pragma once
// =====================================================================================================================

// C++ INCLUDES
// =====================================================================================================================
#include <string>
// =====================================================================================================================

// ZMQUTILS INCLUDES
// =====================================================================================================================
#include <LibZMQUtils/CommandClient>
#include <LibZMQUtils/Utils>
// =====================================================================================================================

// AMELAS INCLUDES
// =====================================================================================================================
#include "libamelas_global.h"
// =====================================================================================================================

// PROJECT INCLUDES
// =====================================================================================================================
// =====================================================================================================================

// AMELAS NAMESPACES
// =====================================================================================================================
namespace amelas{
namespace communication{
// =====================================================================================================================

using zmqutils::common::RequestData;
using zmqutils::common::CommandReply;

class AmelasControllerClient : public zmqutils::CommandClientBase
{
public:

     LIBAMELAS_EXPORT AmelasControllerClient(const std::string& server_endpoint,
                              const std::string& client_name = "",
                              const std::string interf_name = "");

    // TODO
    //virtual void prepareRequest() = 0;


//     LIBAMELAS_EXPORT  doSetHomePosition()
//     {

//     }







    LIBAMELAS_EXPORT ~AmelasControllerClient() override;

protected:

    LIBAMELAS_EXPORT virtual void onClientStart() override;

    LIBAMELAS_EXPORT virtual void onClientStop() override;

    LIBAMELAS_EXPORT virtual void onWaitingReply() override;

    LIBAMELAS_EXPORT virtual void onDeadServer() override;

    LIBAMELAS_EXPORT virtual void onConnected() override;

    LIBAMELAS_EXPORT virtual void onDisconnected() override;

    LIBAMELAS_EXPORT virtual void onInvalidMsgReceived(const CommandReply&) override;

    LIBAMELAS_EXPORT virtual void onReplyReceived(const CommandReply& reply) override;

    LIBAMELAS_EXPORT virtual void onSendingCommand(const RequestData&) override;

    LIBAMELAS_EXPORT virtual void onClientError(const zmq::error_t&, const std::string& ext_info) override;
};

}} // END NAMESPACES.
// =====================================================================================================================
