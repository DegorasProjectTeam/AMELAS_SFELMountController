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

// =====================================================================================================================



}} // END NAMESPACES.
// =====================================================================================================================
