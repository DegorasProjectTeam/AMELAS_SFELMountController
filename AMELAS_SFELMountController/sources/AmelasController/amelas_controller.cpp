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
    _config(config), home_pos_(-1,-1), idle_pos_(-1,-1), park_pos_(-1,-1), calibration_pos_(-1,-1), _logger(logger)
{
    _plc = std::make_shared<amelas::AmelasAdsClient>(_config.plc_config, _logger); 
}

AmelasError AmelasController::setHomePosition(const AltAzPos &pos)
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
        this->home_pos_ = pos;
    }

    // Do things in the hardware (PLC) or FPGA.
    // this->doPLCSetHomePosition(isdhfkljsdhilfhlisd)
    // WARNING: Remember use async if the tasks are computationally demanding.
    // [...]

    // Log.
    std::string cmd_str = ControllerErrorStr[static_cast<size_t>(error)];
    std::cout << std::string(100, '-') << std::endl;
    std::cout << "<AMELAS CONTROLLER>" << std::endl;
    std::cout << "-> SET_HOME_POSITION" << std::endl;
    std::cout << "Time: " << zmqutils::utils::currentISO8601Date() << std::endl;
    std::cout << "Az: " << pos.az << std::endl;
    std::cout << "El: " << pos.el << std::endl;
    std::cout << "Error: " << static_cast<int>(error) << " (" << cmd_str << ")" << std::endl;
    std::cout << std::string(100, '-') << std::endl;

    return error;
}

AmelasError AmelasController::getHomePosition(AltAzPos &pos)
{
    pos = this->home_pos_;

    std::cout << std::string(100, '-') << std::endl;
    std::cout << "<AMELAS CONTROLLER>" << std::endl;
    std::cout << "-> GET_HOME_POSITION" << std::endl;
    std::cout << "Time: " << zmqutils::utils::currentISO8601Date() << std::endl;
    std::cout << std::string(100, '-') << std::endl;
    
    return AmelasError::SUCCESS;
}

AmelasError AmelasController::getDatetime(std::string &)
{
    return AmelasError::SUCCESS;
}

AmelasError AmelasController::setIdlePosition(const AltAzPos &pos)
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
        this->idle_pos_ = pos;
    }

    // Do things in the hardware PLC

    // Log.
    
    return error;
}

AmelasError AmelasController::getIdlePosition(AltAzPos &pos)
{
    pos = this->idle_pos_;

    // Log.
    
    return AmelasError::SUCCESS;
}

AmelasError AmelasController::setParkPosition(const AltAzPos &pos)
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
        this->park_pos_ = pos;
    }

    // Do things in the hardware PLC

    // Log.
    
    return error;
}

AmelasError AmelasController::getParkPosition(AltAzPos &pos)
{
    pos = this->park_pos_;

    // Log.
    
    return AmelasError::SUCCESS;
}

AmelasError AmelasController::setCalibrationPosition(const AltAzPos &pos)
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
        this->calibration_pos_ = pos;
    }

    // Do things in the hardware PLC

    // Log.
    
    return error;
}

AmelasError AmelasController::getCalibrationPosition(AltAzPos &pos)
{
    pos = this->calibration_pos_;

    // Log.
    
    return AmelasError::SUCCESS;
}

AmelasError AmelasController::setIdlePositionHere()
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    // this->idle_pos_.az = _plc->read<double>("az_idle");
    // this->idle_pos_.el = _plc->read<double>("el_idle");

    // Do things in the hardware PLC
    // _plc->write<double>("az_idle", this->idle_pos_.az);
    // _plc->write<double>("el_idle", this->idle_pos_.el);

    // Log.
    
    return error;
}

AmelasError AmelasController::setParkPositionHere()
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    // this->park_pos_.az = _plc->read<double>("az_park");
    // this->park_pos_.el = _plc->read<double>("el_park");

    // Do things in the hardware PLC
    // _plc->write<double>("az_park", this->park_pos_.az);
    // _plc->write<double>("el_park", this->park_pos_.el);

    // Log.
    
    return error;
}

AmelasError AmelasController::setCalibrationPositionHere()
{
    // Auxiliar result.
    AmelasError error = AmelasError::SUCCESS;

    // this->calibration_pos_.az = _plc->read<double>("az_calibration");
    // this->calibration_pos_.el = _plc->read<double>("el_calibration");

    // Do things in the hardware PLC
    // _plc->write<double>("az_calibration", this->calibration_pos_.az);
    // _plc->write<double>("el_calibration", this->calibration_pos_.el);

    // Log.
    
    return error;
}

// =====================================================================================================================

}} // END NAMESPACES.
// =====================================================================================================================
