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
 * @file amelas_controller.h
 * @brief This file contains the declaration of the AmelasController class.
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
#include <map>
#include <string>
// =====================================================================================================================

// ZMQUTILS INCLUDES
// =====================================================================================================================
#include <LibZMQUtils/CommandServer>
#include <LibZMQUtils/Utils>
// =====================================================================================================================

// PROJECT INCLUDES
// =====================================================================================================================
#include "common.h"
#include "libamelas_global.h"
// =====================================================================================================================

// AMELAS NAMESPACES
// =====================================================================================================================
namespace amelas{
namespace controller{
// =====================================================================================================================

class AmelasController
{
public:

    LIBAMELAS_EXPORT AmelasController();

    LIBAMELAS_EXPORT AmelasError setHomePosition(const AltAzPos& pos);

    LIBAMELAS_EXPORT AmelasError getHomePosition(AltAzPos& pos);

    LIBAMELAS_EXPORT AmelasError getDatetime(std::string&);

private:

    AltAzPos home_pos_;

};

}} // END NAMESPACES.
// =====================================================================================================================
