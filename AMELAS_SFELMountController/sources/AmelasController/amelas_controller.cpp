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
    tracking_adjusts_(false),
    mount_power_(false),
    slew_speed_(-1,-1),
    home_pos_(-1,-1),
    idle_pos_(-1,-1),
    park_pos_(-1,-1),
    calibration_pos_(-1,-1),
    home_pos_offset_(-1,-1),
    wait_alt_(-1),
    mount_model_(false),
    meteo_(-1,-1,-1),
    track_pos_offset_(-1,-1),
    track_time_bias_(-1),
    _logger(logger)
{
    _plc = std::make_shared<amelas::AmelasAdsClient>(_config.plcConfig, _logger); 
}

// STATUS AND CONFIGURATION RELATED FUNCTIONS
//=====================================================================================================================
AmelasError AmelasController::getDatetime(std::string &)
{
    return AmelasError::SUCCESS;
}

void AmelasController::setLog(const std::string command, const std::string specific, const AmelasError error)
{
    std::string cmd_str = ControllerErrorStr[static_cast<size_t>(error)];
    std::ostringstream oss;
    oss << std::string(100, '-') << '\n'
    << "<AMELAS CONTROLLER>" << '\n'
    << "-> " << command << '\n'
    << "Time: " << zmqutils::utils::currentISO8601Date() << '\n'
    << specific
    << "Error: " << static_cast<int>(error) << " (" << cmd_str << ")" << '\n'
    << std::string(100, '-') << '\n';
    _logger->error(oss.str());
}

AmelasError AmelasController::setEnable(const bool& enable, const std::string plcSymbol, const std::string command)
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    std::ostringstream oss;

    // TODO: Check the provided values
    if (command == "EN_TRACK_ADJ")
    {
        this->tracking_adjusts_ = enable;
        oss << "Track adj.: " << enable << '\n';
    }
    else if (command == "EN_MOUNT_POWER")
    {
        this->mount_power_ = enable;
        oss << "Mount power: " << enable << '\n';
    }
    else if (command == "EN_MOUNT_MODEL")
    {
        this->mount_model_ = enable;
        oss << "Mount model: " << enable << '\n';
    }

    // Do things in the hardware (PLC).
    _plc->write(plcSymbol, enable);

    // Log.
    setLog(command, oss.str(), error);

    return error;
}

AmelasError AmelasController::setPosition(const AltAzPos& pos, const std::string plcSymbol, const std::string command)
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
        if (command == "SET_HOME_POSITION")
            this->home_pos_ = pos;
        else if (command == "SET_IDLE_POSITION")
            this->idle_pos_ = pos;
        else if (command == "SET_PARK_POSITION")
            this->park_pos_ = pos;
        else if (command == "SET_CALIBRATION_POSITION")
            this->calibration_pos_ = pos;
        else if (command == "SET_HOMING_OFFSETS")
            this->home_pos_offset_ = pos;
        else if (command == "SET_TRACK_POS_OFFSET")
            this->track_pos_offset_ = pos;
        
        // Do things in the hardware (PLC).
        _plc->write(plcSymbol + ".az", pos.az);
        _plc->write(plcSymbol + ".el", pos.el);
    }

    // Log.
    std::ostringstream oss;
    oss << "Az: " << pos.az << '\n'
    << "El: " << pos.el << '\n';
    setLog(command, oss.str(), error);

    return error;
}

AmelasError AmelasController::getPosition(AltAzPos& pos, const std::string plcSymbol, const std::string command)
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    if (command == "GET_HOME_POSITION")
        pos = this->home_pos_;
    else if (command == "GET_IDLE_POSITION")
        pos = this->idle_pos_;
    else if (command == "GET_PARK_POSITION")
        pos = this->park_pos_;
    else if (command == "GET_CALIBRATION_POSITION")
        pos = this->calibration_pos_;
    else if (command == "GET_HOMING_OFFSETS")
        pos = this->home_pos_offset_;
    else if (command == "GET_SLEW_SPEED")
        pos = this->slew_speed_;
    else if (command == "GET_TRACK_POS_OFFSET")
        pos = this->track_pos_offset_;

    // Log.
    setLog(command, "", error);
    
    return error;
}

AmelasError AmelasController::setSpeed(const AltAzVel& vel, const std::string plcSymbol, const std::string command)
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
        if (command == "SET_SLEW_SPEED")
            this->slew_speed_ = vel;
        
        // Do things in the hardware (PLC).
        _plc->write(plcSymbol + ".az", vel.az);
        _plc->write(plcSymbol + ".el", vel.el);
    }

    // Log.
    std::ostringstream oss;
    oss << "Az: " << vel.az << '\n'
    << "El: " << vel.el << '\n';
    setLog(command, oss.str(), error);

    return error;
}

AmelasError AmelasController::getSpeed(AltAzVel& vel, const std::string plcSymbol, const std::string command)
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    if (command == "GET_SLEW_SPEED")
        vel = this->slew_speed_;

    // Log.
    setLog(command, "", error);
    
    return error;
}

AmelasError AmelasController::enableTrackingAdjusts(const bool& enable)
{
    const std::string symbol = "MAIN.TrackingAdjuts";
    const std::string command = "EN_TRACK_ADJ";
    setEnable(enable, symbol, command);
}

AmelasError AmelasController::enableMountPower(const bool& enable)
{
    const std::string symbol = "MAIN.MountPower";
    const std::string command = "EN_MOUNT_POWER";
    setEnable(enable, symbol, command);
}

AmelasError AmelasController::setSlewSpeed(const AltAzVel &vel)
{
    const std::string symbol = "MAIN.SlewSpeed";
    const std::string command = "SET_SLEW_SPEED";
    return setSpeed(vel, symbol, command);
}

AmelasError AmelasController::getSlewSpeed(AltAzVel &vel)
{
    const std::string symbol = "MAIN.SlewSpeed";
    const std::string command = "GET_SLEW_SPEED";
    return getSpeed(vel, symbol, command);
}

AmelasError AmelasController::setHomePosition(const AltAzPos &pos)
{
    const std::string symbol = "MAIN.HomePosition";
    const std::string command = "SET_HOME_POSITION";
    return setPosition(pos, symbol, command);
}

AmelasError AmelasController::getHomePosition(AltAzPos &pos)
{
    const std::string symbol = "MAIN.HomePosition";
    const std::string command = "GET_HOME_POSITION";
    return getPosition(pos, symbol, command);
}

AmelasError AmelasController::setIdlePosition(const AltAzPos &pos)
{
    const std::string symbol = "MAIN.IdlePosition";
    const std::string command = "SET_IDLE_POSITION";
    return setPosition(pos, symbol, command);
}

AmelasError AmelasController::getIdlePosition(AltAzPos &pos)
{
    const std::string symbol = "MAIN.IdlePosition";
    const std::string command = "GET_IDLE_POSITION";
    return getPosition(pos, symbol, command);
}

AmelasError AmelasController::setParkPosition(const AltAzPos &pos)
{
    const std::string symbol = "MAIN.ParkPosition";
    const std::string command = "SET_PARK_POSITION";
    return setPosition(pos, symbol, command);
}

AmelasError AmelasController::getParkPosition(AltAzPos &pos)
{
    const std::string symbol = "MAIN.ParkPosition";
    const std::string command = "GET_PARK_POSITION";
    return getPosition(pos, symbol, command);
}

AmelasError AmelasController::setCalibrationPosition(const AltAzPos &pos)
{
    const std::string symbol = "MAIN.CalibrationPosition";
    const std::string command = "SET_CALIBRATION_POSITION";
    return setPosition(pos, symbol, command);
}

AmelasError AmelasController::getCalibrationPosition(AltAzPos &pos)
{
    const std::string symbol = "MAIN.CalibrationPosition";
    const std::string command = "GET_CALIBRATION_POSITION";
    return getPosition(pos, symbol, command);
}

AmelasError AmelasController::setIdlePositionHere()
{
    AltAzPos pos(_plc->read<double>("MAIN.CurrentPosition.az"), _plc->read<double>("MAIN.CurrentPosition.el"));
    return setIdlePosition(pos);
}

AmelasError AmelasController::setParkPositionHere()
{
    AltAzPos pos(_plc->read<double>("MAIN.CurrentPosition.az"), _plc->read<double>("MAIN.CurrentPosition.el"));
    return setParkPosition(pos);
}

AmelasError AmelasController::setCalibrationPositionHere()
{
    AltAzPos pos(_plc->read<double>("MAIN.CurrentPosition.az"), _plc->read<double>("MAIN.CurrentPosition.el"));
    return setCalibrationPosition(pos);
}

AmelasError AmelasController::setHomingOffsets(const AltAzAdj& pos)
{
    const std::string symbol = "MAIN.HomePositionOffsets";
    const std::string command = "SET_HOMING_OFFSETS";
    return setPosition(pos, symbol, command);
}

AmelasError AmelasController::getHomingOffsets(AltAzAdj &pos)
{
    const std::string symbol = "MAIN.HomePositionOffsets";
    const std::string command = "GET_HOMING_OFFSETS";
    return getPosition(pos, symbol, command);
}

AmelasError AmelasController::setWaitAlt(const double& alt)
{
    const std::string symbol = "MAIN.WaitAlt";
    const std::string command = "SET_WAIT_ALT";

    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    // TODO: Check the provided values
    this->wait_alt_ = alt;

    // Do things in the hardware (PLC).
    _plc->write(symbol, alt);

    // Log.
    std::ostringstream oss;
    oss << "Alt: " << alt << '\n';
    setLog(command, oss.str(), error);

    return error;
}

AmelasError AmelasController::getWaitAlt(double& alt)
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.WaitAlt";
    const std::string command = "GET_WAIT_ALT";

    alt = this->wait_alt_;

    // Log.
    setLog(command, "", error);
    
    return error;
}

AmelasError AmelasController::enableMountModel(const bool& enable)
{
    const std::string symbol = "MAIN.MountModel";
    const std::string command = "EN_MOUNT_MODEL";
    setEnable(enable, symbol, command);
}

AmelasError AmelasController::setMeteoData(const MeteoData& meteo)
{
    const std::string symbol = "MAIN.MeteoData";
    const std::string command = "SET_METEO_DATA";

    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    this->meteo_.press = _plc->read<double>(symbol + ".press");
    this->meteo_.temp  = _plc->read<double>(symbol + ".temp");
    this->meteo_.hr    = _plc->read<double>(symbol + ".hr");

    // Log.
    std::ostringstream oss;
    oss << "Press: " << meteo.press << '\n'
    << "Temp: " << meteo.temp << '\n'
    << "HR: " << meteo.hr << '\n';
    setLog(command, oss.str(), error);

    return error;
}

AmelasError AmelasController::getMeteoData(MeteoData& meteo)
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.MeteoData";
    const std::string command = "SET_METEO_DATA";

    meteo = this->meteo_;

    // Log.
    setLog(command, "", error);
    
    return error;
}
//=====================================================================================================================

// STATUS AND CONFIGURATION RELATED FUNCTIONS
//=====================================================================================================================
AmelasError AmelasController::doStartMotion()
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.StartMotion";
    const std::string command = "DO_START_MOTION";

    // Do things in the hardware (PLC).
    _plc->write(symbol, true);

    // Log.
    setLog(command, "", error);
    
    return error;
}

AmelasError AmelasController::doPauseMotion()
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.PauseMotion";
    const std::string command = "DO_PAUSE_MOTION";

    // Do things in the hardware (PLC).
    _plc->write(symbol, true);

    // Log.
    setLog(command, "", error);
    
    return error;
}

AmelasError AmelasController::doStopMotion()
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.StopMotion";
    const std::string command = "DO_STOP_MOTION";

    // Do things in the hardware (PLC).
    _plc->write(symbol, true);

    // TODO: borrar todo lo relativo al movimiento actual

    // Log.
    setLog(command, "", error);
    
    return error;
}

AmelasError AmelasController::setTrackPosOffset(const AltAzAdj& pos)
{
    const std::string symbol = "MAIN.TrackPosOffset";
    const std::string command = "SET_TRACK_POS_OFFSET";
    return setPosition(pos, symbol, command);
}

AmelasError AmelasController::getTrackPosOffset(AltAzAdj& pos)
{
    const std::string symbol = "MAIN.TrackPosOffset";
    const std::string command = "GET_TRACK_POS_OFFSET";
    return getPosition(pos, symbol, command);
}

AmelasError AmelasController::setTrackTimeBias(const double& time)
{
    const std::string symbol = "MAIN.TrackTimeBias";
    const std::string command = "SET_TRACK_TIME_BIAS";

    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    // TODO: Check the provided values
    this->track_time_bias_ = time;

    // Do things in the hardware (PLC).
    _plc->write(symbol, time);

    // Log.
    std::ostringstream oss;
    oss << "Track time bias: " << time << '\n';
    setLog(command, oss.str(), error);

    return error;
}

AmelasError AmelasController::getTrackTimeBias(double& time)
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.TrackTimeBias";
    const std::string command = "GET_TRACK_TIME_BIAS";

    time = this->track_time_bias_;

    // Log.
    setLog(command, "", error);
    
    return error;
}

AmelasError AmelasController::setAbsoluteAltAzMotion(const AltAzPos& pos, const AltAzVel& vel)
{
    // Auxiliar result.
    AmelasError error_pos = AmelasError::SUCCESS;
    AmelasError error_vel = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.AbsoluteMotion";
    const std::string symbol_pos = "MAIN.AltAzMotion.pos";
    const std::string symbol_vel = "MAIN.AltAzMotion.vel";
    const std::string command_pos = "SET_ABS_ALTAZ_MOTION (POS)";
    const std::string command_vel = "SET_ABS_ALTAZ_MOTION (VEL)";

    error_pos = setPosition(pos, symbol_pos, command_pos);
    error_vel = setSpeed(vel, symbol_vel, command_vel);

    if (error_pos == AmelasError::SUCCESS && error_vel == AmelasError::SUCCESS)
    {
        // Do things in the hardware (PLC).
        _plc->write(symbol, true);
        return AmelasError::SUCCESS;
    }
    else if (error_pos == AmelasError::INVALID_POSITION && error_vel == AmelasError::SUCCESS)
        return AmelasError::INVALID_POSITION;
    else if (error_pos == AmelasError::SUCCESS && error_vel == AmelasError::INVALID_SPEED)
        return AmelasError::INVALID_SPEED;
    else
        return AmelasError::INVALID_ERROR;
}

AmelasError AmelasController::setRelativeAltAzMotion(const AltAzPos& pos, const AltAzVel& vel)
{
    // Auxiliar result.
    AmelasError error_pos = AmelasError::SUCCESS;
    AmelasError error_vel = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.RelativeMotion";
    const std::string symbol_pos = "MAIN.AltAzMotion.pos";
    const std::string symbol_vel = "MAIN.AltAzMotion.vel";
    const std::string command_pos = "SET_REL_ALTAZ_MOTION (POS)";
    const std::string command_vel = "SET_REL_ALTAZ_MOTION (VEL)";

    error_pos = setPosition(pos, symbol_pos, command_pos);
    error_vel = setSpeed(vel, symbol_vel, command_vel);

    if (error_pos == AmelasError::SUCCESS && error_vel == AmelasError::SUCCESS)
    {
        // Do things in the hardware (PLC).
        _plc->write(symbol, true);
        return AmelasError::SUCCESS;
    }
    else if (error_pos == AmelasError::INVALID_POSITION && error_vel == AmelasError::SUCCESS)
        return AmelasError::INVALID_POSITION;
    else if (error_pos == AmelasError::SUCCESS && error_vel == AmelasError::INVALID_SPEED)
        return AmelasError::INVALID_SPEED;
    else
        return AmelasError::INVALID_ERROR;
}

AmelasError AmelasController::setIdleMotion()
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.IdleMotion";
    const std::string command = "SET_IDLE_MOTION";

    // Do things in the hardware (PLC).
    _plc->write(symbol, true);

    // Log.
    setLog(command, "", error);
    
    return error;
}

AmelasError AmelasController::setParkMotion()
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.ParkMotion";
    const std::string command = "SET_Park_MOTION";

    // Do things in the hardware (PLC).
    _plc->write(symbol, true);

    // Log.
    setLog(command, "", error);
    
    return error;
}

AmelasError AmelasController::setCalibrationMotion()
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    const std::string symbol = "MAIN.CalibrationMotion";
    const std::string command = "SET_Calibration_MOTION";

    // Do things in the hardware (PLC).
    _plc->write(symbol, true);

    // Log.
    setLog(command, "", error);
    
    return error;
}

// =====================================================================================================================

}} // END NAMESPACES.
// =====================================================================================================================
