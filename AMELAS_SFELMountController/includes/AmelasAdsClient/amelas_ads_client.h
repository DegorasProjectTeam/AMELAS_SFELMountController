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

// PROJECT INCLUDES
// =====================================================================================================================
#include "libamelas_global.h"
// =====================================================================================================================

namespace amelas {

class AmelasAdsClient
{
    public:
        LIBAMELAS_EXPORT AmelasAdsClient(std::shared_ptr<spdlog::logger> logger);

        LIBAMELAS_EXPORT void connect(const std::string &address);
        LIBAMELAS_EXPORT void disconnect();

        template <typename T> 
        LIBAMELAS_EXPORT void write(const std::string &symbol, const T data);

        template <typename T>
        LIBAMELAS_EXPORT T read(const std::string &symbol);

    private:
        ULONG getHandleFromSymbol(const std::string &symbol);

        std::map<std::string,ULONG> _handlesCache;
        long _adsPort;
        AmsAddr _adsAddress;
        const std::shared_ptr<spdlog::logger> _logger;
};

}

#endif