#pragma once

#include "UDPContext.hpp"
#include "base.h"

#include <functional>
#include <map>
#include <mutex>
#include <set>
#include <vector>

namespace TcpProxy
{
    class TCPProxy;
}

namespace UdpProxy
{
    class UDPProxy;
}

enum class PROTOCOL
{
    TCP,
    UDP
};

class EventHandler
    : public nfapi::NF_EventHandler
    , public UDPProxyHandler
{
    struct ORIGINAL_CONN_INFO
    {
        unsigned char remoteAddress[NF_MAX_ADDRESS_LENGTH];
        std::vector<char> pendedSends;
    };

    std::map<nfapi::ENDPOINT_ID, ORIGINAL_CONN_INFO> m_connInfoMap;

  public:
    // cid, protocol, isOpening, local, remote, pid, fullpath
    typedef std::function<void(size_t, PROTOCOL, bool, std::wstring, std::wstring, int, std::wstring)> logfunc;

    EventHandler(logfunc func);
    virtual ~EventHandler();
    bool init(std::wstring address, std::string username, std::string password);
    void free();

    // from UDPProxyHandler
    virtual void onUdpReceiveComplete(unsigned __int64 id, char *buf, int len, char *remoteAddress, int remoteAddressLen) override;

    virtual void threadStart() override{};
    virtual void threadEnd() override{};

    //
    // TCP events
    //
    virtual void tcpConnectRequest(nfapi::ENDPOINT_ID id, nfapi::PNF_TCP_CONN_INFO pConnInfo) override;
    virtual void tcpConnected(nfapi::ENDPOINT_ID id, nfapi::PNF_TCP_CONN_INFO pConnInfo) override;
    virtual void tcpClosed(nfapi::ENDPOINT_ID id, nfapi::PNF_TCP_CONN_INFO pConnInfo) override;
    virtual void tcpReceive(nfapi::ENDPOINT_ID id, const char *buf, int len) override;
    virtual void tcpSend(nfapi::ENDPOINT_ID id, const char *buf, int len) override;
    virtual void tcpCanReceive(nfapi::ENDPOINT_ID id) override;
    virtual void tcpCanSend(nfapi::ENDPOINT_ID id) override;

    //
    // UDP events
    //
    virtual void udpCreated(nfapi::ENDPOINT_ID id, nfapi::PNF_UDP_CONN_INFO pConnInfo) override;
    virtual void udpConnectRequest(nfapi::ENDPOINT_ID id, nfapi::PNF_UDP_CONN_REQUEST pConnReq) override;
    virtual void udpClosed(nfapi::ENDPOINT_ID id, nfapi::PNF_UDP_CONN_INFO pConnInfo) override;
    virtual void udpReceive(nfapi::ENDPOINT_ID id, const unsigned char *remoteAddr, const char *buf, int len, nfapi::PNF_UDP_OPTIONS opt) override;
    virtual void udpSend(nfapi::ENDPOINT_ID id, const unsigned char *remoteAddr, const char *buf, int len, nfapi::PNF_UDP_OPTIONS opt) override;
    virtual void udpCanReceive(nfapi::ENDPOINT_ID id) override;
    virtual void udpCanSend(nfapi::ENDPOINT_ID id) override;

    void LogTCP(bool connected, nfapi::ENDPOINT_ID id, nfapi::PNF_TCP_CONN_INFO pConnInfo);
    void LogUDP(bool created, nfapi::ENDPOINT_ID id, nfapi::PNF_UDP_CONN_INFO pConnInfo);

  private:
    logfunc doLog;

  private:
    // TcpProxy::TCPProxy *m_tcpProxy;
    UdpProxy::UDPProxy *m_udpProxy;
    std::map<uint64_t, UDPContext *> m_udpCtxMap;

    std::mutex lock;

  private:
    unsigned char g_proxyAddress[NF_MAX_ADDRESS_LENGTH];
    std::string m_username;
    std::string m_password;
};
