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

#include <windows.h>
#include <TcAdsDef.h>
#include <TcAdsAPI.h>

#include "spdlog/spdlog.h"

namespace amelas {

struct AmelasAdsClientConfig 
{
    std::string AmsAddress;
};

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

    private:
        ULONG getHandleFromSymbol(const std::string &symbol);

        std::map<std::string,ULONG> _handlesCache;
        long _adsPort;
        AmsAddr _adsAddress;
        const std::shared_ptr<spdlog::logger> _logger;
        const AmelasAdsClientConfig _config;
};

}

#endif