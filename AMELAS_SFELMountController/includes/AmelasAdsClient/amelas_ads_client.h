//TODO: License

/** ********************************************************************************************************************
 * @file amelas_ads_client.h
 * @brief This file contains the declaration of the AmelasAdsClient class.
 * @author Degoras Project Team
 * @author AVS AMELAS Team
 * @author Tekniker AMELAS Team
 * @copyright EUPL License
 * @version 2309.5
***********************************************************************************************************************/

#ifndef AMELAS_ADS_CLIENT_H
#define AMELAS_ADS_CLIENT_H

#include <map>
#include <string>
#include <memory>
#include <chrono>

#include <windows.h>
#include <TcAdsDef.h>
#include <TcAdsAPI.h>

#include "spdlog/spdlog.h"
#include "nlohmann/json.hpp"

namespace amelas {

struct AmelasAdsClientConfig 
{
    std::string amsAddress;
    unsigned short port;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AmelasAdsClientConfig, amsAddress, port)

class AmelasAdsClient
{
    public:
        AmelasAdsClient(const AmelasAdsClientConfig &config, const std::shared_ptr<spdlog::logger> logger);

        void connect();
        void disconnect();

        template<typename T>
        void write(const std::string &symbol, const T data);

        template<typename T>
        T read(const std::string &symbol);

        /**
         * @brief Subscribe to a PLC variable and get notifications when the value changes
         * 
         * @param symbol PLC Symbol
         * @param callback Function to execute when the value changes
         * @param period Max update period (changes faster than this period will be ignored)
         * 
         * @code{.cpp}
         * std::function<void(const bool)> lambda = [](const bool value) { std::cout << value << std::endl };
         * subscribe("MAIN.pulse", lambda, std::chrono::milliseconds(100));
         * @endcode 
         */
        void subscribe(const std::string& symbol, std::function<void(const bool)> callback, 
                        const std::chrono::milliseconds period = std::chrono::milliseconds(1000));

        void unsubscribeAll(const std::string& symbol);

    private:
        ULONG getHandleFromSymbol(const std::string &symbol);
        void unsubscribeAll(const ULONG variableHandle);

        std::map<std::string,ULONG> _handlesCache;
        long _adsPort;
        AmsAddr _adsAddress;
        const std::shared_ptr<spdlog::logger> _logger;
        const AmelasAdsClientConfig _config;

        std::map<ULONG, ULONG> _subscriptionNotifications;
        static void __stdcall BoolCallback(AmsAddr* pAddr, AdsNotificationHeader* pNotification, ULONG hUser);
        static std::map<ULONG, std::function<void(const bool)>> _boolSubscriptionCallbacks;
        bool _isConnected = false;
};

}

#include "amelas_ads_client.tpp"

#endif