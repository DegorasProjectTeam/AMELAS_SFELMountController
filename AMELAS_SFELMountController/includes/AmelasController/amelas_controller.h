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

    // STATUS AND CONFIGURATION RELATED FUNCTIONS
    //=====================================================================================================================
    LIBAMELAS_EXPORT AmelasError getDatetime(std::string&);

    LIBAMELAS_EXPORT AmelasError enableTrackingAdjusts(const bool& enable);
    LIBAMELAS_EXPORT AmelasError enableMountPower(const bool& enable);

    LIBAMELAS_EXPORT AmelasError setSlewSpeed(const AltAzVel& vel);
    LIBAMELAS_EXPORT AmelasError getSlewSpeed(AltAzVel& vel);

    LIBAMELAS_EXPORT AmelasError setHomePosition(const AltAzPos& pos);
    LIBAMELAS_EXPORT AmelasError getHomePosition(AltAzPos& pos);

    LIBAMELAS_EXPORT AmelasError setIdlePosition(const AltAzPos& pos);
    LIBAMELAS_EXPORT AmelasError getIdlePosition(AltAzPos& pos);

    LIBAMELAS_EXPORT AmelasError setParkPosition(const AltAzPos& pos);
    LIBAMELAS_EXPORT AmelasError getParkPosition(AltAzPos& pos);

    LIBAMELAS_EXPORT AmelasError setCalibrationPosition(const AltAzPos& pos);
    LIBAMELAS_EXPORT AmelasError getCalibrationPosition(AltAzPos& pos);

    LIBAMELAS_EXPORT AmelasError setIdlePositionHere();
    LIBAMELAS_EXPORT AmelasError setParkPositionHere();
    LIBAMELAS_EXPORT AmelasError setCalibrationPositionHere();

    LIBAMELAS_EXPORT AmelasError setHomingOffsets(const AltAzAdj& pos);
    LIBAMELAS_EXPORT AmelasError getHomingOffsets(AltAzAdj& pos);
    
    LIBAMELAS_EXPORT AmelasError setWaitAlt(const double& alt);
    LIBAMELAS_EXPORT AmelasError getWaitAlt(double& alt);

    LIBAMELAS_EXPORT AmelasError enableMountModel(const bool& enable);

    LIBAMELAS_EXPORT AmelasError setMeteoData(const MeteoData& meteo);
    LIBAMELAS_EXPORT AmelasError getMeteoData(MeteoData& meteo);
    // =====================================================================================================================

    // MOTION RELATED FUNCTIONS
    //=====================================================================================================================
    LIBAMELAS_EXPORT AmelasError doStartMotion();
    LIBAMELAS_EXPORT AmelasError doPauseMotion();
    LIBAMELAS_EXPORT AmelasError doStopMotion();

    LIBAMELAS_EXPORT AmelasError setTrackPosOffset(const AltAzAdj& pos);
    LIBAMELAS_EXPORT AmelasError getTrackPosOffset(AltAzAdj& pos);
    
    LIBAMELAS_EXPORT AmelasError setTrackTimeBias(const double& time);
    LIBAMELAS_EXPORT AmelasError getTrackTimeBias(double& time);

    LIBAMELAS_EXPORT AmelasError setAbsoluteAltAzMotion(const AltAzPos& pos, const AltAzVel& vel);
    LIBAMELAS_EXPORT AmelasError setRelativeAltAzMotion(const AltAzPos& pos, const AltAzVel& vel);
    //=====================================================================================================================

private:
    AmelasError setEnable(const bool& enable, const std::string plcSymbol, const std::string command);
    AmelasError setPosition(const AltAzPos& pos, const std::string plcSymbol, const std::string command);
    AmelasError getPosition(AltAzPos& pos, const std::string plcSymbol, const std::string command);
    AmelasError setSpeed(const AltAzVel& vel, const std::string plcSymbol, const std::string command);
    AmelasError getSpeed(AltAzVel& vel, const std::string plcSymbol, const std::string command);
    void setLog(const std::string command, const std::string specific, const AmelasError error);

    const AmelasControllerConfig _config;
    const std::shared_ptr<spdlog::logger> _logger;
    std::shared_ptr<AmelasAdsClient> _plc;

    bool tracking_adjusts_;
    bool mount_power_;
    AltAzVel slew_speed_;
    AltAzPos home_pos_;
    AltAzPos idle_pos_;
    AltAzPos park_pos_;
    AltAzPos calibration_pos_;
    AltAzAdj home_pos_offset_;
    double wait_alt_;
    double mount_model_;
    MeteoData meteo_;
    AltAzAdj track_pos_offset_;
    double track_time_bias_;

};

}} // END NAMESPACES.
// =====================================================================================================================
