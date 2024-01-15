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

// SPDLOG INCLUDES
// =====================================================================================================================
#include "spdlog/spdlog.h"
// =====================================================================================================================

// JSON INCLUDES
// =====================================================================================================================
#include "nlohmann/json.hpp"
// =====================================================================================================================

// PROJECT INCLUDES
// =====================================================================================================================
#include "common.h"
#include "libamelas_global.h"
#include "AmelasAdsClient/amelas_ads_client.h"
// =====================================================================================================================

// AMELAS NAMESPACES
// =====================================================================================================================
namespace amelas{
namespace controller{
// =====================================================================================================================

struct AmelasControllerConfig
{
    amelas::AmelasAdsClientConfig plcConfig;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AmelasControllerConfig, plcConfig)

class AmelasController
{
public:

    LIBAMELAS_EXPORT AmelasController(const AmelasControllerConfig &config, 
                                        const std::shared_ptr<spdlog::logger> logger);

    LIBAMELAS_EXPORT AmelasError setHomePosition(const AltAzPos& pos);
    LIBAMELAS_EXPORT AmelasError getHomePosition(AltAzPos& pos);

    LIBAMELAS_EXPORT AmelasError getDatetime(std::string&);

    LIBAMELAS_EXPORT AmelasError setIdlePosition(const AltAzPos& pos);
    LIBAMELAS_EXPORT AmelasError getIdlePosition(AltAzPos& pos);

    LIBAMELAS_EXPORT AmelasError setParkPosition(const AltAzPos& pos);
    LIBAMELAS_EXPORT AmelasError getParkPosition(AltAzPos& pos);

    LIBAMELAS_EXPORT AmelasError setCalibrationPosition(const AltAzPos& pos);
    LIBAMELAS_EXPORT AmelasError getCalibrationPosition(AltAzPos& pos);

    LIBAMELAS_EXPORT AmelasError setIdlePositionHere();
    LIBAMELAS_EXPORT AmelasError setParkPositionHere();
    LIBAMELAS_EXPORT AmelasError setCalibrationPositionHere();

private:
    const AmelasControllerConfig _config;
    const std::shared_ptr<spdlog::logger> _logger;
    std::shared_ptr<AmelasAdsClient> _plc;

    AltAzPos home_pos_;
    AltAzPos idle_pos_;
    AltAzPos park_pos_;
    AltAzPos calibration_pos_;

};

}} // END NAMESPACES.
// =====================================================================================================================
