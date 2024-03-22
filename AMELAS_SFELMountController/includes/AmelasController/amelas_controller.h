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
#include "LibDegorasSLR/Timing/types/datetime_types.h"
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

    // TODO: LIBAMELAS_EXPORT AmelasError getPLCregisters(std::string& plcRegisters);
    LIBAMELAS_EXPORT AmelasError getPLCregister(const PLCAddress& address, PLCRegisterValue& registerValue); // TODO (funciona la de abajo sin estructura)
    LIBAMELAS_EXPORT AmelasError getPLCprueba(const std::string& symbol, const std::string& type); // PRUEBA (hay que cambiar por sus estructuras)
    //=====================================================================================================================


    // STATUS AND CONFIGURATION RELATED FUNCTIONS
    //=====================================================================================================================
    LIBAMELAS_EXPORT AmelasError getMountLog(const std::string &day);
    LIBAMELAS_EXPORT AmelasError getMountStatus(std::string& mountStatus);
    LIBAMELAS_EXPORT AmelasError getDeviceInfo(std::string& deviceInfo);

    LIBAMELAS_EXPORT AmelasError setTimeSource(const unsigned short int& clock);
    LIBAMELAS_EXPORT AmelasError getTimeSource(unsigned short int& clock);
    LIBAMELAS_EXPORT AmelasError doSyncTimeManual(const std::string& datetime);
    // TODO: LIBAMELAS_EXPORT AmelasError doSyncTimeNTP(const std::string& host = "hora.roa.es", const unsigned& port = 123, const unsigned& timeout);
    LIBAMELAS_EXPORT AmelasError doSyncTimeNTP(const std::string& host = "hora.roa.es", const unsigned& port = 123); // PRUEBA (para ver si comunico con el objeto NTP de TwinCAT)
    LIBAMELAS_EXPORT AmelasError enableExtPPS(const bool& enabled);

    LIBAMELAS_EXPORT AmelasError enableTrackingAdjusts(const bool& enabled);
    LIBAMELAS_EXPORT AmelasError enableMountPower(const bool& enabled);

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
    //LIBAMELAS_EXPORT AmelasError setMountModelCoefs(const MountModelCoefs& coefs); // TODO (funciona la de abajo sin estructura)
    LIBAMELAS_EXPORT AmelasError setMountModelCoefs(const double& an, const double& aw, const double& ca, const double& npae, const double& ie, const double& ia); // PRUEBA (hay que cambiar por su estructura)
    LIBAMELAS_EXPORT AmelasError setMountModelCoefsFile(const std::string &fileData); // PRUEBA (no hay definida ninguna API que sea por fichero, pero es cómodo para pruebas)
    //LIBAMELAS_EXPORT AmelasError getMountModelCoefs(MountModelCoefs& coefs); // TODO (funciona la de abajo sin estructura)
    LIBAMELAS_EXPORT AmelasError getMountModelCoefs(double& an, double& aw, double& ca, double& npae, double& ie, double& ia); // PRUEBA (hay que cambiar por su estructura)
    LIBAMELAS_EXPORT AmelasError applyMountModelCorrections(const bool& bAN, const bool& bAW, const bool& bCA, const bool& bNPAE, const bool& bIE, const bool& bIA); // PRUEBA (no hay definida ninguna API, pero se comprueba que funcione)

    //LIBAMELAS_EXPORT AmelasError setLocation(const StationLocation& location); // TODO (funciona la de abajo sin estructura)
    LIBAMELAS_EXPORT AmelasError setLocation(const double& lat, const double& lon, const double& alt, const double& x, const double& y, const double& z); // PRUEBA (hay que cambiar por su estructura)
    LIBAMELAS_EXPORT AmelasError getLocation(StationLocation& location);

    //LIBAMELAS_EXPORT AmelasError setMeteoData(const MeteoData& meteo); // TODO (funciona la de abajo sin estructura)
    LIBAMELAS_EXPORT AmelasError setMeteoData(const double& press, const double& temp, const double& hr); // PRUEBA (hay que cambiar por su estructura)
    LIBAMELAS_EXPORT AmelasError getMeteoData(MeteoData& meteo);

    LIBAMELAS_EXPORT AmelasError setSlewSpeed(const AltAzVel& vel);
    LIBAMELAS_EXPORT AmelasError getSlewSpeed(AltAzVel& vel);
    // =====================================================================================================================


    // MOTION RELATED FUNCTIONS
    //=====================================================================================================================
    LIBAMELAS_EXPORT AmelasError getMotionMode(AmelasMotionMode& motion_mode);
    LIBAMELAS_EXPORT AmelasError getMotionState(AmelasMotionState& motion_state, AltAzPos &pos);

    LIBAMELAS_EXPORT AmelasError doStartMotion();
    LIBAMELAS_EXPORT AmelasError doPauseMotion();
    LIBAMELAS_EXPORT AmelasError doStopMotion();

    LIBAMELAS_EXPORT AmelasError setTrackPosOffset(const AltAzAdj& pos);
    LIBAMELAS_EXPORT AmelasError getTrackPosOffset(AltAzAdj& pos);
    
    LIBAMELAS_EXPORT AmelasError setTrackTimeBias(const double& time);
    LIBAMELAS_EXPORT AmelasError getTrackTimeBias(double& time);

    LIBAMELAS_EXPORT AmelasError setAbsoluteAltAzMotion(const AltAzPos& pos, const AltAzVel& vel);
    LIBAMELAS_EXPORT AmelasError setRelativeAltAzMotion(const AltAzPos& pos, const AltAzVel& vel);
    LIBAMELAS_EXPORT AmelasError setContAltAzMotion(const AltAzVel &vel);

    LIBAMELAS_EXPORT AmelasError setHomingMotion();
    LIBAMELAS_EXPORT AmelasError setIdleMotion();
    LIBAMELAS_EXPORT AmelasError setParkMotion();
    LIBAMELAS_EXPORT AmelasError setCalibrationMotion();

    // TODO: LIBAMELAS_EXPORT AmelasError setCPFMotion(const file& cpf, AmelasTracking& tracking);
    LIBAMELAS_EXPORT AmelasError setCPFMotion(const unsigned short int& example_selector);
    // TODO: LIBAMELAS_EXPORT AmelasError setStarMotion(const StarData& star_data);
    //=====================================================================================================================


    // OTHER FUNCTIONS
    //=====================================================================================================================
    LIBAMELAS_EXPORT AmelasError getDatetime(std::string&);
    LIBAMELAS_EXPORT AmelasError pruebaBucles(); // PRUEBA (para testear bucles sobre arrays)
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
    double deg_to_radians(const double& degrees);
    double arcsec_to_radians(const double& arcsec);
    double arcsec_to_deg(const double& arcsec);
    unsigned long long iso8601DatetimeTowin32Ticks(const std::string& datetime);
    unsigned long long modifiedJulianDateTimeTowin32Ticks(const dpslr::timing::types::MJDateTime& mjdt);

    const AmelasControllerConfig _config;
    const std::shared_ptr<spdlog::logger> _logger;
    std::shared_ptr<AmelasAdsClient> _plc;

    bool _avoid_sun = false;
    bool _enable_mount_model = false;
    double _ie_tpoint = 0.0;
    double _ia_tpoint = 0.0;
    double _ca_tpoint = 0.0;
    double _an_tpoint = 0.0;
    double _aw_tpoint = 0.0;
    double _npae_tpoint = 0.0;
    double _elOffset = 0.0;
    double _azOffset = 0.0;
    unsigned short int _clockSource = 0;
};

}} // END NAMESPACES.
// =====================================================================================================================
