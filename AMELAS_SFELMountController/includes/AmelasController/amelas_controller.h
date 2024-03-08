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


    // SAFETY RELATED FUNCTIONS
    //=====================================================================================================================
    LIBAMELAS_EXPORT AmelasError doResetState();
    LIBAMELAS_EXPORT AmelasError enableAvoidSun(const bool& enabled);
    //=====================================================================================================================


    // LOW LEVEL PLC REGISTERS RELATED FUNCTIONS
    //=====================================================================================================================
    LIBAMELAS_EXPORT AmelasError doConnectPLC();
    LIBAMELAS_EXPORT AmelasError doDisconnectPLC();

    LIBAMELAS_EXPORT PLCRegisterValue getPLCregisterValue(const std::string symbol, const std::string type);
    LIBAMELAS_EXPORT AmelasError getPLCregister(const PLCAddress& address, PLCRegisterValue& registerValue);
    LIBAMELAS_EXPORT AmelasError getPLCprueba(const std::string& symbol, const std::string& type);
    //=====================================================================================================================


    // STATUS AND CONFIGURATION RELATED FUNCTIONS
    //=====================================================================================================================
    LIBAMELAS_EXPORT AmelasError getMountLog(const std::string &day);

    // TODO: LIBAMELAS_EXPORT AmelasError doSyncTimeNTP(const std::string& host, const unsigned& port, const unsigned& timeout);
    LIBAMELAS_EXPORT AmelasError doSyncTimeNTP(const unsigned& port);
    // TODO: LIBAMELAS_EXPORT AmelasError doSyncTimeManual(const std::string& datetime);

    // TODO: LIBAMELAS_EXPORT AmelasError getMountStatus();
    // TODO: LIBAMELAS_EXPORT AmelasError getDeviceInfo();

    LIBAMELAS_EXPORT AmelasError getDatetime(std::string&);

    LIBAMELAS_EXPORT AmelasError enableTrackingAdjusts(const bool& enabled);
    LIBAMELAS_EXPORT AmelasError enableMountPower(const bool& enabled);

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
    
    LIBAMELAS_EXPORT AmelasError setWaitAlt(const double& alt);
    LIBAMELAS_EXPORT AmelasError getWaitAlt(double& alt);

    LIBAMELAS_EXPORT AmelasError setHomingOffsets(const AltAzAdj& pos);
    LIBAMELAS_EXPORT AmelasError getHomingOffsets(AltAzAdj& pos);

    LIBAMELAS_EXPORT AmelasError enableMountModel(const bool& enabled);
    // TODO: LIBAMELAS_EXPORT AmelasError setMountModelCoefs(const MountModelCoefs& coefs);
    // TODO: LIBAMELAS_EXPORT AmelasError getMountModelCoefs(MountModelCoefs& coefs);

    //LIBAMELAS_EXPORT AmelasError setLocation(const StationLocation& location); // TODO
    LIBAMELAS_EXPORT AmelasError setLocation(const double& lat, const double& lon, const double& alt, const double& x, const double& y, const double& z);
    LIBAMELAS_EXPORT AmelasError getLocation(StationLocation& location);

    //LIBAMELAS_EXPORT AmelasError setMeteoData(const MeteoData& meteo); // TODO
    LIBAMELAS_EXPORT AmelasError setMeteoData(const double& press, const double& temp, const double& hr);
    LIBAMELAS_EXPORT AmelasError getMeteoData(MeteoData& meteo);
    
    // TODO: LIBAMELAS_EXPORT AmelasError enableSimulationMode(const bool& enabled);
    // TODO: LIBAMELAS_EXPORT AmelasError getSimulationState(bool& enabled);
    // TODO: LIBAMELAS_EXPORT AmelasError setSimulationTime(const std::string& datetime);
    // =====================================================================================================================


    // MOTION RELATED FUNCTIONS
    //=====================================================================================================================
    LIBAMELAS_EXPORT AmelasError getMotionMode(AmelasMotionMode& motion_mode);
    LIBAMELAS_EXPORT AmelasError getMotionState(AmelasMotionState& motion_state, AltAzPos &pos);

    /**
     * @brief   Starts the motion.
     * @return  AmelasError
     * @warning If the mount does not have any movement loaded, the function will do nothing and a warning will occur.
     */
    LIBAMELAS_EXPORT AmelasError doStartMotion();
    /**
     * @brief   Pauses the motion.
     * @return  AmelasError
     * @warning If the mount is not moving, the function will do nothing and a warning will occur.
     */
    LIBAMELAS_EXPORT AmelasError doPauseMotion();
    /**
     * @brief   Stops the motion.
     * @return  AmelasError
     * @warning If the mount is not moving, the function will do nothing and a warning will occur.
     */
    LIBAMELAS_EXPORT AmelasError doStopMotion();

    LIBAMELAS_EXPORT AmelasError setTrackPosOffset(const AltAzAdj& pos);
    LIBAMELAS_EXPORT AmelasError getTrackPosOffset(AltAzAdj& pos);
    
    LIBAMELAS_EXPORT AmelasError setTrackTimeBias(const double& time);
    LIBAMELAS_EXPORT AmelasError getTrackTimeBias(double& time);

    LIBAMELAS_EXPORT AmelasError setAbsoluteAltAzMotion(const AltAzPos& pos, const AltAzVel& vel);
    LIBAMELAS_EXPORT AmelasError setRelativeAltAzMotion(const AltAzPos& pos, const AltAzVel& vel);
    LIBAMELAS_EXPORT AmelasError setContAltAzMotion(const AltAzVel &vel);

    // TODO: LIBAMELAS_EXPORT AmelasError setHomingMotion();
    LIBAMELAS_EXPORT AmelasError setIdleMotion();
    LIBAMELAS_EXPORT AmelasError setParkMotion();
    LIBAMELAS_EXPORT AmelasError setCalibrationMotion();

    LIBAMELAS_EXPORT AmelasError pruebaBucles();

    // TODO: LIBAMELAS_EXPORT AmelasError setCPFMotion(const file& cpf, AmelasTracking& tracking);
    // TODO: LIBAMELAS_EXPORT AmelasError setStarMotion(const StarData& star_data);
    //=====================================================================================================================


private:
    AmelasError setEnable(const bool& enable, const std::string plcSymbol, const std::string command);
    AmelasError setPosition(const AltAzPos& pos, const std::string plcSymbol, const std::string command);
    AmelasError getPosition(AltAzPos& pos, const std::string plcSymbol, const std::string command);
    AmelasError setSpeed(const AltAzVel& vel, const std::string plcSymbol, const std::string command);
    AmelasError getSpeed(AltAzVel& vel, const std::string plcSymbol, const std::string command);
    AmelasMotionMode motionModes();
    AmelasMotionState motionStates();
    void setLog(const std::string command, const std::string specific, const AmelasError error);

    const AmelasControllerConfig _config;
    const std::shared_ptr<spdlog::logger> _logger;
    std::shared_ptr<AmelasAdsClient> _plc;
    bool _avoid_sun = false;
};

}} // END NAMESPACES.
// =====================================================================================================================
