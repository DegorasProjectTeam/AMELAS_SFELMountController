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

AmelasError AmelasController::enableAvoidSun(const bool &enabled)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "EN_AVOID_SUN";

    // Variable used for this function
    std::ostringstream oss;

    // Functionality
    if (_avoid_sun == enabled)
    {
        oss << "Avoid Sun is already set to " << enabled << "." << '\n';

        error = AmelasError::ENABLE_WARN;
    }
    else
    {
        oss << "Avoid Sun" << '\n'
            << "  Old: " << _avoid_sun << '\n'
            << "  New: " << enabled    << '\n';

        _avoid_sun = enabled;
    }

    // Log
    setLog(command, oss.str(), error);

    return error;
}
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
AmelasError AmelasController::getPLCregister(const PLCAddress &address, PLCRegisterValue &registerValue)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "GET_PLC_REGISTER";

    // Functionality
    registerValue.symbol = address.symbol;
    registerValue.type = address.type;

    if (address.type == "double")
        registerValue.value = std::to_string(_plc->read<double>(address.symbol));
    else if (address.type == "int")
        registerValue.value = std::to_string(_plc->read<short int>(address.symbol));
    else if (address.type == "bool")
        registerValue.value = std::to_string(_plc->read<bool>(address.symbol));

    // Log
    std::ostringstream oss;
    oss << "Symbol: " << registerValue.symbol << " <" << registerValue.type << "> = " << registerValue.value << '\n';
    setLog(command, oss.str(), error);

    return error;
}

AmelasError AmelasController::getPLCprueba(const std::string &symbol, const std::string &type)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "GET_PLC_REGISTER";

    // Variable used for this function
    std::string value;

    // Functionality
    if (type == "double")
        value = std::to_string(_plc->read<double>(symbol));
    else if (type == "int")
        value = std::to_string(_plc->read<short int>(symbol));
    else if (type == "bool")
        value = std::to_string(_plc->read<bool>(symbol));

    // Log
    std::ostringstream oss;
    oss << "Symbol: " << symbol << " <" << type << "> = " << value << '\n';
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
    else if (error == AmelasError::MOUNT_UNSAFE_STATE || error == AmelasError::UNSAFE_POSITION || error == AmelasError::UNSAFE_SPEED || error == AmelasError::STAR_UNSAFE || error == AmelasError::ENABLE_WARN || error == AmelasError::START_WARN || error == AmelasError::STOP_WARN)
        oss_1 << std::string(90, '-') << '\n';
    else
        oss_1 << std::string(92, '-') << '\n';

    // Log message generation
    std::ostringstream oss;
    oss << oss_1.str()
        << "<AMELAS CONTROLLER>" << '\n'
        << "-> " << command << '\n'
        << "Time: " << zmqutils::utils::currentISO8601Date() << '\n'
        << specific;

    std::ostringstream oss_2;
    oss_2 << "Error: " << static_cast<int>(error) << " (" << ControllerErrorStr[static_cast<size_t>(error)] << ")" << '\n'
          << std::string(100, '-') << '\n';

    // Log output
    if (error == AmelasError::SUCCESS)
        _logger->info(oss.str() + oss_2.str());
    else if (error == AmelasError::MOUNT_UNSAFE_STATE || error == AmelasError::UNSAFE_POSITION || error == AmelasError::UNSAFE_SPEED || error == AmelasError::STAR_UNSAFE)
        _logger->warn(oss.str() + oss_2.str());
    else if (error == AmelasError::ENABLE_WARN || error == AmelasError::START_WARN || error == AmelasError::STOP_WARN)
        _logger->warn(oss.str() + std::string(100, '-') + '\n');
    else
        _logger->error(oss.str() + oss_2.str());
}

AmelasMotionMode AmelasController::motionModes()
{
    // Symbol used for PLC
    const std::string symbol = "MAIN.commander._motionMode";

    // Variables used for this function
    AmelasMotionMode motion_mode;

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

    return motion_mode;
}

AmelasMotionState AmelasController::motionStates()
{
    // Symbols used for PLC
    const std::string sState     = "MAIN.axesController._motionState";
    const std::string sActPosAz  = "MAIN.axesController._azimuthAxis._axis.NcToPlc.ActPos";
    const std::string sActPosEl  = "MAIN.axesController._elevationAxis._axis.NcToPlc.ActPos";

    // Variables used for this function
    AmelasMotionState motion_state;

    // Functionality
    if (_plc->read<double>(sState) == 4.0      // Homing
        || _plc->read<double>(sState) == 6.0   // MovingAbsolute
        || _plc->read<double>(sState) == 7.0   // MovingRelative
        || _plc->read<double>(sState) == 13.0) // MovingVelocity
        motion_state = AmelasMotionState::MOVING;
    else if (_plc->read<double>(sState) == 8.0) // Halting
        motion_state = AmelasMotionState::PAUSED;
    else if (motionModes() == AmelasMotionMode::ABSOLUTE_MOTION   // LoadMoveAbsolute
            || motionModes() == AmelasMotionMode::RELATIVE_MOTION // LoadMoveRelative
            || motionModes() == AmelasMotionMode::CONTINUOUS      // LoadMoveVelocity
            || motionModes() == AmelasMotionMode::HOMING_OP       // LoadMoveToHome
            || motionModes() == AmelasMotionMode::TO_IDLE         // LoadMoveToIdle
            || motionModes() == AmelasMotionMode::TO_PARK         // LoadMoveToPark
            || motionModes() == AmelasMotionMode::TO_CALIBRATION  // LoadMoveToCalibration
            || motionModes() == AmelasMotionMode::CPF             // LoadMoveCPFMotion
            || motionModes() == AmelasMotionMode::STAR)           // LoadMoveStarMotion
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

    return motion_state;
}

AmelasError AmelasController::setEnable(const bool &enabled, const std::string plcSymbol, const std::string command)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Variable used for this function
    std::ostringstream oss;
    bool oldEn;

    // Modifying log message
    if (_plc->read<bool>(plcSymbol) == enabled)
    {
        std::string result;
        std::string prefix = "MAIN.commander.enable";
        size_t pos = plcSymbol.find(prefix);

        if (pos != std::string::npos)
            result = plcSymbol.substr(pos + prefix.length());

        oss << result << " is already set to " << enabled << "." << '\n';

        error = AmelasError::ENABLE_WARN;
    }
    else
    {
        oldEn = _plc->read<bool>(plcSymbol);

        if (command == "EN_TRACK_ADJ")
        {
            oss << "Track adj" << '\n'
                << "  Old: " << oldEn   << '\n'
                << "  New: " << enabled << '\n';
        }
        else if (command == "EN_MOUNT_POWER")
        {
            oss << "Mount power" << '\n'
                << "  Old: " << oldEn   << '\n'
                << "  New: " << enabled << '\n';
        }
        else if (command == "EN_MOUNT_MODEL")
        {
            oss << "Mount model" << '\n'
                << "  Old: " << oldEn   << '\n'
                << "  New: " << enabled << '\n';
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

    // Variables used for this function
    AltAzPos oldPos;

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
        /*oldPos.az = _plc->read<double>(plcSymbol + ".Azimuth");
        oldPos.el = _plc->read<double>(plcSymbol + ".Elevation");
        _plc->write(plcSymbol + ".Azimuth", pos.az);
        _plc->write(plcSymbol + ".Elevation", pos.el);*/
    }

    if (motionStates() == AmelasMotionState::MOVING)
    {
        error = AmelasError::MOUNT_UNSAFE_STATE;
    }

    oldPos.az = _plc->read<double>(plcSymbol + ".Azimuth");
    oldPos.el = _plc->read<double>(plcSymbol + ".Elevation");

    _plc->write(plcSymbol + ".Azimuth", pos.az);
    _plc->write(plcSymbol + ".Elevation", pos.el);

    // Log
    std::ostringstream oss;    
    oss << "Old:" << '\n'
        << "  Az: " << oldPos.az << " \370" << '\n'
        << "  El: " << oldPos.el << " \370" << '\n'
        << "New:" << '\n'
        << "  Az: " << pos.az << " \370" << '\n'
        << "  El: " << pos.el << " \370" << '\n';
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
    std::ostringstream oss;
    oss << "  Az: " << pos.az << " \370" << '\n'
        << "  El: " << pos.el << " \370" << '\n';
    setLog(command, oss.str(), error);
    
    return error;
}

AmelasError AmelasController::setSpeed(const AltAzVel &vel, const std::string plcSymbol, const std::string command)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Variables used for this function
    AltAzVel oldSpeed;

    // Check the provided values
    if (vel.az < 0.0 ||  vel.el < 0.0)
    {
        error = AmelasError::INVALID_SPEED;
    }
    else
    {
        // Functionality
        oldSpeed.az = _plc->read<double>(plcSymbol + ".Azimuth");
        oldSpeed.el = _plc->read<double>(plcSymbol + ".Elevation");

        _plc->write(plcSymbol + ".Azimuth", vel.az);
        _plc->write(plcSymbol + ".Elevation", vel.el);
    }

    if (motionStates() == AmelasMotionState::MOVING)
    {
        error = AmelasError::MOUNT_UNSAFE_STATE;
    }

    // Log
    std::ostringstream oss;
    oss << "Old:" << '\n'
        << "  Az: " << oldSpeed.az << " \370/s" << '\n'
        << "  El: " << oldSpeed.el << " \370/s" << '\n'
        << "New:" << '\n'
        << "  Az: " << vel.az << " \370/s" << '\n'
        << "  El: " << vel.el << " \370/s" << '\n';
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
    std::ostringstream oss;
    oss << "  Az: " << vel.az << " \370/s" << '\n'
        << "  El: " << vel.el << " \370/s" << '\n';
    setLog(command, oss.str(), error);
    
    return error;
}

double AmelasController::deg_to_radians(const double &degrees)
{
    return degrees * (M_PI / 180.0);
}

double AmelasController::arcsec_to_radians(const double &arcsec)
{
    return arcsec * (M_PI / 648000.0);
}

double AmelasController::arcsec_to_deg(const double &arcsec)
{
    return arcsec * (1 / 3600.0);
}

AmelasError AmelasController::getMountLog(const std::string &day)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "GET_MOUNT_LOG";

    // Variables used for this function
    std::string file = "./logs/daily_" + day + ".txt";
    std::ifstream logFile;
    std::string logRead;
    std::string strLog = "";

    // Functionality
    logFile.open(file);

    if (logFile.is_open())
    {
        while (logFile)
        {
            std::getline (logFile, logRead);
            std::cout << logRead << '\n';
            strLog = strLog + logRead + '\n';
        }
        std::cout << '\n';
    }
    else
    {
        error = AmelasError::FILE_ERROR;
    }

    logFile.close();

    // Log
    std::ostringstream oss;
    oss << "File: daily_" << day << ".txt" << '\n';
    setLog(command, oss.str(), error);

    return error;
}

// TODO: AmelasError AmelasController::doSyncTimeNTP(const std::string &host, const unsigned &port, const unsigned &timeout)

// TODO
AmelasError AmelasController::doSyncTimeNTP(const std::string &host, const unsigned &port)
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
    _plc->write<unsigned short int>(symbolPort, port);

    // Log
    std::ostringstream oss;
    oss << "Server name: " << host << '\n'
        << "Server port: " << port << '\n';
    setLog(command, oss.str(), error);

    return error;
}

// TODO: AmelasError AmelasController::doSyncTimeManual(const std::string &datetime)

// TODO
AmelasError AmelasController::getMountStatus(std::string &mountStatus)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "GET_MOUNT_STATUS";

    // Variables used for this function
    std::ostringstream oss;
    AltAzPos actPos(_plc->read<double>("MAIN.axesController._azimuthAxis._axis.NcToPlc.ActPos"), _plc->read<double>("MAIN.axesController._elevationAxis._axis.NcToPlc.ActPos"));
    AltAzVel actVel(_plc->read<double>("MAIN.axesController._azimuthAxis._axis.NcToPlc.ActVelo"), _plc->read<double>("MAIN.axesController._elevationAxis._axis.NcToPlc.ActVelo"));
    // -- Related to azimuth
    bool azEnabled     = _plc->read<bool>("MAIN.axesController._azimuthAxis._enable");
    bool azMoving      = _plc->read<bool>("MAIN.axesController._azimuthAxis._fbPower.Axis.Status.Moving");
    bool azPositiveDir = _plc->read<bool>("MAIN.axesController._azimuthAxis._fbPower.Axis.Status.PositiveDirection");
    bool azNegativeDir = _plc->read<bool>("MAIN.axesController._azimuthAxis._fbPower.Axis.Status.NegativeDirection");
    bool azInTargetPos = _plc->read<bool>("MAIN.axesController._azimuthAxis._fbPower.Axis.Status.InTargetPosition");
    // -- Related to elevation
    bool elEnabled     = _plc->read<bool>("MAIN.axesController._elevationAxis._enable");
    bool elMoving      = _plc->read<bool>("MAIN.axesController._elevationAxis._fbPower.Axis.Status.Moving");
    bool elPositiveDir = _plc->read<bool>("MAIN.axesController._elevationAxis._fbPower.Axis.Status.PositiveDirection");
    bool elNegativeDir = _plc->read<bool>("MAIN.axesController._elevationAxis._fbPower.Axis.Status.NegativeDirection");
    bool elInTargetPos = _plc->read<bool>("MAIN.axesController._elevationAxis._fbPower.Axis.Status.InTargetPosition");

    // Functionality
    oss << "STATUS INFO"                        << '\n'
        << "  Actual position:"                 << '\n'
        << "    Az: " << actPos.az << " \370"   << '\n'
        << "    El: " << actPos.el << " \370"   << '\n'
        << ""                                   << '\n'
        << "  Actual velocity:"                 << '\n'
        << "    Az: " << actVel.az << " \370/s" << '\n'
        << "    El: " << actVel.el << " \370/s" << '\n'
        << ""                                   << '\n'
        << "  Azimuth axis:"                    << '\n'
        << "    Enabled:     " << azEnabled     << '\n'
        << "    Moving:      " << azMoving      << '\n'
        << "    InTargetPos: " << azInTargetPos << '\n'
        << "    Positive:    " << azPositiveDir << '\n'
        << "    Negative:    " << azNegativeDir << '\n'
        << ""                                   << '\n'
        << "  Elevation axis:"                  << '\n'
        << "    Enabled:     " << elEnabled     << '\n'
        << "    Moving:      " << elMoving      << '\n'
        << "    InTargetPos: " << elInTargetPos << '\n'
        << "    Positive:    " << elPositiveDir << '\n'
        << "    Negative:    " << elNegativeDir << '\n';

    mountStatus = oss.str();

    // Log
    setLog(command, oss.str(), error);

    return error;
}

// TODO
AmelasError AmelasController::getDeviceInfo(std::string &deviceInfo)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "GET_MOUNT_INFO";

    // Variables used for this function
    std::ostringstream oss;

    // Functionality
    oss << "MOUNT INFO"                    << '\n'
        << "  C++ software version:     1.9.4" << '\n'
        << "  TwinCAT software version: 1.4.0" << '\n';

    deviceInfo = oss.str();

    // Log
    setLog(command, oss.str(), error);

    return error;
}

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
    oss << "  Alt: " << alt << '\n';
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
    /*// Command used for log
    const std::string command = "EN_MOUNT_MODEL";

    // Symbol used for PLC
    const std::string symbol = "MAIN.commander.enableMountModel";

    // Functionality
    return setEnable(enabled, symbol, command)*/
    
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "EN_MOUNT_MODEL";

    // Variable used for this function
    std::ostringstream oss;

    // Functionality
    if (_enable_mount_model == enabled)
    {
        oss << "Mount model is already set to " << enabled << "." << '\n';

        error = AmelasError::ENABLE_WARN;
    }
    else
    {
        oss << "Mount model" << '\n'
            << "  Old: " << _enable_mount_model << '\n'
            << "  New: " << enabled << '\n';

        _enable_mount_model = enabled;
    }

    // Log
    setLog(command, oss.str(), error);

    return error;
}

// TODO: AmelasError AmelasController::setMountModelCoefs(const MountModelCoefs &coefs)
AmelasError AmelasController::setMountModelCoefs(const double &an, const double &aw, const double &ca, const double &npae, const double &ie, const double &ia)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "SET_MOUNT_MODEL_COEFS";

    // Variable used for this function
    // -- Assign the variables stored internally before the change to the "old" variables
    double old_an = _an_tpoint;
    double old_aw = _aw_tpoint;
    double old_ca = _ca_tpoint;
    double old_npae = _npae_tpoint;
    double old_ie = _ie_tpoint;
    double old_ia = _ia_tpoint;

    // Functionality
    // -- Assigning the input coefficients to the interns
    _an_tpoint = an;
    _aw_tpoint = aw;
    _ca_tpoint = ca;
    _npae_tpoint = npae;
    _ie_tpoint = ie;
    _ia_tpoint = ia;

    // Log
    std::ostringstream oss;
    oss << "Old:" << '\n'
        << "  AN:   " << old_an   << " \"" << '\n'
        << "  AW:   " << old_aw   << " \"" << '\n'
        << "  CA:   " << old_ca   << " \"" << '\n'
        << "  NPAE: " << old_npae << " \"" << '\n'
        << "  IE:   " << old_ie   << " \"" << '\n'
        << "  IA:   " << old_ia   << " \"" << '\n'
        << "New:" << '\n'
        << "  AN:   " << _an_tpoint   << " \"" << '\n'
        << "  AW:   " << _aw_tpoint   << " \"" << '\n'
        << "  CA:   " << _ca_tpoint   << " \"" << '\n'
        << "  NPAE: " << _npae_tpoint << " \"" << '\n'
        << "  IE:   " << _ie_tpoint   << " \"" << '\n'
        << "  IA:   " << _ia_tpoint   << " \"" << '\n';
    setLog(command, oss.str(), error);

    return error;
}

AmelasError AmelasController::setMountModelCoefsFile(const std::string &fileData)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "SET_MOUNT_MODEL_COEFS_FILE";

    // Variables used for this function
    std::string file = "./logs/" + fileData + ".txt";
    std::ifstream logFile;
    std::string logRead;
    int lineCount = 0; // Read line counter
    std::map<std::string, double> coefs; // Stores variables and their values
    std::string coefName;
    double coefVal;
    std::ostringstream oss_aux;
    // -- Assign the variables stored internally before the change to the "old" variables
    double old_an = _an_tpoint;
    double old_aw = _aw_tpoint;
    double old_ca = _ca_tpoint;
    double old_npae = _npae_tpoint;
    double old_ie = _ie_tpoint;
    double old_ia = _ia_tpoint;

    // Functionality
    logFile.open(file);

    if (logFile.is_open())
    {
        while (logFile)
        {
            std::getline (logFile, logRead); // Reads the file line by line
            lineCount++; // Increases the line counter

            // If two lines have already been read, processes the content
            if (lineCount > 2)
            {
                std::istringstream iss(logRead);

                // Reads the name of the variable and its associated value
                if (iss >> coefName >> coefVal)
                    coefs[coefName] = coefVal; // Stores the variable and its associated value in the map
            }
        }
    }
    else
    {
        error = AmelasError::FILE_ERROR;
    }

    logFile.close();

    for (const auto& pair : coefs)
    {
        std::cout << pair.first << ": " << pair.second << std::endl;

        if (pair.first == "AN")
            _an_tpoint = pair.second;
        else if (pair.first == "AW")
            _aw_tpoint = pair.second;
        else if (pair.first == "CA")
            _ca_tpoint = pair.second;
        else if (pair.first == "NPAE")
            _npae_tpoint = pair.second;
        else if (pair.first == "IE")
            _ie_tpoint = pair.second;
        else if (pair.first == "IA")
            _ia_tpoint = pair.second;
    }

    // Log
    if (error == AmelasError::SUCCESS)
    {
        oss_aux << "Old:" << '\n'
                << "  AN:   " << old_an   << " \"" << '\n'
                << "  AW:   " << old_aw   << " \"" << '\n'
                << "  CA:   " << old_ca   << " \"" << '\n'
                << "  NPAE: " << old_npae << " \"" << '\n'
                << "  IE:   " << old_ie   << " \"" << '\n'
                << "  IA:   " << old_ia   << " \"" << '\n'
                << "New:" << '\n'
                << "  AN:   " << _an_tpoint   << " \"" << '\n'
                << "  AW:   " << _aw_tpoint   << " \"" << '\n'
                << "  CA:   " << _ca_tpoint   << " \"" << '\n'
                << "  NPAE: " << _npae_tpoint << " \"" << '\n'
                << "  IE:   " << _ie_tpoint   << " \"" << '\n'
                << "  IA:   " << _ia_tpoint   << " \"" << '\n';
    }
    else
    {
        oss_aux << "";
    }

    std::ostringstream oss;
    oss << "File: " << fileData << ".txt" << '\n'
        << oss_aux.str();
    setLog(command, oss.str(), error);

    return error;
}

// TODO: AmelasError AmelasController::getMountModelCoefs(MountModelCoefs &coefs)
AmelasError AmelasController::getMountModelCoefs(double &an, double &aw, double &ca, double &npae, double &ie, double &ia)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "GET_MOUNT_MODEL_COEFS";

    // Functionality
    an = _an_tpoint;
    aw = _aw_tpoint;
    ca = _ca_tpoint;
    npae = _npae_tpoint;
    ie = _ie_tpoint;
    ia = _ia_tpoint;

    // Log
    setLog(command, "", error);

    return error;
}

AmelasError AmelasController::applyMountModelCorrections(const bool &bAN, const bool &bAW, const bool &bCA, const bool &bNPAE, const bool &bIE, const bool &bIA)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "APPLY_MOUNT_MODEL_CORRECTIONS";

    // Variable used for this function
    std::ostringstream oss;
    std::string str_partial = "";
    // -- Current position in radians
    AltAzPos actPosRadians(deg_to_radians(_plc->read<double>("MAIN.axesController._azimuthAxis._axis.NcToPlc.ActPos")), deg_to_radians(_plc->read<double>("MAIN.axesController._elevationAxis._axis.NcToPlc.ActPos")));
    // -- Offset for each coefficient
    double azOffset_IE = 0.0;
    double elOffset_IE = 0.0;
    double azOffset_IA = 0.0;
    double elOffset_IA = 0.0;
    double azOffset_CA = 0.0;
    double elOffset_CA = 0.0;
    double azOffset_AN = 0.0;
    double elOffset_AN = 0.0;
    double azOffset_AW = 0.0;
    double elOffset_AW = 0.0;
    double azOffset_NPAE = 0.0;
    double elOffset_NPAE = 0.0;

    // Functionality
    // -- Pass everything to deg
    double ie = arcsec_to_deg(_ie_tpoint);
    double ia = arcsec_to_deg(_ia_tpoint);
    double ca = arcsec_to_deg(_ca_tpoint);
    double an = arcsec_to_deg(_an_tpoint);
    double aw = arcsec_to_deg(_aw_tpoint);
    double npae = arcsec_to_deg(_npae_tpoint);

    // -- Calculate the offstes from the coefficients
    if (!_enable_mount_model)
    {
        error = AmelasError::ENABLE_WARN;
        oss << "The use of corrections is not enabled. Enable it with command 60." << '\n';
    }
    else
    {
        // --> Elevation index error
        if (bIE)
        {
            azOffset_IE = 0.0;
            elOffset_IE = ie;

            str_partial = str_partial + "  IE:   " + std::to_string(azOffset_IE) + " " + std::to_string(elOffset_IE) + '\n';
        }
        else
        {
            azOffset_IE = 0.0;
            elOffset_IE = 0.0;
        }

        // --> Azimuth index error
        if (bIA)
        {
            azOffset_IA = -ia;
            elOffset_IA = 0.0;

            str_partial = str_partial + "  IA:   " + std::to_string(azOffset_IA) + " " + std::to_string(elOffset_IA) + '\n';
        }
        else
        {
            azOffset_IA = 0.0;
            elOffset_IA = 0.0;
        }

        // --> LR collimation error
        if (bCA)
        {
            if (cos(actPosRadians.el) != 0.0)
                azOffset_CA = -ca * 1 / cos(actPosRadians.el);
            elOffset_CA = 0.0;

            str_partial = str_partial + "  CA:   " + std::to_string(azOffset_CA) + " " + std::to_string(elOffset_CA) + '\n';
        }
        else
        {
            azOffset_CA = 0.0;
            elOffset_CA = 0.0;
        }

        // --> Azimuth axis misalignment NS
        if (bAN)
        {
            azOffset_AN = -an * sin(actPosRadians.az) * tan(actPosRadians.el);
            elOffset_AN = -an * cos(actPosRadians.az);

            str_partial = str_partial + "  AN:   " + std::to_string(azOffset_AN) + " " + std::to_string(elOffset_AN) + '\n';
        }
        else
        {
            azOffset_AN = 0.0;
            elOffset_AN = 0.0;
        }

        // --> Azimuth axis misalignment EW
        if (bAW)
        {
            azOffset_AW = -aw * cos(actPosRadians.az) * tan(actPosRadians.el);
            elOffset_AW = aw * sin(actPosRadians.az);

            str_partial = str_partial + "  AW:   " + std::to_string(azOffset_AW) + " " + std::to_string(elOffset_AW) + '\n';
        }
        else
        {
            azOffset_AW = 0.0;
            elOffset_AW = 0.0;
        }

        // --> Az/El non-perpendicularity
        if (bNPAE)
        {
            azOffset_NPAE = -npae * tan(actPosRadians.el);
            elOffset_NPAE = 0.0;

            str_partial = str_partial + "  NPAE: " + std::to_string(azOffset_NPAE) + " " + std::to_string(elOffset_NPAE) + '\n';
        }
        else
        {
            azOffset_NPAE = 0.0;
            elOffset_NPAE = 0.0;
        }

        // -- Sum of all partial coefficients
        _azOffset = azOffset_AN + azOffset_AW + azOffset_CA + azOffset_IA + azOffset_IE + azOffset_NPAE;
        _elOffset = elOffset_AN + elOffset_AW + elOffset_CA + elOffset_IA + elOffset_IE + elOffset_NPAE;

        oss << "Partial coefficients applied:" << '\n'
            << str_partial
            << "Final corrections:" << '\n'
            << "  Az: " << _azOffset << " \370" << '\n'
            << "  El: " << _elOffset << " \370" << '\n';
    }

    // Log
    setLog(command, oss.str(), error);

    return error;
}

// TODO
AmelasError AmelasController::setLocation(const double &lat, const double &lon, const double &alt, const double &x, const double &y, const double &z)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "SET_LOCATION";

    // Symbol used for PLC
    const std::string symbol = "GLOBALS.Parameters.Commander.StationLocation";

    // Functionality
    _plc->write(symbol + ".wgs84.lat", lat);
    _plc->write(symbol + ".wgs84.lon", lon);
    _plc->write(symbol + ".wgs84.alt", alt);
    _plc->write(symbol + ".ecef.x", x);
    _plc->write(symbol + ".ecef.y", y);
    _plc->write(symbol + ".ecef.z", z);

    // Log
    std::ostringstream oss;
    oss << "  Station location: " << '\n'
        << "    Geodetic coordinates: " << '\n'
        << "      Lat: " << lat << '\n'
        << "      Lon: " << lon << '\n'
        << "      Alt: " << alt << '\n'
        << "    Geocentric coordinates: " << '\n'
        << "      x:   " << x << '\n'
        << "      y:   " << y << '\n'
        << "      z:   " << z << '\n';
    setLog(command, oss.str(), error);

    return error;
}

/*AmelasError AmelasController::setLocation(const StationLocation &location)
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
}*/

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
    std::ostringstream oss;
    oss << "  Station location: " << '\n'
        << "    Geodetic coordinates: " << '\n'
        << "      Lat: " << location.wgs84.lat << '\n'
        << "      Lon: " << location.wgs84.lon << '\n'
        << "      Alt: " << location.wgs84.alt << '\n'
        << "    Geocentric coordinates: " << '\n'
        << "      x:   " << location.ecef.x << '\n'
        << "      y:   " << location.ecef.y << '\n'
        << "      z:   " << location.ecef.z << '\n';
    setLog(command, oss.str(), error);
    
    return error;
}

// TODO
AmelasError AmelasController::setMeteoData(const double &press, const double &temp, const double &hr)
{
    // Auxiliar result
    AmelasError error = AmelasError::SUCCESS;

    // Command used for log
    const std::string command = "SET_METEO_DATA";

    // Symbol used for PLC
    const std::string symbol = "GLOBALS.Parameters.Commander.MeteoData";

    // Functionality
    _plc->write(symbol + ".press", press);
    _plc->write(symbol + ".temp", temp);
    _plc->write(symbol + ".hr", hr);

    // Log
    std::ostringstream oss;
    oss << "  Meteo data: "                   << '\n'
        << "    Press: " << press << " mbar"  << '\n'
        << "    Temp:  " << temp  << " \370C" << '\n'
        << "    Hr:    " << hr    << " %"     << '\n';
    setLog(command, oss.str(), error);

    return error;
}


/*AmelasError AmelasController::setMeteoData(const MeteoData &meteo)
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
}*/

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
    std::ostringstream oss;
    oss << "  Meteo data: "                   << '\n'
        << "    Press: " << meteo.press << " mbar"  << '\n'
        << "    Temp:  " << meteo.temp  << " \370C" << '\n'
        << "    Hr:    " << meteo.hr    << " %"     << '\n';
    setLog(command, oss.str(), error);
    
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

    // Functionality
    motion_mode = motionModes();

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

    // Variables used for this function
    pos.az = _plc->read<double>("MAIN.axesController._azimuthAxis._axis.NcToPlc.ActPos");
    pos.el = _plc->read<double>("MAIN.axesController._elevationAxis._axis.NcToPlc.ActPos");

    // Functionality
    motion_state = motionStates();

    // Log
    std::ostringstream oss;

    if (motion_state == AmelasMotionState::STOPPED
        || motion_state == AmelasMotionState::IDLE
        || motion_state == AmelasMotionState::PARK
        || motion_state == AmelasMotionState::CALIBRATION
        || motion_state == AmelasMotionState::MOVING)
    {
        oss << "Motion state: " << MotionStateStr[static_cast<size_t>(motion_state)]   << '\n'
            << "  Az: " << std::to_string(pos.az) << " \370" << '\n'
            << "  El: " << std::to_string(pos.el) << " \370" << '\n';
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

    // Variable used for this function
    std::ostringstream oss;

    // Functionality
    if (motionModes() == AmelasMotionMode::NO_MOTION)
    {
        //error = AmelasError::MOUNT_UNSAFE_STATE;
        error = AmelasError::START_WARN;
        oss << "There is no move loaded, so the operation cannot be carried out." << '\n';
    }
    else
    {
        // CHECK: _plc->executeCommand(symbol);
        _plc->write(symbol + ".cmd", true);
        oss << "";
    }

    // Log
    setLog(command, oss.str(), error);

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

    // Variable used for this function
    std::ostringstream oss;

    // Functionality
    if (motionStates() != AmelasMotionState::MOVING)
    {
        //error = AmelasError::MOUNT_UNSAFE_STATE;
        error = AmelasError::STOP_WARN;
        oss << "The mount is already stopped, so the operation cannot be carried out." << '\n';
    }
    else
    {
        // CHECK: _plc->executeCommand(symbol);
        _plc->write(symbol + ".cmd", true);
        oss << "";
    }

    // Log
    setLog(command, oss.str(), error);
    
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

    // Variable used for this function
    std::ostringstream oss;

    // Functionality
    if (motionStates() != AmelasMotionState::MOVING)
    {
        //error = AmelasError::MOUNT_UNSAFE_STATE;
        error = AmelasError::STOP_WARN;
        oss << "The mount is already stopped, so the operation cannot be carried out." << '\n';
    }
    else
    {
        // CHECK: _plc->executeCommand(symbol);
        _plc->write(symbol + ".cmd", true);
        oss << "";
    }

    // Log
    setLog(command, oss.str(), error);
    
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
    oss << "  Track time bias: " << time << " ms" << '\n';
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
    std::ostringstream oss;
    oss << "  Track time bias: " << time << " ms" << '\n';
    setLog(command, oss.str(), error);
    
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
    oss << "  Az (pos): " << pos.az << " \370"   << " - El (pos): " << pos.el << " \370" << '\n'
        << "  Az (vel): " << vel.az << " \370/s" << " - El (vel): " << vel.el << " \370/s" << '\n';
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
    oss << "  Az (vel): " << vel.az << " \370/s" << '\n'
        << "  El (vel): " << vel.el << " \370/s" << '\n';
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
