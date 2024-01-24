#ifndef AMELAS_ADS_CLIENT_TPP
#define AMELAS_ADS_CLIENT_TPP

namespace amelas {

template <typename T> 
void AmelasAdsClient::write(const std::string &symbol, const T data)
{
    if (!_isConnected)
    {
        std::ostringstream oss;
        oss << "Tried to write ads symbol: " << symbol << " while client disconnected";
        _logger->error(oss.str());
        // TODO: Throw here
    }

    ULONG handle = getHandleFromSymbol(symbol);
    long errorCode = AdsSyncWriteReqEx(_adsPort, &_adsAddress, ADSIGRP_SYM_VALBYHND,
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
    if (!_isConnected)
    {
        std::ostringstream oss;
        oss << "Tried to read ads symbol: " << symbol << " while client disconnected";
        _logger->error(oss.str());
        // TODO: Throw here
    }

    T readValue;
    ULONG handle = getHandleFromSymbol(symbol);
    ULONG pcbReturn;
    long errorCode =
        AdsSyncReadReqEx2(_adsPort, &_adsAddress, ADSIGRP_SYM_VALBYHND, handle,
                            sizeof(T), (PVOID)&readValue, &pcbReturn);
    if (errorCode) {
        std::ostringstream oss;
        oss << "Error: AdsSyncReadReqEx2 reading symbol: " << symbol << " - Error code:" << errorCode << '\n';
        _logger->error(oss.str());
        // TODO: Throw here
    }
    return readValue;
}


}

#endif