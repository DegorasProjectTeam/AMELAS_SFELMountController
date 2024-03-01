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
 * @file amelas_controller.cpp
 * @brief EXAMPLE FILE - This file contains the implementation of the AmelasController example class.
 * @author Degoras Project Team
 * @author AVS AMELAS Team
 * @copyright EUPL License
 * @version 2309.5
***********************************************************************************************************************/

// PROJECT INCLUDES
// =====================================================================================================================
#include "AmelasController/amelas_controller.h"
// =====================================================================================================================

// AMELAS NAMESPACES
// =====================================================================================================================
namespace amelas{
namespace controller{

AmelasController::AmelasController(const AmelasControllerConfig &config, 
                                    const std::shared_ptr<spdlog::logger> logger) :
    _config(config),
    _logger(logger)
{
    _plc = std::make_shared<amelas::AmelasAdsClient>(_config.plcConfig, _logger); 
}


// SAFETY RELATED FUNCTIONS
//=====================================================================================================================
AmelasError AmelasController::doResetState()
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "DO_RESET_STATE";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.doResetState";

    // Functionality
    _plc->write(symbol, true);

    // Log
    setLog(command, "", error);

    return error;
}

// TODO: AmelasError AmelasController::enableAvoidSun(const bool &enabled)
//=====================================================================================================================


// LOW LEVEL PLC REGISTERS RELATED FUNCTIONS
//=====================================================================================================================
AmelasError AmelasController::doConnectPLC()
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "DO_CONNECT_PLC";

    // Functionality
    _plc->connect();

    // Log
    setLog(command, "", error);

    return error;
}

AmelasError AmelasController::doDisconnectPLC()
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "DO_DISCONNECT_PLC";

    // Functionality
    _plc->disconnect();

    // Log
    setLog(command, "", error);

    return error;
}

// TODO
PLCRegisterValue AmelasController::getPLCregisterValue(const std::string symbol, const std::string type)
{
    PLCRegisterValue registerValue;

    registerValue.symbol = symbol;
    registerValue.type = type;

    if (type == "double")
        registerValue.value = std::to_string(_plc->read<double>(symbol));
    else if (type == "int")
        registerValue.value = std::to_string(_plc->read<short int>(symbol));
    else if (type == "bool")
        registerValue.value = std::to_string(_plc->read<bool>(symbol));

    return registerValue;
}

AmelasError AmelasController::getPLCregister(const PLCAddress &address, PLCRegisterValue &registerValue)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    const std::string command = "GET_PLC_REGISTER";

    /*if (address.type == "double")
        value.value = std::to_string(_plc->read<double>(address.symbol));
    else if (address.type == "int")
        value.value = std::to_string(_plc->read<short int>(address.symbol));
    else if (address.type == "bool")
        value.value = std::to_string(_plc->read<bool>(address.symbol));

    value.symbol = address.symbol;
    value.type = address.type;

    // Log
    std::ostringstream oss;
    oss << "Symbol: " << value.symbol << " <" << value.type << "> = " << value.value << '\n';
    setLog(command, oss.str(), error);*/

    // if (value.type == "double")
    //     value.value = std::to_string(_plc->read<double>(value.symbol));
    // else if (value.type == "int")
    //     value.value = std::to_string(_plc->read<short int>(value.symbol));
    // else if (value.type == "bool")
    //     value.value = std::to_string(_plc->read<bool>(value.symbol));

    registerValue = getPLCregisterValue(address.symbol, address.type);

    // Log
    std::ostringstream oss;
    oss << "Symbol: " << registerValue.symbol << " <" << registerValue.type << "> = " << registerValue.value << '\n';
    setLog(command, oss.str(), error);

    return error;
}

//=====================================================================================================================


// STATUS AND CONFIGURATION RELATED FUNCTIONS
//=====================================================================================================================
AmelasError AmelasController::getDatetime(std::string &)
{
    return AmelasError::SUCCESS;
}

void AmelasController::setLog(const std::string command, const std::string specific, const AmelasError error)
{
    // Adjustment separator lenght according to the error
    std::ostringstream oss_1;
    if (error == AmelasError::SUCCESS)
        oss_1 << std::string(93, '-') << '\n';
    else if (error == AmelasError::UNSAFE_POSITION || error == AmelasError::UNSAFE_SPEED)
        oss_1 << std::string(90, '-') << '\n';
    else
        oss_1 << std::string(92, '-') << '\n';

    // Log message generation
    std::ostringstream oss;
    oss << oss_1.str()
        << "<AMELAS CONTROLLER>" << '\n'
        << "-> " << command << '\n'
        << "Time: " << zmqutils::utils::currentISO8601Date() << '\n'
        << specific
        << "Error: " << static_cast<int>(error) << " (" << ControllerErrorStr[static_cast<size_t>(error)] << ")" << '\n'
        << std::string(100, '-') << '\n';

    // Log output
    if (error == AmelasError::SUCCESS)
        _logger->info(oss.str());
    else if (error == AmelasError::MOUNT_UNSAFE_STATE || error == AmelasError::UNSAFE_POSITION || error == AmelasError::UNSAFE_SPEED || error == AmelasError::STAR_UNSAFE)
        _logger->warn(oss.str());
    else
        _logger->error(oss.str());
}

AmelasError AmelasController::setEnable(const bool &enabled, const std::string plcSymbol, const std::string command)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Variable used for this function
    std::ostringstream oss;

    // Modifying log message
    if (_plc->read<bool>(plcSymbol) == enabled)
    {
        std::string result;
        std::string prefix = "MAIN.commander.enable";
        size_t pos = plcSymbol.find(prefix);

        if (pos != std::string::npos)
            result = plcSymbol.substr(pos + prefix.length());

        oss << result << " is already set to " << enabled << "." << '\n';
    }
    else
    {
        if (command == "EN_TRACK_ADJ")
        {
            oss << "Track adj.: " << enabled << '\n';
        }
        else if (command == "EN_MOUNT_POWER")
        {
            oss << "Mount power: " << enabled << '\n';
        }
        else if (command == "EN_MOUNT_MODEL")
        {
            oss << "Mount model: " << enabled << '\n';
        }

        // Functionality
        _plc->write(plcSymbol, enabled);
    }

    // Log
    setLog(command, oss.str(), error);

    return error;
}

AmelasError AmelasController::setPosition(const AltAzPos &pos, const std::string plcSymbol, const std::string command)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Check the provided values
    if (pos.az > 360.0 ||  pos.az < 0.0 || pos.el >= 90. || pos.el < 0.)
    {
        error = AmelasError::INVALID_POSITION;
    }
    else
    {
        if (pos.az >= 300.0 || pos.el >= 80.)
        {
            error = AmelasError::UNSAFE_POSITION;
        }

        // Functionality
        _plc->write(plcSymbol + ".Azimuth", pos.az);
        _plc->write(plcSymbol + ".Elevation", pos.el);
    }

    // Log
    std::ostringstream oss;
    oss << "Az: " << pos.az << '\n'
        << "El: " << pos.el << '\n';
    setLog(command, oss.str(), error);

    return error;
}

AmelasError AmelasController::getPosition(AltAzPos &pos, const std::string plcSymbol, const std::string command)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Functionality
    pos.az = _plc->read<double>(plcSymbol + ".Azimuth");
    pos.el = _plc->read<double>(plcSymbol + ".Elevation");

    // Log
    setLog(command, "", error);
    
    return error;
}

AmelasError AmelasController::setSpeed(const AltAzVel &vel, const std::string plcSymbol, const std::string command)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Check the provided values
    if (vel.az < 0.0 ||  vel.el < 0.0)
    {
        error = AmelasError::INVALID_SPEED;
    }
    else
    {
        // Functionality
        _plc->write(plcSymbol + ".Azimuth", vel.az);
        _plc->write(plcSymbol + ".Elevation", vel.el);
    }

    // Log
    std::ostringstream oss;
    oss << "Az: " << vel.az << '\n'
        << "El: " << vel.el << '\n';
    setLog(command, oss.str(), error);

    return error;
}

AmelasError AmelasController::getSpeed(AltAzVel &vel, const std::string plcSymbol, const std::string command)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Functionality
    vel.az = _plc->read<double>(plcSymbol + ".Azimuth");
    vel.el = _plc->read<double>(plcSymbol + ".Elevation");

    // Log
    setLog(command, "", error);
    
    return error;
}

// TODO: AmelasError AmelasController::getMountLog(const std::string &start, const std::string &end)
// TODO: AmelasError AmelasController::doSyncTimeNTP(const std::string &host, const unsigned &port, const unsigned &timeout)

// TODO
AmelasError AmelasController::doSyncTimeNTP(const unsigned &port = 123)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "DO_SYNC_NTP";

    // Symbols used for PLC
    const std::string symbolEnable = "Object1.ClientPara.bEnable";
    const std::string symbolHost   = "Object1.ClientPara.sServerName";
    const std::string symbolPort   = "Object1.ClientPara.nServerPort";

    // Functionality
    _plc->write(symbolEnable, true);
    _plc->write(symbolPort, port);

    // Log
    std::ostringstream oss;
    oss << "Server port: " << port << '\n';
    setLog(command, oss.str(), error);

    return error;
}

// TODO: AmelasError AmelasController::doSyncTimeManual(const std::string &datetime)
// TODO: AmelasError AmelasController::getMountStatus()
// TODO: AmelasError AmelasController::getDeviceInfo()

AmelasError AmelasController::enableTrackingAdjusts(const bool &enabled)
{
    // Command used for log
    const std::string command = "EN_TRACK_ADJ";

    // Symbol used for PLC
    const std::string symbol = "MAIN.timeManager._bEnableTrackingAdjuts";

    // Functionality
    return setEnable(enabled, symbol, command);
}

AmelasError AmelasController::enableMountPower(const bool &enabled)
{
    // Command used for log
    const std::string command = "EN_MOUNT_POWER";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.enableMountPower";

    // Functionality
    return setEnable(enabled, symbol, command);
}

AmelasError AmelasController::setSlewSpeed(const AltAzVel &vel)
{
    // Command used for log
    const std::string command = "SET_SLEW_SPEED";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.SlewSpeed";

    // Functionality
    return setSpeed(vel, symbol, command);
}

AmelasError AmelasController::getSlewSpeed(AltAzVel &vel)
{
    // Command used for log
    const std::string command = "GET_SLEW_SPEED";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.SlewSpeed";

    // Functionality
    return getSpeed(vel, symbol, command);
}

AmelasError AmelasController::setHomePosition(const AltAzPos &pos)
{
    // Command used for log
    const std::string command = "SET_HOME_POSITION";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.HomePosition";

    // Functionality
    return setPosition(pos, symbol, command);
}

AmelasError AmelasController::getHomePosition(AltAzPos &pos)
{
    // Command used for log
    const std::string command = "GET_HOME_POSITION";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.HomePosition";

    // Functionality
    return getPosition(pos, symbol, command);
}

AmelasError AmelasController::setIdlePosition(const AltAzPos &pos)
{
    // Command used for log
    const std::string command = "SET_IDLE_POSITION";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.IdlePosition";

    // Functionality
    return setPosition(pos, symbol, command);
}

AmelasError AmelasController::getIdlePosition(AltAzPos &pos)
{
    // Command used for log
    const std::string command = "GET_IDLE_POSITION";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.IdlePosition";

    // Functionality
    return getPosition(pos, symbol, command);
}

AmelasError AmelasController::setParkPosition(const AltAzPos &pos)
{
    // Command used for log
    const std::string command = "SET_PARK_POSITION";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.ParkPosition";

    // Functionality
    return setPosition(pos, symbol, command);
}

AmelasError AmelasController::getParkPosition(AltAzPos &pos)
{
    // Command used for log
    const std::string command = "GET_PARK_POSITION";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.ParkPosition";

    // Functionality
    return getPosition(pos, symbol, command);
}

AmelasError AmelasController::setCalibrationPosition(const AltAzPos &pos)
{
    // Command used for log
    const std::string command = "SET_CALIBRATION_POSITION";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.CalibrationPosition";

    // Functionality
    return setPosition(pos, symbol, command);
}

AmelasError AmelasController::getCalibrationPosition(AltAzPos &pos)
{
    // Command used for log
    const std::string command = "GET_CALIBRATION_POSITION";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.CalibrationPosition";

    // Functionality
    return getPosition(pos, symbol, command);
}

AmelasError AmelasController::setIdlePositionHere()
{
    // Functionality
    AltAzPos pos(_plc->read<double>("MAIN.axesController._azimuthAxis._axis.NcToPlc.ActPos"), _plc->read<double>("MAIN.axesController._elevationAxis._axis.NcToPlc.ActPos"));
    return setIdlePosition(pos);
}

AmelasError AmelasController::setParkPositionHere()
{
    // Functionality
    AltAzPos pos(_plc->read<double>("MAIN.axesController._azimuthAxis._axis.NcToPlc.ActPos"), _plc->read<double>("MAIN.axesController._elevationAxis._axis.NcToPlc.ActPos"));
    return setParkPosition(pos);
}

AmelasError AmelasController::setCalibrationPositionHere()
{
    // Functionality
    AltAzPos pos(_plc->read<double>("MAIN.axesController._azimuthAxis._axis.NcToPlc.ActPos"), _plc->read<double>("MAIN.axesController._elevationAxis._axis.NcToPlc.ActPos"));
    return setCalibrationPosition(pos);
}

AmelasError AmelasController::setWaitAlt(const double &alt)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "SET_WAIT_ALT";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.WaitAlt";

    // TODO: Check the provided values

    // Functionality
    _plc->write(symbol, alt);

    // Log
    std::ostringstream oss;
    oss << "Alt: " << alt << '\n';
    setLog(command, oss.str(), error);

    return error;
}

AmelasError AmelasController::getWaitAlt(double &alt)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "GET_WAIT_ALT";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.WaitAlt";

    // Functionality
    alt = _plc->read<double>(symbol);

    // Log
    setLog(command, "", error);
    
    return error;
}

AmelasError AmelasController::setHomingOffsets(const AltAzAdj &pos)
{
    // Command used for log
    const std::string command = "SET_HOMING_OFFSETS";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.HomingOffsets";

    // Functionality
    return setPosition(pos, symbol, command);
}

AmelasError AmelasController::getHomingOffsets(AltAzAdj &pos)
{
    // Command used for log
    const std::string command = "GET_HOMING_OFFSETS";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.HomingOffsets";

    // Functionality
    return getPosition(pos, symbol, command);
}

AmelasError AmelasController::enableMountModel(const bool &enabled)
{
    // Command used for log
    const std::string command = "EN_MOUNT_MODEL";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.enableMountModel";

    // Functionality
    return setEnable(enabled, symbol, command);
}

// TODO: AmelasError AmelasController::setMountModelCoefs(const MountModelCoefs &coefs)
// TODO: AmelasError AmelasController::getMountModelCoefs(MountModelCoefs &coefs)

// TODO
AmelasError AmelasController::setLocation(const StationLocation &location)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "SET_LOCATION";

    // Symbol used for PLC
    const std::string symbol = "GLOBALS.Parameters.Commander.StationLocation";

    // Functionality
    _plc->write(symbol + ".wgs84.lat", location.wgs84.lat);
    _plc->write(symbol + ".wgs84.lon", location.wgs84.lon);
    _plc->write(symbol + ".wgs84.alt", location.wgs84.alt);
    _plc->write(symbol + ".ecef.x", location.ecef.x);
    _plc->write(symbol + ".ecef.y", location.ecef.y);
    _plc->write(symbol + ".ecef.z", location.ecef.z);

    // Log
    std::ostringstream oss;
    oss << "Lat: " << location.wgs84.lat << '\n'
        << "Lon: " << location.wgs84.lon << '\n'
        << "Alt: " << location.wgs84.alt << '\n'
        << "X: "   << location.ecef.x    << '\n'
        << "Y: "   << location.ecef.y    << '\n'
        << "Z: "   << location.ecef.z    << '\n';
    setLog(command, oss.str(), error);

    return error;
}

AmelasError AmelasController::getLocation(StationLocation &location)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "GET_LOCATION";

    // Symbol used for PLC
    const std::string symbol = "GLOBALS.Parameters.Commander.StationLocation";

    // Functionality
    location.wgs84.lat = _plc->read<double>(symbol + ".wgs84.lat");
    location.wgs84.lon = _plc->read<double>(symbol + ".wgs84.lon");
    location.wgs84.alt = _plc->read<double>(symbol + ".wgs84.alt");
    location.ecef.x    = _plc->read<double>(symbol + ".ecef.x");
    location.ecef.y    = _plc->read<double>(symbol + ".ecef.y");
    location.ecef.z    = _plc->read<double>(symbol + ".ecef.z");

    // Log
    setLog(command, "", error);
    
    return error;
}

// TODO
AmelasError AmelasController::setMeteoData(const MeteoData &meteo)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "SET_METEO_DATA";

    // Symbol used for PLC
    const std::string symbol = "GLOBALS.Parameters.Commander.MeteoData";

    // Functionality
    _plc->write(symbol + ".press", meteo.press);
    _plc->write(symbol + ".temp", meteo.temp);
    _plc->write(symbol + ".hr", meteo.hr);

    // Log
    std::ostringstream oss;
    oss << "Press: " << meteo.press << '\n'
        << "Temp: "  << meteo.temp  << '\n'
        << "HR: "    << meteo.hr    << '\n';
    setLog(command, oss.str(), error);

    return error;
}

AmelasError AmelasController::getMeteoData(MeteoData &meteo)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "GET_METEO_DATA";

    // Symbol used for PLC
    const std::string symbol = "GLOBALS.Parameters.Commander.MeteoData";

    // Functionality
    meteo.press = _plc->read<double>(symbol + ".press");
    meteo.temp  = _plc->read<double>(symbol + ".temp");
    meteo.hr    = _plc->read<double>(symbol + ".hr");

    // Log
    setLog(command, "", error);
    
    return error;
}

// TODO: AmelasError AmelasController::enableSimulationMode(const bool &enabled)
// TODO: AmelasError AmelasController::getSimulationState(bool &enabled)
// TODO: AmelasError AmelasController::setSimulationTime(const std::string &datetime)
//=====================================================================================================================


// MOTION RELATED FUNCTIONS
//=====================================================================================================================
AmelasError AmelasController::getMotionMode(AmelasMotionMode &motion_mode)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "GET_MOTION_MODE";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander._motionMode";

    // Functionality
    if (_plc->read<double>(symbol) == 0.0)
        motion_mode = AmelasMotionMode::NO_MOTION;
    else if (_plc->read<double>(symbol) == 1.0)
        motion_mode = AmelasMotionMode::ABSOLUTE_MOTION;
    else if (_plc->read<double>(symbol) == 2.0)
        motion_mode = AmelasMotionMode::RELATIVE_MOTION;
    else if (_plc->read<double>(symbol) == 3.0)
        motion_mode = AmelasMotionMode::CONTINUOUS;
    else if (_plc->read<double>(symbol) == 4.0)
        motion_mode = AmelasMotionMode::HOMING_OP;
    else if (_plc->read<double>(symbol) == 5.0)
        motion_mode = AmelasMotionMode::TO_IDLE;
    else if (_plc->read<double>(symbol) == 6.0)
        motion_mode = AmelasMotionMode::TO_PARK;
    else if (_plc->read<double>(symbol) == 7.0)
        motion_mode = AmelasMotionMode::TO_CALIBRATION;
    else if (_plc->read<double>(symbol) == 8.0)
        motion_mode = AmelasMotionMode::CPF;
    else if (_plc->read<double>(symbol) == 9.0)
        motion_mode = AmelasMotionMode::STAR;
    // TODO
    // else if (_plc->read<double>(symbol) == X.0)
    //     motion_mode = AmelasMotionMode::TLE;
    else
        motion_mode = AmelasMotionMode::UNKNOWN;

    // Log
    std::string motion_str = MotionModeStr[static_cast<size_t>(motion_mode)];
    std::ostringstream oss;
    oss << "Motion mode: " << motion_str << '\n';
    setLog(command, oss.str(), error);

    return error;
}

AmelasError AmelasController::getMotionState(AmelasMotionState &motion_state, AltAzPos &pos)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "GET_MOTION_STATE";

    // Symbols used for PLC
    const std::string sState     = "MAIN.axesController._motionState";
    const std::string sMode      = "MAIN.commander._motionMode";
    const std::string sActPosAz  = "MAIN.axesController._azimuthAxis._axis.NcToPlc.ActPos";
    const std::string sActPosEl  = "MAIN.axesController._elevationAxis._axis.NcToPlc.ActPos";

    // Variables used for this function
    pos.az = _plc->read<double>(sActPosAz);
    pos.el = _plc->read<double>(sActPosEl);

    // Functionality
    if (_plc->read<double>(sState) == 4.0      // Homing
        || _plc->read<double>(sState) == 6.0   // MovingAbsolute
        || _plc->read<double>(sState) == 7.0   // MovingRelative
        || _plc->read<double>(sState) == 13.0) // MovingVelocity
        motion_state = AmelasMotionState::MOVING;
    else if (_plc->read<double>(sState) == 8.0) // Halting
        motion_state = AmelasMotionState::PAUSED;
    else if (_plc->read<double>(sMode) == 1.0    // LoadMoveAbsolute
            || _plc->read<double>(sMode) == 2.0  // LoadMoveRelative
            || _plc->read<double>(sMode) == 3.0  // LoadMoveVelocity
            || _plc->read<double>(sMode) == 4.0  // LoadMoveToHome
            || _plc->read<double>(sMode) == 5.0  // LoadMoveToIdle
            || _plc->read<double>(sMode) == 6.0  // LoadMoveToPark
            || _plc->read<double>(sMode) == 7.0  // LoadMoveToCalibration
            || _plc->read<double>(sMode) == 8.0  // LoadMoveCPFMotion
            || _plc->read<double>(sMode) == 9.0) // LoadMoveStarMotion
        motion_state = AmelasMotionState::WAITING_START;
    else if (_plc->read<double>(sState) == 3.0 // Standstill
            && _plc->read<double>(sActPosAz) == _plc->read<double>("MAIN.commander.IdlePosition.Azimuth")
            && _plc->read<double>(sActPosEl) == _plc->read<double>("MAIN.commander.IdlePosition.Elevation"))
        motion_state = AmelasMotionState::IDLE;
    else if (_plc->read<double>(sState) == 3.0 // Standstill
            && _plc->read<double>(sActPosAz) == _plc->read<double>("MAIN.commander.ParkPosition.Azimuth")
            && _plc->read<double>(sActPosEl) == _plc->read<double>("MAIN.commander.ParkPosition.Elevation"))
        motion_state = AmelasMotionState::PARK;
    else if (_plc->read<double>(sState) == 3.0 // Standstill
            && _plc->read<double>(sActPosAz) == _plc->read<double>("MAIN.commander.CalibrationPosition.Azimuth")
            && _plc->read<double>(sActPosEl) == _plc->read<double>("MAIN.commander.CalibrationPosition.Elevation"))
        motion_state = AmelasMotionState::CALIBRATION;
    else if (_plc->read<double>(sState) == 3.0) // Standstill
        motion_state = AmelasMotionState::STOPPED;
    else if (_plc->read<double>(sState) == 10.0) // Error
        motion_state = AmelasMotionState::INVALID_ERROR;
    else if (_plc->read<double>(sState) == 0.0) // Disabled
        motion_state = AmelasMotionState::DISABLED;
    else if (_plc->read<double>(sState) == 11.0) // Reset
        motion_state = AmelasMotionState::RESET;
    else
        motion_state = AmelasMotionState::UNKNOWN;

    // Log
    std::ostringstream oss;

    if (motion_state == AmelasMotionState::STOPPED
        || motion_state == AmelasMotionState::IDLE
        || motion_state == AmelasMotionState::PARK
        || motion_state == AmelasMotionState::CALIBRATION)
    {
        oss << "Motion state: " << MotionStateStr[static_cast<size_t>(motion_state)]   << '\n'
            << "Az: " << std::to_string(pos.az) << '\n'
            << "El: " << std::to_string(pos.el) << '\n';
    }
    else
        oss << "Motion state: " << MotionStateStr[static_cast<size_t>(motion_state)] << '\n';

    setLog(command, oss.str(), error);

    return error;
}

AmelasError AmelasController::doStartMotion()
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "DO_START_MOTION";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.StartMotion";

    // Check conditions
    if (_plc->read<double>(symbol) == 4.0      // Homing
        || _plc->read<double>(symbol) == 6.0   // MovingAbsolute
        || _plc->read<double>(symbol) == 7.0   // MovingRelative
        || _plc->read<double>(symbol) == 13.0) // MovingVelocity
    {
        ; // Do nothing
    }
    else
    {
        // Functionality
        // CHECK: _plc->executeCommand(symbol);
        _plc->write(symbol + ".cmd", true);
    }

    // Log
    setLog(command, "", error);

    return error;
}

AmelasError AmelasController::doPauseMotion()
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "DO_PAUSE_MOTION";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.PauseMotion";

    // Functionality
    // CHECK: _plc->executeCommand(symbol);
    _plc->write(symbol + ".cmd", true);

    // Log
    setLog(command, "", error);
    
    return error;
}

AmelasError AmelasController::doStopMotion()
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "DO_STOP_MOTION";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.StopMotion";

    // Functionality
    // CHECK: _plc->executeCommand(symbol);
    _plc->write(symbol + ".cmd", true);

    // Log
    setLog(command, "", error);
    
    return error;
}

AmelasError AmelasController::setTrackPosOffset(const AltAzAdj &pos)
{
    // Command used for log
    const std::string command = "SET_TRACK_POS_OFFSET";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.TrackPosOffset";

    // Functionality
    return setPosition(pos, symbol, command);
}

AmelasError AmelasController::getTrackPosOffset(AltAzAdj &pos)
{
    // Command used for log
    const std::string command = "GET_TRACK_POS_OFFSET";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.TrackPosOffset";

    // Functionality
    return getPosition(pos, symbol, command);
}

AmelasError AmelasController::setTrackTimeBias(const double &time)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "SET_TRACK_TIME_BIAS";

    // Symbol used for PLC
    const std::string symbol = "MAIN.timeManager._rTrackTimeBias";

    // TODO: Check the provided values

    // Functionality
    _plc->write(symbol, time);

    // Log
    std::ostringstream oss;
    oss << "Track time bias: " << time << '\n';
    setLog(command, oss.str(), error);

    return error;
}

AmelasError AmelasController::getTrackTimeBias(double &time)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "GET_TRACK_TIME_BIAS";

    // Symbol used for PLC
    const std::string symbol = "MAIN.timeManager._rTrackTimeBias";

    // Functionality
    time = _plc->read<double>(symbol);

    // Log
    setLog(command, "", error);
    
    return error;
}

// TODO
AmelasError AmelasController::setAbsoluteAltAzMotion(const AltAzPos &pos, const AltAzVel &vel)
{
    // Auxiliar results
    AmelasError error_pos = AmelasError::SUCCESS;
    AmelasError error_vel = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "SET_ABS_ALTAZ_MOTION";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.moveAbsolute";

    error_pos = setPosition(pos, symbol + "Position", command + " (POS)");
    error_vel = setSpeed(vel, symbol + "Velocity", command + " (VEL)");

    //if (error_pos == AmelasError::SUCCESS && error_vel == AmelasError::SUCCESS)
    //{
        // Functionality
        // CHECK: _plc->executeCommand(symbol + "Cmd");
        _plc->write(symbol + "Cmd.cmd", true);

     //   return AmelasError::SUCCESS;
    //}
    //else if (error_pos == AmelasError::INVALID_POSITION && error_vel == AmelasError::SUCCESS)
    //    return AmelasError::INVALID_POSITION;
    //else if (error_pos == AmelasError::SUCCESS && error_vel == AmelasError::INVALID_SPEED)
    //    return AmelasError::INVALID_SPEED;
    //else
    //    return AmelasError::INVALID_ERROR;
    return AmelasError::SUCCESS;
}

AmelasError AmelasController::setRelativeAltAzMotion(const AltAzPos &pos, const AltAzVel &vel)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "SET_REL_ALTAZ_MOTION";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.moveRelative";

    // Functionality
    _plc->write(symbol + "Position.Azimuth", pos.az);
    _plc->write(symbol + "Position.Elevation", pos.el);
    _plc->write(symbol + "Velocity.Azimuth", vel.az);
    _plc->write(symbol + "Velocity.Elevation", vel.el);
    // CHECK: _plc->executeCommand(symbol + "Cmd");
    _plc->write(symbol + "Cmd.cmd", true);

    // Log
    std::ostringstream oss;
    oss << "Az (pos): " << pos.az << " - El (pos): " << pos.el << '\n'
        << "Az (vel): " << vel.az << " - El (vel): " << vel.el << '\n';
    setLog(command, oss.str(), error);
    
    return error;
}

AmelasError AmelasController::setContAltAzMotion(const AltAzVel &vel)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "SET_CON_ALTAZ_MOTION";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.moveVelocity";

    // Functionality
    _plc->write(symbol + "Velocity.Azimuth", vel.az);
    _plc->write(symbol + "Velocity.Elevation", vel.el);
    // CHECK: _plc->executeCommand(symbol + "Cmd");
    _plc->write(symbol + "Cmd.cmd", true);

    // Log
    std::ostringstream oss;
    oss << "Az (vel): " << vel.az << '\n'
        << "El (vel): " << vel.el << '\n';
    setLog(command, oss.str(), error);

    return error;
}

// TODO: AmelasError AmelasController::setHomingMotion()

AmelasError AmelasController::setIdleMotion()
{

    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "SET_IDLE_MOTION";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.moveToIdleCmd";

    if (_plc->read<double>("MAIN.commander.SlewSpeed.Azimuth") == 0.0
        || _plc->read<double>("MAIN.commander.SlewSpeed.Elevation") == 0.0)
    {
        AmelasError error = AmelasError::INVALID_SPEED;

        // Log
        std::ostringstream oss;
        oss << "There is no speed defined in SlewSpeed." << '\n';
        setLog(command, oss.str(), error);
    }
    else
    {
        // Functionality
        // CHECK: _plc->executeCommand(symbol);
        _plc->write(symbol + ".cmd", true);

        // Log
        setLog(command, "", error);
    }
    
    return error;
}

AmelasError AmelasController::setParkMotion()
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "SET_PARK_MOTION";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.moveToParkCmd";

    if (_plc->read<double>("MAIN.commander.SlewSpeed.Azimuth") == 0.0
        || _plc->read<double>("MAIN.commander.SlewSpeed.Elevation") == 0.0)
    {
        AmelasError error = AmelasError::INVALID_SPEED;

        // Log
        std::ostringstream oss;
        oss << "There is no speed defined in SlewSpeed." << '\n';
        setLog(command, oss.str(), error);
    }
    else
    {
        // Functionality
        // CHECK: _plc->executeCommand(symbol);
        _plc->write(symbol + ".cmd", true);

        // Log
        setLog(command, "", error);
    }
    
    return error;
}

AmelasError AmelasController::setCalibrationMotion()
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "SET_CALIBRATION_MOTION";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.moveToCalibrationCmd";

    if (_plc->read<double>("MAIN.commander.SlewSpeed.Azimuth") == 0.0
        || _plc->read<double>("MAIN.commander.SlewSpeed.Elevation") == 0.0)
    {
        AmelasError error = AmelasError::INVALID_SPEED;

        // Log
        std::ostringstream oss;
        oss << "There is no speed defined in SlewSpeed." << '\n';
        setLog(command, oss.str(), error);
    }
    else
    {
        // Functionality
        // CHECK: _plc->executeCommand(symbol);
        _plc->write(symbol + ".cmd", true);

        // Log
        setLog(command, "", error);
    }
    
    return error;
}

AmelasError AmelasController::pruebaBucles()
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "DO_PRUEBA_BUCLES";

    // Symbols used for PLC
    const std::string bButton  = "ENTRY_POINT_TRACKING.bButton";
    const std::string aBuffer1 = "ENTRY_POINT_TRACKING.aBuffer1[";
    const std::string aBuffer2 = "ENTRY_POINT_TRACKING.aBuffer2[";

    // Variables used for this function
    unsigned short int cMin = _plc->read<unsigned short int>("ENTRY_POINT_TRACKING.cMin");
    unsigned short int cMax = _plc->read<unsigned short int>("ENTRY_POINT_TRACKING.cMax");
    unsigned short int i    = 0;

    // Functionality
    _plc->write(bButton, true);

    while (i <= cMax*2+1)
    {
        std::string aBuffer;

        if (i <= cMax)
            aBuffer = aBuffer1;
        else
            aBuffer = aBuffer2;

        if (_plc->read<bool>(aBuffer + std::to_string(i) + "].bRead") == true)
        {
            _plc->write(aBuffer + std::to_string(i) + "].bRead", false);
            _plc->write(aBuffer + std::to_string(i) + "].nPosition.Azimuth", static_cast<double>(i));
            _plc->write(aBuffer + std::to_string(i) + "].nPosition.Elevation", static_cast<double>(i));
            _plc->write(aBuffer + std::to_string(i) + "].bWritten", true);
            i++;
        }
    }

    // Log
    setLog(command, "", error);

    return error;
}

// TODO: AmelasError AmelasController::setCPFMotion(const file &cpf, AmelasTracking &tracking)
// TODO: AmelasError AmelasController::setStarMotion(const StarData &star_data)
// =====================================================================================================================


}} // END NAMESPACES.
// =====================================================================================================================
