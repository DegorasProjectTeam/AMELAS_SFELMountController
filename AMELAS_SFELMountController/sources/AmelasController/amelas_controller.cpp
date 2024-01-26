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
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.commander.doResetState";
    const std::string command = "DO_RESET_STATE";

    _plc->write(symbol, true);

    // Log.
    setLog(command, "", error);

    return error;
}

// TODO: AmelasError AmelasController::enableAvoidSun(const bool &enabled)
//=====================================================================================================================


// LOW LEVEL PLC REGISTERS RELATED FUNCTIONS
//=====================================================================================================================
AmelasError AmelasController::doConnectPLC()
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string command = "DO_CONNECT_PLC";

    _plc->connect();

    // Log.
    setLog(command, "", error);

    return error;
}

AmelasError AmelasController::doDisconnectPLC()
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string command = "DO_DISCONNECT_PLC";

    _plc->disconnect();

    // Log.
    setLog(command, "", error);

    return error;
}

AmelasError AmelasController::getPLCregister(const PLCAddress &address, PLCRegisterValue &value)
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string command = "GET_PLC_REGISTER";

    if (address.type == "double")
        value.value = std::to_string(_plc->read<double>(address.symbol));
    else if (address.type == "int")
        value.value = std::to_string(_plc->read<int>(address.symbol));
    else if (address.type == "bool")
        value.value = std::to_string(_plc->read<bool>(address.symbol));

    value.symbol = address.symbol;
    value.type = address.type;

    // Log.
    std::ostringstream oss;
    oss << "Symbol: " << value.symbol << " <" << value.type << "> = " << value.value << '\n';
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
    std::string cmd_str = ControllerErrorStr[static_cast<size_t>(error)];

    std::ostringstream oss_1;

    if (error == AmelasError::SUCCESS)
        oss_1 << std::string(93, '-') << '\n';
    else if (error == AmelasError::UNSAFE_POSITION || error == AmelasError::UNSAFE_SPEED)
        oss_1 << std::string(90, '-') << '\n';
    else
        oss_1 << std::string(92, '-') << '\n';

    std::ostringstream oss;
    oss << oss_1.str()
    << "<AMELAS CONTROLLER>" << '\n'
    << "-> " << command << '\n'
    << "Time: " << zmqutils::utils::currentISO8601Date() << '\n'
    << specific
    << "Error: " << static_cast<int>(error) << " (" << cmd_str << ")" << '\n'
    << std::string(100, '-') << '\n';

    if (error == AmelasError::SUCCESS)
        _logger->info(oss.str());
    else if (error == AmelasError::MOUNT_UNSAFE_STATE || error == AmelasError::UNSAFE_POSITION || error == AmelasError::UNSAFE_SPEED || error == AmelasError::STAR_UNSAFE)
        _logger->warn(oss.str());
    else
        _logger->error(oss.str());
}

AmelasError AmelasController::setEnable(const bool &enabled, const std::string plcSymbol, const std::string command)
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    std::ostringstream oss;

    // TODO: Check the provided values
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

    // Do things in the hardware (PLC).
    _plc->write(plcSymbol, enabled);

    // Log.
    setLog(command, oss.str(), error);

    return error;
}

AmelasError AmelasController::setPosition(const AltAzPos &pos, const std::string plcSymbol, const std::string command)
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    // Check the provided values.
    if (pos.az >= 360.0 ||  pos.az < 0.0 || pos.el >= 90. || pos.el < 0.)
    {
        error = AmelasError::INVALID_POSITION;
    }
    else
    {
        if (pos.az >= 300.0 || pos.el >= 80.)
        {
            error = AmelasError::UNSAFE_POSITION;
        }

        // Do things in the hardware (PLC).
        _plc->write(plcSymbol + ".Azimuth", pos.az);
        _plc->write(plcSymbol + ".Elevation", pos.el);
    }

    // Log.
    std::ostringstream oss;
    oss << "Az: " << pos.az << '\n'
    << "El: " << pos.el << '\n';
    setLog(command, oss.str(), error);

    return error;
}

AmelasError AmelasController::getPosition(AltAzPos &pos, const std::string plcSymbol, const std::string command)
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    pos.az = _plc->read<double>(plcSymbol + ".Azimuth");
    pos.el = _plc->read<double>(plcSymbol + ".Elevation");

    // Log.
    setLog(command, "", error);
    
    return error;
}

AmelasError AmelasController::setSpeed(const AltAzVel &vel, const std::string plcSymbol, const std::string command)
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    // Check the provided values.
    if (vel.az < 0.0 ||  vel.el < 0.0)
    {
        error = AmelasError::INVALID_SPEED;
    }
    else
    {
        // Do things in the hardware (PLC).
        _plc->write(plcSymbol + ".Azimuth", vel.az);
        _plc->write(plcSymbol + ".Elevation", vel.el);
    }

    // Log.
    std::ostringstream oss;
    oss << "Az: " << vel.az << '\n'
    << "El: " << vel.el << '\n';
    setLog(command, oss.str(), error);

    return error;
}

AmelasError AmelasController::getSpeed(AltAzVel &vel, const std::string plcSymbol, const std::string command)
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    vel.az = _plc->read<double>(plcSymbol + ".Azimuth");
    vel.el = _plc->read<double>(plcSymbol + ".Elevation");

    // Log.
    setLog(command, "", error);
    
    return error;
}

// TODO: AmelasError AmelasController::getMountLog(const std::string &start, const std::string &end)
// TODO: AmelasError AmelasController::doSyncTimeNTP(const std::string &host, const unsigned &port, const unsigned &timeout)
// TODO: AmelasError AmelasController::doSyncTimeManual(const std::string &datetime)
// TODO: AmelasError AmelasController::getMountStatus()
// TODO: AmelasError AmelasController::getDeviceInfo()

AmelasError AmelasController::enableTrackingAdjusts(const bool &enabled)
{
    const std::string symbol = "MAIN.commander.enableTrackingAdjuts";
    const std::string command = "EN_TRACK_ADJ";
    return setEnable(enabled, symbol, command);
}

AmelasError AmelasController::enableMountPower(const bool &enabled)
{
    const std::string symbol = "MAIN.commander.enableMountPower";
    const std::string command = "EN_MOUNT_POWER";
    return setEnable(enabled, symbol, command);
}

AmelasError AmelasController::setSlewSpeed(const AltAzVel &vel)
{
    const std::string symbol = "MAIN.commander.SlewSpeed";
    const std::string command = "SET_SLEW_SPEED";
    return setSpeed(vel, symbol, command);
}

AmelasError AmelasController::getSlewSpeed(AltAzVel &vel)
{
    const std::string symbol = "MAIN.commander.SlewSpeed";
    const std::string command = "GET_SLEW_SPEED";
    return getSpeed(vel, symbol, command);
}

AmelasError AmelasController::setHomePosition(const AltAzPos &pos)
{
    const std::string symbol = "MAIN.commander.HomePosition";
    const std::string command = "SET_HOME_POSITION";
    return setPosition(pos, symbol, command);
}

AmelasError AmelasController::getHomePosition(AltAzPos &pos)
{
    const std::string symbol = "MAIN.commander.HomePosition";
    const std::string command = "GET_HOME_POSITION";
    return getPosition(pos, symbol, command);
}

AmelasError AmelasController::setIdlePosition(const AltAzPos &pos)
{
    const std::string symbol = "MAIN.commander.IdlePosition";
    const std::string command = "SET_IDLE_POSITION";
    return setPosition(pos, symbol, command);
}

AmelasError AmelasController::getIdlePosition(AltAzPos &pos)
{
    const std::string symbol = "MAIN.commander.IdlePosition";
    const std::string command = "GET_IDLE_POSITION";
    return getPosition(pos, symbol, command);
}

AmelasError AmelasController::setParkPosition(const AltAzPos &pos)
{
    const std::string symbol = "MAIN.commander.ParkPosition";
    const std::string command = "SET_PARK_POSITION";
    return setPosition(pos, symbol, command);
}

AmelasError AmelasController::getParkPosition(AltAzPos &pos)
{
    const std::string symbol = "MAIN.commander.ParkPosition";
    const std::string command = "GET_PARK_POSITION";
    return getPosition(pos, symbol, command);
}

AmelasError AmelasController::setCalibrationPosition(const AltAzPos &pos)
{
    const std::string symbol = "MAIN.commander.CalibrationPosition";
    const std::string command = "SET_CALIBRATION_POSITION";
    return setPosition(pos, symbol, command);
}

AmelasError AmelasController::getCalibrationPosition(AltAzPos &pos)
{
    const std::string symbol = "MAIN.commander.CalibrationPosition";
    const std::string command = "GET_CALIBRATION_POSITION";
    return getPosition(pos, symbol, command);
}

AmelasError AmelasController::setIdlePositionHere()
{
    AltAzPos pos(_plc->read<double>("MAIN.axesController._azimuthAxis._axis.NcToPlc.ActPos"), _plc->read<double>("MAIN.axesController._elevationAxis._axis.NcToPlc.ActPos"));
    return setIdlePosition(pos);
}

AmelasError AmelasController::setParkPositionHere()
{
    AltAzPos pos(_plc->read<double>("MAIN.axesController._azimuthAxis._axis.NcToPlc.ActPos"), _plc->read<double>("MAIN.axesController._elevationAxis._axis.NcToPlc.ActPos"));
    return setParkPosition(pos);
}

AmelasError AmelasController::setCalibrationPositionHere()
{
    AltAzPos pos(_plc->read<double>("MAIN.axesController._azimuthAxis._axis.NcToPlc.ActPos"), _plc->read<double>("MAIN.axesController._elevationAxis._axis.NcToPlc.ActPos"));
    return setCalibrationPosition(pos);
}

AmelasError AmelasController::setWaitAlt(const double &alt)
{
    const std::string symbol = "MAIN.commander.WaitAlt";
    const std::string command = "SET_WAIT_ALT";

    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    // TODO: Check the provided values

    // Do things in the hardware (PLC).
    _plc->write(symbol, alt);

    // Log.
    std::ostringstream oss;
    oss << "Alt: " << alt << '\n';
    setLog(command, oss.str(), error);

    return error;
}

AmelasError AmelasController::getWaitAlt(double &alt)
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.commander.WaitAlt";
    const std::string command = "GET_WAIT_ALT";

    alt = _plc->read<double>(symbol);

    // Log.
    setLog(command, "", error);
    
    return error;
}

AmelasError AmelasController::setHomingOffsets(const AltAzAdj &pos)
{
    const std::string symbol = "MAIN.commander.HomingOffsets";
    const std::string command = "SET_HOMING_OFFSETS";
    return setPosition(pos, symbol, command);
}

AmelasError AmelasController::getHomingOffsets(AltAzAdj &pos)
{
    const std::string symbol = "MAIN.commander.HomingOffsets";
    const std::string command = "GET_HOMING_OFFSETS";
    return getPosition(pos, symbol, command);
}

AmelasError AmelasController::enableMountModel(const bool &enabled)
{
    const std::string symbol = "MAIN.commander.enableMountModel";
    const std::string command = "EN_MOUNT_MODEL";
    return setEnable(enabled, symbol, command);
}

// TODO: AmelasError AmelasController::setMountModelCoefs(const MountModelCoefs &coefs)
// TODO: AmelasError AmelasController::getMountModelCoefs(MountModelCoefs &coefs)

AmelasError AmelasController::setLocation(const StationLocation &location)
{
    const std::string symbol = "GLOBALS.Parameters.Commander.StationLocation";
    const std::string command = "SET_LOCATION";

    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    // Do things in the hardware (PLC).
    _plc->write(symbol + ".wgs84.lat", location.wgs84.lat);
    _plc->write(symbol + ".wgs84.lon", location.wgs84.lon);
    _plc->write(symbol + ".wgs84.alt", location.wgs84.alt);
    _plc->write(symbol + ".ecef.x", location.ecef.x);
    _plc->write(symbol + ".ecef.y", location.ecef.y);
    _plc->write(symbol + ".ecef.z", location.ecef.z);

    // Log.
    std::ostringstream oss;
    oss << "Lat: " << location.wgs84.lat << '\n'
    << "Lon: " << location.wgs84.lon << '\n'
    << "Alt: " << location.wgs84.alt << '\n'
    << "X: " << location.ecef.x << '\n'
    << "Y: " << location.ecef.y << '\n'
    << "Z: " << location.ecef.z << '\n';
    setLog(command, oss.str(), error);

    return error;
}

AmelasError AmelasController::getLocation(StationLocation &location)
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "GLOBALS.Parameters.Commander.StationLocation";
    const std::string command = "GET_LOCATION";

    location.wgs84.lat = _plc->read<double>(symbol + ".wgs84.lat");
    location.wgs84.lon = _plc->read<double>(symbol + ".wgs84.lon");
    location.wgs84.alt = _plc->read<double>(symbol + ".wgs84.alt");
    location.ecef.x = _plc->read<double>(symbol + ".ecef.x");
    location.ecef.y = _plc->read<double>(symbol + ".ecef.y");
    location.ecef.z =_plc->read<double>(symbol + ".ecef.z");

    // Log.
    setLog(command, "", error);
    
    return error;
}

AmelasError AmelasController::setMeteoData(const MeteoData &meteo)
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "GLOBALS.Parameters.Commander.MeteoData";
    const std::string command = "SET_METEO_DATA";

    // Do things in the hardware (PLC).
    _plc->write(symbol + ".press", meteo.press);
    _plc->write(symbol + ".temp", meteo.temp);
    _plc->write(symbol + ".hr", meteo.hr);

    // Log.
    std::ostringstream oss;
    oss << "Press: " << meteo.press << '\n'
    << "Temp: " << meteo.temp << '\n'
    << "HR: " << meteo.hr << '\n';
    setLog(command, oss.str(), error);

    return error;
}

AmelasError AmelasController::getMeteoData(MeteoData &meteo)
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "GLOBALS.Parameters.Commander.MeteoData";
    const std::string command = "GET_METEO_DATA";

    meteo.press = _plc->read<double>(symbol + ".press");
    meteo.temp = _plc->read<double>(symbol + ".temp");
    meteo.hr = _plc->read<double>(symbol + ".hr");

    // Log.
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
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.commander._motionMode";
    const std::string command = "GET_MOTION_MODE";

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
    // else if (_plc->read<double>(symbol) == X.0)
    //     motion_mode = AmelasMotionMode::TLE;
    else
        motion_mode = AmelasMotionMode::UNKNOWN;

    // Log.
    std::string motion_str = MotionModeStr[static_cast<size_t>(motion_mode)];
    std::ostringstream oss;
    oss << "Motion mode: " << motion_str << '\n';
    setLog(command, oss.str(), error);
    
    return error;
}

AmelasError AmelasController::getMotionState(AmelasMotionState &motion_state)
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.axesController._motionState";
    const std::string command = "GET_MOTION_STATE";

    if (_plc->read<double>(symbol) == 4.0
        || _plc->read<double>(symbol) == 6.0
        || _plc->read<double>(symbol) == 7.0
        || _plc->read<double>(symbol) == 13.0)
        motion_state = AmelasMotionState::MOVING;
    else if (_plc->read<double>(symbol) == 8.0)
        motion_state = AmelasMotionState::PAUSED;
    else if (_plc->read<double>("MAIN.commander._motionMode") == 1.0
                || _plc->read<double>("MAIN.commander._motionMode") == 2.0
                || _plc->read<double>("MAIN.commander._motionMode") == 3.0
                || _plc->read<double>("MAIN.commander._motionMode") == 4.0
                || _plc->read<double>("MAIN.commander._motionMode") == 5.0
                || _plc->read<double>("MAIN.commander._motionMode") == 6.0
                || _plc->read<double>("MAIN.commander._motionMode") == 7.0
                || _plc->read<double>("MAIN.commander._motionMode") == 8.0
                || _plc->read<double>("MAIN.commander._motionMode") == 9.0)
        motion_state = AmelasMotionState::WAITING_START;
    else if (_plc->read<double>(symbol) == 3.0)
        motion_state = AmelasMotionState::STOPPED;
    else if (_plc->read<double>(symbol) == 10.0)
        motion_state = AmelasMotionState::INVALID_ERROR;
    else if (_plc->read<double>(symbol) == 0.0)
        motion_state = AmelasMotionState::DISABLED;
    else if (_plc->read<double>(symbol) == 11.0)
        motion_state = AmelasMotionState::RESET;
    else
        motion_state = AmelasMotionState::UNKNOWN;

    // Log.
    std::string motion_str = MotionStateStr[static_cast<size_t>(motion_state)];
    std::ostringstream oss;
    oss << "Motion state: " << motion_str << '\n';
    setLog(command, oss.str(), error);

    return error;
}

AmelasError AmelasController::doStartMotion()
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.commander.StartMotion";
    const std::string command = "DO_START_MOTION";

    // Do things in the hardware (PLC).
    // _plc->executeCommand(symbol);
    _plc->write(symbol + ".cmd", true);

    // Log.
    setLog(command, "", error);
    
    return error;
}

AmelasError AmelasController::doPauseMotion()
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.commander.PauseMotion";
    const std::string command = "DO_PAUSE_MOTION";

    // Do things in the hardware (PLC).
    // _plc->executeCommand(symbol);
    _plc->write(symbol + ".cmd", true);

    // Log.
    setLog(command, "", error);
    
    return error;
}

AmelasError AmelasController::doStopMotion()
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.commander.StopMotion";
    const std::string command = "DO_STOP_MOTION";

    // Do things in the hardware (PLC).
    // _plc->executeCommand(symbol);
    _plc->write(symbol + ".cmd", true);

    // TODO: borrar todo lo relativo al movimiento actual

    // Log.
    setLog(command, "", error);
    
    return error;
}

AmelasError AmelasController::setTrackPosOffset(const AltAzAdj &pos)
{
    const std::string symbol = "MAIN.commander.TrackPosOffset";
    const std::string command = "SET_TRACK_POS_OFFSET";
    return setPosition(pos, symbol, command);
}

AmelasError AmelasController::getTrackPosOffset(AltAzAdj &pos)
{
    const std::string symbol = "MAIN.commander.TrackPosOffset";
    const std::string command = "GET_TRACK_POS_OFFSET";
    return getPosition(pos, symbol, command);
}

AmelasError AmelasController::setTrackTimeBias(const double &time)
{
    const std::string symbol = "MAIN.commander.TrackTimeBias";
    const std::string command = "SET_TRACK_TIME_BIAS";

    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    // TODO: Check the provided values

    // Do things in the hardware (PLC).
    _plc->write(symbol, time);

    // Log.
    std::ostringstream oss;
    oss << "Track time bias: " << time << '\n';
    setLog(command, oss.str(), error);

    return error;
}

AmelasError AmelasController::getTrackTimeBias(double &time)
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.commander.TrackTimeBias";
    const std::string command = "GET_TRACK_TIME_BIAS";

    time = _plc->read<double>(symbol);

    // Log.
    setLog(command, "", error);
    
    return error;
}

AmelasError AmelasController::setAbsoluteAltAzMotion(const AltAzPos &pos, const AltAzVel &vel)
{
    // Auxiliar result.
    AmelasError error_pos = AmelasError::SUCCESS;
    AmelasError error_vel = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.commander.moveAbsolute";
    const std::string command = "SET_ABS_ALTAZ_MOTION";

    error_pos = setPosition(pos, symbol + "Position", command + " (POS)");
    error_vel = setSpeed(vel, symbol + "Velocity", command + " (VEL)");

    if (error_pos == AmelasError::SUCCESS && error_vel == AmelasError::SUCCESS)
    {
        // Do things in the hardware (PLC).
        // _plc->executeCommand(symbol);
        _plc->write(symbol + "Cmd.cmd", true);

        return AmelasError::SUCCESS;
    }
    else if (error_pos == AmelasError::INVALID_POSITION && error_vel == AmelasError::SUCCESS)
        return AmelasError::INVALID_POSITION;
    else if (error_pos == AmelasError::SUCCESS && error_vel == AmelasError::INVALID_SPEED)
        return AmelasError::INVALID_SPEED;
    else
        return AmelasError::INVALID_ERROR;
}

AmelasError AmelasController::setRelativeAltAzMotion(const AltAzPos &pos, const AltAzVel &vel)
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.commander.moveRelative";
    const std::string command = "SET_REL_ALTAZ_MOTION";

    // Do things in the hardware (PLC).
    _plc->write(symbol + "Position.Azimuth", pos.az);
    _plc->write(symbol + "Position.Elevation", pos.el);
    _plc->write(symbol + "Velocity.Azimuth", vel.az);
    _plc->write(symbol + "Velocity.Elevation", vel.el);
    // _plc->executeCommand(symbol + "Cmd");
    _plc->write(symbol + "Cmd.cmd", true);

    // Log.
    std::ostringstream oss;
    oss << "Az (pos): " << pos.az << " - El (pos): " << pos.el << '\n'
    << "Az (vel): " << vel.az << " - El (vel): " << vel.el << '\n';
    setLog(command, oss.str(), error);
    
    return error;
}

AmelasError AmelasController::setContAltAzMotion(const AltAzVel &vel)
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.commander.moveVelocity";
    const std::string command = "SET_CON_ALTAZ_MOTION";

    // Do things in the hardware (PLC).
    _plc->write(symbol + "Velocity.Azimuth", vel.az);
    _plc->write(symbol + "Velocity.Elevation", vel.el);
    _plc->write(symbol + "Cmd.cmd", true);

    // Log.
    std::ostringstream oss;
    oss << "Az (vel): " << vel.az << '\n'
    << "El (vel): " << vel.el << '\n';
    setLog(command, oss.str(), error);

    return error;
}

// TODO: AmelasError AmelasController::setHomingMotion()

AmelasError AmelasController::setIdleMotion()
{

    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.commander.moveToIdleCmd";
    const std::string command = "SET_IDLE_MOTION";

    if (_plc->read<double>("MAIN.commander.SlewSpeed.Azimuth") == 0.0
    || _plc->read<double>("MAIN.commander.SlewSpeed.Elevation") == 0.0)
    {
        AmelasError error = AmelasError::INVALID_SPEED;

        // Log.
        std::ostringstream oss;
        oss << "There is no speed defined in SlewSpeed." << '\n';
        setLog(command, oss.str(), error);
    }
    else
    {
        // Do things in the hardware (PLC).
        // _plc->executeCommand(symbol);
        _plc->write(symbol + ".cmd", true);

        // Log.
        setLog(command, "", error);
    }
    
    return error;
}

AmelasError AmelasController::setParkMotion()
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.commander.moveToParkCmd";
    const std::string command = "SET_Park_MOTION";

    if (_plc->read<double>("MAIN.commander.SlewSpeed.Azimuth") == 0.0
    || _plc->read<double>("MAIN.commander.SlewSpeed.Elevation") == 0.0)
    {
        AmelasError error = AmelasError::INVALID_SPEED;

        // Log.
        std::ostringstream oss;
        oss << "There is no speed defined in SlewSpeed." << '\n';
        setLog(command, oss.str(), error);
    }
    else
    {
        // Do things in the hardware (PLC).
        // _plc->executeCommand(symbol);
        _plc->write(symbol + ".cmd", true);

        // Log.
        setLog(command, "", error);
    }
    
    return error;
}

AmelasError AmelasController::setCalibrationMotion()
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.commander.moveToCalibrationCmd";
    const std::string command = "SET_Calibration_MOTION";

    if (_plc->read<double>("MAIN.commander.SlewSpeed.Azimuth") == 0.0
    || _plc->read<double>("MAIN.commander.SlewSpeed.Elevation") == 0.0)
    {
        AmelasError error = AmelasError::INVALID_SPEED;

        // Log.
        std::ostringstream oss;
        oss << "There is no speed defined in SlewSpeed." << '\n';
        setLog(command, oss.str(), error);
    }
    else
    {
        // Do things in the hardware (PLC).
        // _plc->executeCommand(symbol);
        _plc->write(symbol + ".cmd", true);

        // Log.
        setLog(command, "", error);
    }
    
    return error;
}

// TODO: AmelasError AmelasController::setCPFMotion(const file &cpf, AmelasTracking &tracking)
// TODO: AmelasError AmelasController::setStarMotion(const StarData &star_data)
// =====================================================================================================================


}} // END NAMESPACES.
// =====================================================================================================================
