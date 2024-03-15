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
 * @file common.h
 * @brief This file contains the declaration of common elements for the AmelasController module.
 * @author Degoras Project Team
 * @author AVS AMELAS Team
 * @copyright EUPL License
 * @version 2310.1
***********************************************************************************************************************/

// =====================================================================================================================
#pragma once
// =====================================================================================================================

// C++ INCLUDES
// =====================================================================================================================
#include <string>
#include <map>
#include <vector>
#include <variant>
#include <functional>
// =====================================================================================================================

// ZMQUTILS INCLUDES
// =====================================================================================================================
#include <LibZMQUtils/Utils>
// =====================================================================================================================

// PROJECT INCLUDES
// =====================================================================================================================
#include "libamelas_global.h"
// =====================================================================================================================

// AMELAS NAMESPACES
// =====================================================================================================================
namespace amelas{
namespace controller{
// =====================================================================================================================

// CONSTANTS
// =====================================================================================================================
// =====================================================================================================================

// CONVENIENT ALIAS, ENUMERATIONS AND CONSTEXPR
// =====================================================================================================================

class AmelasController;

enum class AmelasError : std::int32_t
{
    INVALID_ERROR      = -1,
    SUCCESS            = 0,
    MOUNT_UNSAFE_STATE = 1,
    MOUNT_ERROR_STATE  = 2,
    INVALID_POSITION   = 3,
    UNSAFE_POSITION    = 4,
    INVALID_SPEED      = 5,
    UNSAFE_SPEED       = 6,
    NOT_IN_TRACKING    = 7,
    CPF_INVALID        = 8,
    CPF_OUTDATED       = 9,
    CPF_NO_PASSES      = 10,
    STAR_NOT_VISIBLE   = 11,
    STAR_UNSAFE        = 12,
    STAR_INVALID       = 13,
    ENABLE_WARN        = 14,
    START_WARN         = 15,
    STOP_WARN          = 16,
    FILE_ERROR         = 17

};

static constexpr std::array<const char*, 18> ControllerErrorStr
{
    "SUCCESS - Controller process success.",
    "MOUNT_UNSAFE_STATE - The mount is in an unsafe state, so the operation cannot be done.",
    "MOUNT_ERROR_STATE - The mount is in error state, so the operation cannot be done.",
    "INVALID_POSITION - The provided position (az/alt) is invalid.",
    "UNSAFE_POSITION - The provided position (az/alt) is unsafe.",
    "INVALID_SPEED - The provided speed (az/alt) is invalid.",
    "UNSAFE_SPEED - The provided speed (az/alt) is unsafe.",
    "NOT_IN_TRACKING - The mount is not in a tracking mode type and a command related to a track is called.",
    "CPF_INVALID - The provided CPF file is invalid, so it cannot be used for calculating passes.",
    "CPF_OUTDATED - The provided CPF file is outdated , so it cannot be used for calculating passes.",
    "CPF_NO_PASSES - No passes found between current  time (real or simulation) and CPF end time in the provided CPF file.",
    "STAR_NOT_VISIBLE - The provided star is not currently visible in your local sky.",
    "STAR_UNSAFE - The position of the provided star is unsafe (for example 90 deg altitude or Sun colision).",
    "STAR_INVALID - The provided star parameters are invalid (for example due to an unsoported standard equinox).",
    "It is already at the desired value.",
    "There is no move loaded, so the operation cannot be carried out.",
    "The mount is already stopped, so the operation cannot be carried out.",
    "FILE_ERROR - Can't open file. Is it possible it was moved, renamed or deleted?"
};

enum class AmelasMotionMode : std::int32_t
{
    NO_MOTION       = 0,
    ABSOLUTE_MOTION = 1,
    RELATIVE_MOTION = 2,
    CONTINUOUS      = 3,
    CPF             = 4,
    TLE             = 5,
    STAR            = 6,
    TO_IDLE         = 7,
    TO_PARK         = 8,
    TO_CALIBRATION  = 9,
    HOMING_OP       = 10,
    UNKNOWN         = 11
};

static constexpr std::array<const char*, 12> MotionModeStr
{
    "NO_MOTION - The mount does not have any motion mode loaded.",
    "ABSOLUTE - The mount is moved to an absolute coordinate at a certain speed.",
    "RELATIVE - The mount is moved a several number of degrees (step) from the current position at a certain speed.",
    "CONTINUOUS - The mount is moved continuously during an specified time or infinite (until limits or stopped) at a certain speed.",
    "CPF - The mount automatically tracks an object orbiting the earth based on its CPF (until track end or stopped).",
    "TLE - The mount automatically tracks an object orbiting the earth based on its TLE (until track end or stopped).",
    "STAR - The mount automatically tracks a star based on its parameters.",
    "TO_IDLE - The mount automatically goes to the IDLE position at a certain speed.",
    "TO_PARK - The mount automatically goes to the PARK position at a certain speed.",
    "TO_CALIBRATION - The mount automatically goes to the CALIBRATION position at a certain speed.",
    "HOMING_OP - The mount automatically performs the homing operation.",
    "UNKNOWN - Unrecognized state in application."
};

enum class AmelasMotionState : std::int32_t
{
    IDLE          = 0,
    PARK          = 1,
    CALIBRATION   = 2,
    MOVING        = 3,
    PAUSED        = 4,
    WAITING_START = 5,
    STOPPED       = 6,
    INVALID_ERROR = 7,
    DISABLED      = 8,
    RESET         = 9,
    UNKNOWN       = 10
};

static constexpr std::array<const char*, 11> MotionStateStr
{
    "IDLE - The mount is currently stopped in the IDLE position.",
    "PARK - The mount is currently stopped in the PARK position.",
    "CALIBRATION - The mount is currently stopped in the CALIBRATION position.",
    "MOVING - The mount is performing a movement operation and is currently in motion.",
    "PAUSED - The mount is performing a movement operation but is paused.",
    "WAITING_START - The mount is waiting for the start of an automatic track at the initial coordinates of the object.",
    "STOPPED - The mount is currently stopped in an unknown position.",
    "ERROR - The PLC state machine and the mount itself is currently stopped in error mode.",
    "DISABLED - The axes are disabled.",
    "RESET - The PLC state machine is being reset.",
    "UNKNOWN - Unrecognized state in application."
};

struct AltAzPos final : public zmqutils::utils::Serializable
{
    LIBAMELAS_EXPORT AltAzPos(double az, double el);

    LIBAMELAS_EXPORT AltAzPos();

    LIBAMELAS_EXPORT AltAzPos(const AltAzPos& pos) = default;

    LIBAMELAS_EXPORT AltAzPos& operator =(const AltAzPos& pos) = default;

    LIBAMELAS_EXPORT size_t serialize(zmqutils::utils::BinarySerializer& serializer) const final;

    LIBAMELAS_EXPORT void deserialize(zmqutils::utils::BinarySerializer& serializer) final;

    LIBAMELAS_EXPORT size_t serializedSize() const final;

    LIBAMELAS_EXPORT ~AltAzPos() final;

    double az;
    double el;
};

using AltAzAdj = AltAzPos;
using AltAzVel = AltAzPos;

struct MeteoData final : public zmqutils::utils::Serializable
{
    LIBAMELAS_EXPORT MeteoData(double press, double temp, double hr);

    LIBAMELAS_EXPORT MeteoData();

    LIBAMELAS_EXPORT MeteoData(const MeteoData& meteo) = default;

    LIBAMELAS_EXPORT MeteoData& operator =(const MeteoData& meteo) = default;

    LIBAMELAS_EXPORT size_t serialize(zmqutils::utils::BinarySerializer& serializer) const final;

    LIBAMELAS_EXPORT void deserialize(zmqutils::utils::BinarySerializer& serializer) final;

    LIBAMELAS_EXPORT size_t serializedSize() const final;

    LIBAMELAS_EXPORT ~MeteoData() final;

    double press;
    double temp;
    double hr;
};

struct WGS84Coords final : public zmqutils::utils::Serializable
{
    LIBAMELAS_EXPORT WGS84Coords(double lat, double lon, double alt);

    LIBAMELAS_EXPORT WGS84Coords();

    LIBAMELAS_EXPORT WGS84Coords(const WGS84Coords& WGS84) = default;

    LIBAMELAS_EXPORT WGS84Coords& operator =(const WGS84Coords& WGS84) = default;

    LIBAMELAS_EXPORT size_t serialize(zmqutils::utils::BinarySerializer& serializer) const final;

    LIBAMELAS_EXPORT void deserialize(zmqutils::utils::BinarySerializer& serializer) final;

    LIBAMELAS_EXPORT size_t serializedSize() const final;

    LIBAMELAS_EXPORT ~WGS84Coords() final;

    double lat;
    double lon;
    double alt;
};

struct ECEFCoords final : public zmqutils::utils::Serializable
{
    LIBAMELAS_EXPORT ECEFCoords(double x, double y, double z);

    LIBAMELAS_EXPORT ECEFCoords();

    LIBAMELAS_EXPORT ECEFCoords(const ECEFCoords& ECEF) = default;

    LIBAMELAS_EXPORT ECEFCoords& operator =(const ECEFCoords& ECEF) = default;

    LIBAMELAS_EXPORT size_t serialize(zmqutils::utils::BinarySerializer& serializer) const final;

    LIBAMELAS_EXPORT void deserialize(zmqutils::utils::BinarySerializer& serializer) final;

    LIBAMELAS_EXPORT size_t serializedSize() const final;

    LIBAMELAS_EXPORT ~ECEFCoords() final;

    double x;
    double y;
    double z;
};

struct StationLocation final : public zmqutils::utils::Serializable
{
    LIBAMELAS_EXPORT StationLocation(WGS84Coords wgs84, ECEFCoords ecef);

    LIBAMELAS_EXPORT StationLocation();

    LIBAMELAS_EXPORT StationLocation(const StationLocation& location) = default;

    LIBAMELAS_EXPORT StationLocation& operator =(const StationLocation& location) = default;

    LIBAMELAS_EXPORT size_t serialize(zmqutils::utils::BinarySerializer& serializer) const final;

    LIBAMELAS_EXPORT void deserialize(zmqutils::utils::BinarySerializer& serializer) final;

    LIBAMELAS_EXPORT size_t serializedSize() const final;

    LIBAMELAS_EXPORT ~StationLocation() final;

    WGS84Coords wgs84;
    ECEFCoords ecef;
};

struct PLCAddress final : public zmqutils::utils::Serializable
{
    LIBAMELAS_EXPORT PLCAddress(std::string symbol, std::string type);

    LIBAMELAS_EXPORT PLCAddress();

    LIBAMELAS_EXPORT PLCAddress(const PLCAddress& plc) = default;

    LIBAMELAS_EXPORT PLCAddress& operator =(const PLCAddress& address) = default;

    LIBAMELAS_EXPORT size_t serialize(zmqutils::utils::BinarySerializer& serializer) const final;

    LIBAMELAS_EXPORT void deserialize(zmqutils::utils::BinarySerializer& serializer) final;

    LIBAMELAS_EXPORT size_t serializedSize() const final;

    LIBAMELAS_EXPORT ~PLCAddress() final;

    std::string symbol;
    std::string type;
};

struct PLCRegisterValue final : public zmqutils::utils::Serializable
{
    LIBAMELAS_EXPORT PLCRegisterValue(std::string symbol, std::string type, std::string value);

    LIBAMELAS_EXPORT PLCRegisterValue();

    LIBAMELAS_EXPORT PLCRegisterValue(const PLCRegisterValue& plc) = default;

    LIBAMELAS_EXPORT PLCRegisterValue& operator =(const PLCRegisterValue& registerValue) = default;

    LIBAMELAS_EXPORT size_t serialize(zmqutils::utils::BinarySerializer& serializer) const final;

    LIBAMELAS_EXPORT void deserialize(zmqutils::utils::BinarySerializer& serializer) final;

    LIBAMELAS_EXPORT size_t serializedSize() const final;

    LIBAMELAS_EXPORT ~PLCRegisterValue() final;

    std::string symbol;
    std::string type;
    std::string value;
};


// Generic callback.
template<typename... Args>
using AmelasControllerCallback = controller::AmelasError(AmelasController::*)(Args...);

// Callback function type aliases
// -- SAFETY RELATED FUNCTIONS
using DoResetStateCallback   = std::function<AmelasError()>;
using EnableAvoidSunCallback = std::function<AmelasError(const bool&)>;

// -- LOW LEVEL PLC REGISTERS RELATED FUNCTIONS
using DoConnectPLCCallback    = std::function<AmelasError()>;
using DoDisconnectPLCCallback = std::function<AmelasError()>;
using GetPLCRegisterCallback  = std::function<AmelasError(const PLCAddress&, PLCRegisterValue&)>;
using GetPLCpruebaCallback    = std::function<AmelasError(const std::string&, const std::string&)>; // PRUEBA

// -- STATUS AND CONFIGURATION RELATED FUNCTIONS
using GetMountLogCallback                = std::function<AmelasError(const std::string&)>;
using DoSyncTimeNTPCallback              = std::function<AmelasError(const std::string&, const unsigned&)>; // PRUEBA
using GetMountStatusCallback             = std::function<AmelasError(std::string&)>; // PRUEBA
using GetDeviceInfoCallback              = std::function<AmelasError(std::string&)>; // PRUEBA
using EnableTrackingAdjustsCallback      = std::function<AmelasError(const bool&)>;
using EnableMountPowerCallback           = std::function<AmelasError(const bool&)>;
using SetSlewSpeedCallback               = std::function<AmelasError(const AltAzVel&)>;
using GetSlewSpeedCallback               = std::function<AmelasError(AltAzVel&)>;
using SetHomePositionCallback            = std::function<AmelasError(const AltAzPos&)>;
using GetHomePositionCallback            = std::function<AmelasError(AltAzPos&)>;
using SetIdlePositionCallback            = std::function<AmelasError(const AltAzPos&)>;
using GetIdlePositionCallback            = std::function<AmelasError(AltAzPos&)>;
using SetParkPositionCallback            = std::function<AmelasError(const AltAzPos&)>;
using GetParkPositionCallback            = std::function<AmelasError(AltAzPos&)>;
using SetCalibrationPositionCallback     = std::function<AmelasError(const AltAzPos&)>;
using GetCalibrationPositionCallback     = std::function<AmelasError(AltAzPos&)>;
using SetIdlePositionHereCallback        = std::function<AmelasError()>;
using SetParkPositionHereCallback        = std::function<AmelasError()>;
using SetCalibrationPositionHereCallback = std::function<AmelasError()>;
using SetWaitAltCallback                 = std::function<AmelasError(const double&)>;
using GetWaitAltCallback                 = std::function<AmelasError(double&)>;
using SetHomingOffsetsCallback           = std::function<AmelasError(const AltAzAdj&)>;
using GetHomingOffsetsCallback           = std::function<AmelasError(AltAzAdj&)>;
using EnableMountModelCallback           = std::function<AmelasError(const bool&)>;
using SetMountModelCoefsCallback         = std::function<AmelasError(const double&, const double&, const double&, const double&, const double&, const double&)>; // PRUEBA
using SetMountModelCoefsFileCallback     = std::function<AmelasError(const std::string&)>; // PRUEBA
using GetMountModelCoefsCallback         = std::function<AmelasError(double&, double&, double&, double&, double&, double&)>; // PRUEBA
using ApplyMountModelCorrectionsCallback = std::function<AmelasError(const bool&, const bool&, const bool&, const bool&, const bool&, const bool&)>; // PRUEBA
using SetLocationCallback                = std::function<AmelasError(const double&, const double&, const double&, const double&, const double&, const double&)>; // PRUEBA
//using SetLocationCallback                = std::function<AmelasError(const StationLocation&)>;
using GetLocationCallback                = std::function<AmelasError(StationLocation&)>;
using SetMeteoDataCallback               = std::function<AmelasError(const double&, const double&, const double&)>; // PRUEBA
//using SetMeteoDataCallback               = std::function<AmelasError(const MeteoData&)>;
using GetMeteoDataCallback               = std::function<AmelasError(MeteoData&)>;

// -- MOTION RELATED FUNCTIONS
using GetMotionModeCallback          = std::function<AmelasError(AmelasMotionMode&)>;
using GetMotionStateCallback         = std::function<AmelasError(AmelasMotionState&, AltAzPos&)>;
using SetDoStartMotionCallback       = std::function<AmelasError()>;
using SetDoPauseMotionCallback       = std::function<AmelasError()>;
using SetDoStopMotionCallback        = std::function<AmelasError()>;
using SetTrackPosOffsetCallback      = std::function<AmelasError(const AltAzAdj&)>;
using GetTrackPosOffsetCallback      = std::function<AmelasError(AltAzAdj&)>;
using SetTrackTimeBiasCallback       = std::function<AmelasError(const double&)>;
using GetTrackTimeBiasCallback       = std::function<AmelasError(double&)>;
using SetAbsoluteAltAzMotionCallback = std::function<AmelasError(const AltAzPos&, const AltAzVel&)>;
using SetRelativeAltAzMotionCallback = std::function<AmelasError(const AltAzPos&, const AltAzVel&)>;
using SetContAltAzMotionCallback     = std::function<AmelasError(const AltAzVel&)>;
using SetIdleMotionCallback          = std::function<AmelasError()>;
using SetParkMotionCallback          = std::function<AmelasError()>;
using SetCalibrationMotionCallback   = std::function<AmelasError()>;
using SetCPFMotionCallback           = std::function<AmelasError()>;

// -- OTHER FUNCTIONS
using GetDatetimeCallback = std::function<AmelasError(std::string&)>;
using DoPruebaBucles      = std::function<AmelasError()>; // PRUEBA

// =====================================================================================================================

}} // END NAMESPACES.
// =====================================================================================================================
