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
    INVALID_ERROR = -1,
    SUCCESS = 0,
    INVALID_POSITION = 1,
    UNSAFE_POSITION = 2,
    INVALID_SPEED = 3,
    UNSAFE_SPEED = 4
};

static constexpr std::array<const char*, 5>  ControllerErrorStr
{
    "SUCCESS - Controller process success",
    "INVALID_POSITION - The provided position (az/alt) is invalid.",
    "UNSAFE_POSITION - The provided position (az/alt) is unsafe.",
    "INVALID_SPEED - The provided speed (az/alt) is invalid.",
    "UNSAFE_SPEED - The provided speed (az/alt) is unsafe."
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

    LIBAMELAS_EXPORT MeteoData& operator =(const MeteoData& pos) = default;

    LIBAMELAS_EXPORT size_t serialize(zmqutils::utils::BinarySerializer& serializer) const final;

    LIBAMELAS_EXPORT void deserialize(zmqutils::utils::BinarySerializer& serializer) final;

    LIBAMELAS_EXPORT size_t serializedSize() const final;

    LIBAMELAS_EXPORT ~MeteoData() final;

    double press;
    double temp;
    double hr;
};

// Generic callback.
template<typename... Args>
using AmelasControllerCallback = controller::AmelasError(AmelasController::*)(Args...);

// Callback function type aliases
using GetDatetimeCallback = std::function<AmelasError(std::string&)>;

using EnableTrackingAdjustsCallback = std::function<AmelasError(const bool&)>;
using EnableMountPowerCallback = std::function<AmelasError(const bool&)>;

using SetSlewSpeedCallback = std::function<AmelasError(const AltAzVel&)>;
using GetSlewSpeedCallback = std::function<AmelasError(AltAzVel&)>;

using SetHomePositionCallback = std::function<AmelasError(const AltAzPos&)>;
using GetHomePositionCallback = std::function<AmelasError(AltAzPos&)>;

using SetIdlePositionCallback = std::function<AmelasError(const AltAzPos&)>;
using GetIdlePositionCallback = std::function<AmelasError(AltAzPos&)>;

using SetParkPositionCallback = std::function<AmelasError(const AltAzPos&)>;
using GetParkPositionCallback = std::function<AmelasError(AltAzPos&)>;

using SetCalibrationPositionCallback = std::function<AmelasError(const AltAzPos&)>;
using GetCalibrationPositionCallback = std::function<AmelasError(AltAzPos&)>;

using SetIdlePositionHereCallback = std::function<AmelasError()>;
using SetParkPositionHereCallback = std::function<AmelasError()>;
using SetCalibrationPositionHereCallback = std::function<AmelasError()>;

using SetHomingOffsetsCallback = std::function<AmelasError(const AltAzAdj&)>;
using GetHomingOffsetsCallback = std::function<AmelasError(AltAzAdj&)>;

using SetWaitAltCallback = std::function<AmelasError(const double&)>;
using GetWaitAltCallback = std::function<AmelasError(double&)>;

using EnableMountModelCallback = std::function<AmelasError(const bool&)>;

using SetMeteoData = std::function<AmelasError(const MeteoData&)>;
using GetMeteoData = std::function<AmelasError(MeteoData&)>;

using SetDoStartMotionCallback = std::function<AmelasError()>;
using SetDoPauseMotionCallback = std::function<AmelasError()>;
using SetDoStopMotionCallback = std::function<AmelasError()>;

using SetTrackPosOffsetCallback = std::function<AmelasError(const AltAzAdj&)>;
using GetTrackPosOffsetCallback = std::function<AmelasError(AltAzAdj&)>;

using SetTrackTimeBiasCallback = std::function<AmelasError(const double&)>;
using GetTrackTimeBiasCallback = std::function<AmelasError(double&)>;

using SetAbsoluteAltAzMotionCallback = std::function<AmelasError(const AltAzPos&, const AltAzVel&)>;
using SetRelativeAltAzMotionCallback = std::function<AmelasError(const AltAzPos&, const AltAzVel&)>;

using SetIdleMotionCallback = std::function<AmelasError()>;
using SetParkMotionCallback = std::function<AmelasError()>;
using SetCalibrationMotionCallback = std::function<AmelasError()>;

// =====================================================================================================================

}} // END NAMESPACES.
// =====================================================================================================================
