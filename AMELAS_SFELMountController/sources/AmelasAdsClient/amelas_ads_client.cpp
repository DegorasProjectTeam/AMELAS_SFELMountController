#include "AmelasAdsClient/amelas_ads_client.h"

#include <vector>
#include <sstream>

namespace amelas {

std::map<ULONG, std::function<void(const bool)>> AmelasAdsClient::_boolSubscriptionCallbacks;

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

void getAmsAddressFromString(const std::string &str, const long adsPort, AmsAddr* addr)
{
    if (str.empty())
    {
        auto errorCode = AdsGetLocalAddressEx(adsPort, addr);
    }
    else
    {
        auto addressOctetsString = splitString(str, '.');
        for (auto ii = 0; ii < 6; ii++)
        {
            addr->netId.b[ii] = (unsigned char) strtoul(addressOctetsString[ii].c_str(), NULL, 0);
        }
    }
}

AmelasAdsClient::AmelasAdsClient(const AmelasAdsClientConfig &config, const std::shared_ptr<spdlog::logger> logger) 
: _config(config), _logger(logger)
{
    _adsAddress.port = _config.port;
}

void AmelasAdsClient::connect()
{
    if (_isConnected)
    {
        _logger->warn("Tried to connect ads client when already connected");
        return;
    }

    _adsPort = AdsPortOpenEx();
    if (_adsPort == 0)
    {
        std::ostringstream oss;
        oss << "Ads failed to connect to: " << _config.amsAddress << '\n';
        _logger->error(oss.str());
    }
    else
    {
        std::ostringstream oss;
        oss << "Ads connected to: " << _config.amsAddress << '\n';
        _logger->info(oss.str());
    }

    getAmsAddressFromString(_config.amsAddress, _adsPort, &_adsAddress);

    _isConnected = true;
}

void AmelasAdsClient::disconnect()
{
    if (!_isConnected)
    {
        _logger->warn("Tried to disconnect ads client when already disconnected");
        return;
    }

    // Remove all active subscriptions
    while (!_subscriptionNotifications.empty())
    {
        unsubscribeAll(_subscriptionNotifications.begin()->first);
    }

    // Clear handles cache
    _handlesCache.clear();
    
    long errorCode = AdsPortCloseEx(_adsPort);
    if (errorCode)
    {
        std::ostringstream oss;
        oss << "Error: AdsPortCloseEx: " << errorCode << '\n';
        _logger->error(oss.str());
        // TODO: throw
    }
    std::ostringstream oss;
    oss << "Ads disconnected from: " << _config.amsAddress << '\n';
    _logger->info(oss.str());

    _isConnected = false;
}

int AmelasAdsClient::executeCommand(const std::string &symbol)
{
    write(symbol + ".cmd", true);

    if (read<bool>(symbol + ".ack"))
    {
        return 0;
    }
    else if (read<bool>(symbol + ".nack"))
    {
        return read<int>(symbol + ".code");
    }
    else
        return 0;
}

void __stdcall AmelasAdsClient::BoolCallback(AmsAddr* pAddr, AdsNotificationHeader* pNotification, ULONG hUser)
{
    bool value = *(bool *)pNotification->data;
    std::function<void(const bool)> callback = _boolSubscriptionCallbacks[hUser];
    callback(value);
}

void AmelasAdsClient::subscribe(const std::string& symbol, std::function<void(const bool)> callback, 
                                    const std::chrono::milliseconds period)
{
    if (!_isConnected)
    {
        std::ostringstream oss;
        oss << "Tried to subscribe to ads symbol: " << symbol << " while client disconnected";
        _logger->error(oss.str());
        // TODO: Throw here
    }

    AdsNotificationAttrib notificationAttrib;
    notificationAttrib.cbLength = 4;
    notificationAttrib.nTransMode = ADSTRANS_SERVERONCHA;
    notificationAttrib.nMaxDelay = 0;
    unsigned long periodIn100ns = std::chrono::duration_cast<std::chrono::microseconds>(period).count() * 10;
    notificationAttrib.nCycleTime = periodIn100ns;

    auto variableHandle = getHandleFromSymbol(symbol);
    _boolSubscriptionCallbacks[variableHandle] = callback;

    ULONG notificationHandle;
    auto errorCode = AdsSyncAddDeviceNotificationReqEx(_adsPort, &_adsAddress, ADSIGRP_SYM_VALBYHND, variableHandle, 
                                                        &notificationAttrib, AmelasAdsClient::BoolCallback, 
                                                        variableHandle, &notificationHandle);

    if (errorCode) {
        std::ostringstream oss;
        oss << "Error: AdsSyncAddDeviceNotificationReq subscribing symbol: " << symbol << " - Error code:" << errorCode << '\n';
        _logger->error(oss.str());
        // TODO: Throw here
    }

    _subscriptionNotifications[variableHandle] = notificationHandle;
}

void AmelasAdsClient::unsubscribeAll(const std::string& symbol)
{
    if (!_isConnected)
    {
        std::ostringstream oss;
        oss << "Tried to unsubscribe from ads symbol: " << symbol << " while client disconnected";
        _logger->error(oss.str());
        // TODO: Throw here
    }

    auto variableHandle = getHandleFromSymbol(symbol);
    // TODO: Catch exception from unsubscribe all and rethrow with symbol information
    unsubscribeAll(variableHandle);
}

void AmelasAdsClient::unsubscribeAll(const ULONG variableHandle)
{
    if (!_subscriptionNotifications.count(variableHandle))
    {
        // TODO: throw here
        /*std::ostringstream oss;
        oss << "Request to remove non-existent subscription from: " << symbol << "\n";
        _logger->warn(oss.str());
        return;*/
    }
    auto notificationHandle = _subscriptionNotifications[variableHandle];

    auto errorCode = AdsSyncDelDeviceNotificationReqEx(_adsPort, &_adsAddress, notificationHandle);
    if (errorCode) {
        /*std::ostringstream oss;
        oss << "AdsSyncDelDeviceNotificationReqEx unsubscribing symbol: " << symbol 
            << " - Error code:" << errorCode << '\n';
        _logger->error(oss.str());*/
        // TODO: Throw here
    }

    _subscriptionNotifications.erase(variableHandle);
}

ULONG AmelasAdsClient::getHandleFromSymbol(const std::string &symbol)
{
    if (_handlesCache.count(symbol))
    {
        return _handlesCache[symbol];
    }
    ULONG handle;
    ULONG pcbReturn;
    long errorCode = AdsSyncReadWriteReqEx2(_adsPort, &_adsAddress, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(handle),
                                                &handle, symbol.size(), (void *)symbol.c_str(), &pcbReturn);
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