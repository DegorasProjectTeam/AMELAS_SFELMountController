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
 * @file common.cpp
 * @brief EXAMPLE FILE - This file contains the implementation of common elements for the AmelasController module.
 * @author Degoras Project Team
 * @author AVS AMELAS Team
 * @copyright EUPL License
 * @version 2309.5
***********************************************************************************************************************/

// PROJECT INCLUDES
// =====================================================================================================================
#include "AmelasController/common.h"
// =====================================================================================================================

// AMELAS NAMESPACES
// =====================================================================================================================
namespace amelas{
namespace controller{

AltAzPos::AltAzPos(double az, double el):
    az(az), el(el){}

AltAzPos::AltAzPos(): az(-1), el(-1){}

size_t AltAzPos::serialize(zmqutils::utils::BinarySerializer &serializer) const
{
    return serializer.write(az, el);
}

void AltAzPos::deserialize(zmqutils::utils::BinarySerializer &serializer)
{
    serializer.read(az, el);
}

size_t AltAzPos::serializedSize() const
{
    return (2*sizeof(uint64_t) + sizeof(double)*2);
}

AltAzPos::~AltAzPos(){}

MeteoData::MeteoData(double press, double temp, double hr):
    press(press), temp(temp), hr(hr){}

MeteoData::MeteoData(): press(-1), temp(-1), hr(-1){}

size_t MeteoData::serialize(zmqutils::utils::BinarySerializer &serializer) const
{
    return serializer.write(press, temp, hr);
}

void MeteoData::deserialize(zmqutils::utils::BinarySerializer &serializer)
{
    serializer.read(press, temp, hr);
}

size_t MeteoData::serializedSize() const
{
    return (2*sizeof(uint64_t) + sizeof(double)*3);
}

MeteoData::~MeteoData(){}

WGS84Coords::WGS84Coords(double lat, double lon, double alt):
    lat(lat), lon(lon), alt(alt){}

WGS84Coords::WGS84Coords(): lat(-1), lon(-1), alt(-1){}

size_t WGS84Coords::serialize(zmqutils::utils::BinarySerializer &serializer) const
{
    return serializer.write(lat, lon, alt);
}

void WGS84Coords::deserialize(zmqutils::utils::BinarySerializer &serializer)
{
    serializer.read(lat, lon, alt);
}

size_t WGS84Coords::serializedSize() const
{
    return (3*sizeof(uint64_t) + sizeof(double)*3);
}

WGS84Coords::~WGS84Coords(){}

ECEFCoords::ECEFCoords(double x, double y, double z):
    x(x), y(y), z(z){}

ECEFCoords::ECEFCoords(): x(-1), y(-1), z(-1){}

size_t ECEFCoords::serialize(zmqutils::utils::BinarySerializer &serializer) const
{
    return serializer.write(x, y, z);
}

void ECEFCoords::deserialize(zmqutils::utils::BinarySerializer &serializer)
{
    serializer.read(x, y, z);
}

size_t ECEFCoords::serializedSize() const
{
    return (3*sizeof(uint64_t) + sizeof(double)*3);
}

ECEFCoords::~ECEFCoords(){}

StationLocation::StationLocation(WGS84Coords wgs84, ECEFCoords ecef):
    wgs84(wgs84), ecef(ecef){}

StationLocation::StationLocation(): wgs84(-1,-1,-1), ecef(-1,-1,-1){}

size_t StationLocation::serialize(zmqutils::utils::BinarySerializer &serializer) const
{
    return serializer.write(wgs84, ecef);
}

void StationLocation::deserialize(zmqutils::utils::BinarySerializer &serializer)
{
    serializer.read(wgs84, ecef);
}

size_t StationLocation::serializedSize() const
{
    return (2*sizeof(uint64_t) + sizeof(double)*6);
}

StationLocation::~StationLocation(){}

PLCAddress::PLCAddress(std::string symbol, std::string type):
    symbol(symbol), type(type){}

PLCAddress::PLCAddress(): symbol(""), type(""){}

size_t PLCAddress::serialize(zmqutils::utils::BinarySerializer &serializer) const
{
    return serializer.write(symbol, type);
}

void PLCAddress::deserialize(zmqutils::utils::BinarySerializer &serializer)
{
    serializer.read(symbol, type);
}

size_t PLCAddress::serializedSize() const
{
    return (2*sizeof(uint64_t) + symbol.size() + type.size());
}

PLCAddress::~PLCAddress(){}

PLCRegisterValue::PLCRegisterValue(std::string symbol, std::string type, std::string value):
    symbol(symbol), type(type), value(value){}

PLCRegisterValue::PLCRegisterValue(): symbol(""), type(""), value(""){}

size_t PLCRegisterValue::serialize(zmqutils::utils::BinarySerializer &serializer) const
{
    return serializer.write(symbol, type, value);
}

void PLCRegisterValue::deserialize(zmqutils::utils::BinarySerializer &serializer)
{
    serializer.read(symbol, type, value);
}

size_t PLCRegisterValue::serializedSize() const
{
    return (2*sizeof(uint64_t) + symbol.size() + type.size() + value.size());
}

PLCRegisterValue::~PLCRegisterValue(){}

// =====================================================================================================================



}} // END NAMESPACES.
// =====================================================================================================================
