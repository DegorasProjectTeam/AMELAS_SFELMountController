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
// TODO: AmelasError AmelasController::doResetState()
// TODO: AmelasError AmelasController::enableAvoidSun(const bool &enabled)
//=====================================================================================================================


// LOW LEVEL PLC REGISTERS RELATED FUNCTIONS
//=====================================================================================================================
AmelasError AmelasController::doConnectPLC(AltAzPos &pos)
{
    _plc->connect();

    return AmelasError::SUCCESS;
}

// TODO: AmelasError AmelasController::doConnectPLC()
// TODO: AmelasError AmelasController::doDisconnectPLC()
// TODO: AmelasError AmelasController::getPLCregister(const PLCAddress &address, PLCRegisterValue &value)

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
    const std::string symbol = "MAIN.commander.MountModel";
    const std::string command = "EN_MOUNT_MODEL";
    return setEnable(enabled, symbol, command);
}

// TODO: AmelasError AmelasController::setMountModelCoefs(const MountModelCoefs &coefs)
// TODO: AmelasError AmelasController::getMountModelCoefs(MountModelCoefs &coefs)
// TODO: AmelasError AmelasController::setLocation(const MountLocation &location)
// TODO: AmelasError AmelasController::getLocation(MountLocation &location)

AmelasError AmelasController::setMeteoData(const MeteoData &meteo)
{
    const std::string symbol = "MAIN.commander.MeteoData";
    const std::string command = "SET_METEO_DATA";

    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    // TODO

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

    const std::string symbol = "MAIN.commander.MeteoData";
    const std::string command = "SET_METEO_DATA";

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
// TODO: AmelasError AmelasController::getMotionMode(AmelasMotionMode& motion_mode)
// TODO: AmelasError AmelasController::getMotionState(AmelasMotionState& motion_state)

AmelasError AmelasController::doStartMotion()
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.commander.StartMotion";
    const std::string command = "DO_START_MOTION";

    // Do things in the hardware (PLC).
    // _plc->write(symbol, true);
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
    // _plc->write(symbol, true);
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
    // _plc->write(symbol, true);
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

    const std::string symbol = "MAIN.commander.moveAbsoluteCmd";
    const std::string symbol_pos = "MAIN.commander.moveAbsolutePosition";
    const std::string symbol_vel = "MAIN.commander.moveAbsoluteVelocity";
    const std::string command_pos = "SET_ABS_ALTAZ_MOTION (POS)";
    const std::string command_vel = "SET_ABS_ALTAZ_MOTION (VEL)";

    error_pos = setPosition(pos, symbol_pos, command_pos);
    error_vel = setSpeed(vel, symbol_vel, command_vel);

    if (error_pos == AmelasError::SUCCESS && error_vel == AmelasError::SUCCESS)
    {
        // Do things in the hardware (PLC).
        // _plc->write(symbol, true);
        // _plc->executeCommand(symbol);
        _plc->write(symbol + ".cmd", true);
        AmelasMotionMode ::ABSOLUTE_MOTION;
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
    AmelasError error_pos = AmelasError::SUCCESS;
    AmelasError error_vel = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.commander.moveRelativeCmd";
    const std::string symbol_pos = "MAIN.commander.moveRelativePosition";
    const std::string symbol_vel = "MAIN.commander.moveRelativeVelocity";
    const std::string command_pos = "SET_REL_ALTAZ_MOTION (POS)";
    const std::string command_vel = "SET_REL_ALTAZ_MOTION (VEL)";

    error_pos = setPosition(pos, symbol_pos, command_pos);
    error_vel = setSpeed(vel, symbol_vel, command_vel);

    if (error_pos == AmelasError::SUCCESS && error_vel == AmelasError::SUCCESS)
    {
        // Do things in the hardware (PLC).
        // _plc->write(symbol, true);
        // _plc->executeCommand(symbol);
        _plc->write(symbol + ".cmd", true);
        AmelasMotionMode ::RELATIVE_MOTION;
        return AmelasError::SUCCESS;
    }
    else if (error_pos == AmelasError::INVALID_POSITION && error_vel == AmelasError::SUCCESS)
        return AmelasError::INVALID_POSITION;
    else if (error_pos == AmelasError::SUCCESS && error_vel == AmelasError::INVALID_SPEED)
        return AmelasError::INVALID_SPEED;
    else
        return AmelasError::INVALID_ERROR;
}

AmelasError AmelasController::setContAltAzMotion(const AltAzVel &vel)
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.commander.moveVelocityCmd";
    const std::string symbol_vel = "MAIN.commander.moveVelocity";
    const std::string command_vel = "SET_REL_ALTAZ_MOTION (VEL)";

    // Do things in the hardware (PLC).
    _plc->write(symbol_vel + ".Azimuth", vel.az);
    _plc->write(symbol_vel + ".Elevation", vel.el);
    _plc->write(symbol + ".cmd", true);

    AmelasMotionMode::CONTINUOUS;

    // Log.
    std::ostringstream oss;
    oss << "Az: " << vel.az << '\n'
    << "El: " << vel.el << '\n';
    setLog(command_vel, oss.str(), error);

    return error;
}

// TODO: AmelasError AmelasController::setHomingMotion()

AmelasError AmelasController::setIdleMotion()
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.commander.moveToIdleCmd";
    const std::string command = "SET_IDLE_MOTION";

    // Do things in the hardware (PLC).
    // _plc->write(symbol, true);
    // _plc->executeCommand(symbol);
    _plc->write(symbol + ".cmd", true);
    AmelasMotionMode ::TO_IDLE;

    // Log.
    setLog(command, "", error);
    
    return error;
}

AmelasError AmelasController::setParkMotion()
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.commander.moveToParkCmd";
    const std::string command = "SET_Park_MOTION";

    // Do things in the hardware (PLC).
    // _plc->write(symbol, true);
    // _plc->executeCommand(symbol);
    _plc->write(symbol + ".cmd", true);
    AmelasMotionMode ::TO_PARK;

    // Log.
    setLog(command, "", error);
    
    return error;
}

AmelasError AmelasController::setCalibrationMotion()
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.commander.moveToCalibrationCmd";
    const std::string command = "SET_Calibration_MOTION";

    // Do things in the hardware (PLC).
    // _plc->write(symbol, true);
    // _plc->executeCommand(symbol);
    _plc->write(symbol + ".cmd", true);
    AmelasMotionMode ::TO_CALIBRATION;

    // Log.
    setLog(command, "", error);
    
    return error;
}

// TODO: AmelasError AmelasController::setCPFMotion(const file &cpf, AmelasTracking &tracking)
// TODO: AmelasError AmelasController::setStarMotion(const StarData &star_data)
// =====================================================================================================================


}} // END NAMESPACES.
// =====================================================================================================================
