#include "AmelasAdsClient/amelas_ads_client.h"

#include <vector>
#include <sstream>

namespace amelas {

std::vector<std::string> splitString(const std::string &str, const char delim)
{
    std::vector<std::string> result; 
    auto left = str.begin(); 
    for (auto it = left; it != str.end(); ++it) 
    { 
        if (*it == delim) 
        { 
            result.emplace_back(&*left, it - left); 
            left = it + 1; 
        } 
    } 
    if (left != str.end()) 
        result.emplace_back(&*left, str.end() - left); 
    return result; 
}

AmelasAdsClient::AmelasAdsClient(const AmelasAdsClientConfig &config, const std::shared_ptr<spdlog::logger> logger) 
: _config(config), _logger(logger)
{
    auto addressAndPort = splitString(_config.AmsAddress, ':');
    _adsAddress.port = (unsigned short) strtoul(addressAndPort[1].c_str(), NULL, 0);
    auto addressOctetsString = splitString(addressAndPort[0], '.');
    for (auto ii = 0; ii < 6; ii++)
    {
        _adsAddress.netId.b[ii] = (unsigned char) strtoul(addressOctetsString[ii].c_str(), NULL, 0);
    }
}

void AmelasAdsClient::connect()
{
    _adsPort = AdsPortOpen();
}

void AmelasAdsClient::disconnect()
{
    long errorCode = AdsPortClose();
    if (errorCode)
    {
        std::ostringstream oss;
        oss << "Error: AdsPortClose: " << errorCode << '\n';
        _logger->error(oss.str());
    }
}

template <typename T> 
void AmelasAdsClient::write(const std::string &symbol, const T data)
{
    ULONG handle = getHandleFromSymbol(symbol);
    long errorCode = AdsSyncWriteReq(&_adsAddress, ADSIGRP_SYM_VALBYHND,
                                        handle, sizeof(T), (PVOID)&data);
    if (errorCode) {
        std::ostringstream oss;
        oss << "Error: AdsSyncWriteReqEx writing symbol: " << symbol << " - Error code:" << errorCode << '\n';
        _logger->error(oss.str());
        // TODO: Throw here
    }
    
}

template <typename T>
T AmelasAdsClient::read(const std::string &symbol)
{
    T readValue;
    ULONG handle = getHandleFromSymbol(symbol);
    long errorCode =
        AdsSyncReadReq(&_adsAddress, ADSIGRP_SYM_VALBYHND, handle,
                            sizeof(T), (PVOID)&readValue);
    if (errorCode) {
        std::ostringstream oss;
        oss << "Error: AdsSyncReadReq reading symbol: " << symbol << " - Error code:" << errorCode << '\n';
        _logger->error(oss.str());
        // TODO: Throw here
    }
    return readValue;
}

ULONG AmelasAdsClient::getHandleFromSymbol(const std::string &symbol)
{
    if (_handlesCache.count(symbol))
    {
        return _handlesCache[symbol];
    }
    ULONG handle;
    long errorCode = AdsSyncReadWriteReq(&_adsAddress, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(handle),
                                                &handle, symbol.size(), (void *)symbol.c_str());
    if (errorCode)
    {
        std::ostringstream oss;
        oss << "Error: AdsSyncReadWriteReq getting symbol: " << symbol << " - Error code:" << errorCode << '\n';
        _logger->error(oss.str());
        // TODO: Throw here
    }

    _handlesCache[symbol] = handle;
    return handle;
}

}