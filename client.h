#pragma once
#include <string>
#include "netsock.h"

class CClient
{
public:

    CClient() {m_psock = nullptr; m_connected = false;}

    void    launch(std::string remote_ip, int remote_port);

    void    send(void* buffer, int byte_count);

    void    task();

protected:
    
    bool        m_connected;
    std::string m_remote_ip;
    int         m_remote_port;
    NetSock*    m_psock;
};
