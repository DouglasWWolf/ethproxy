#pragma once
#include "netsock.h"

class CServer
{
public:
    CServer() {m_psock = nullptr; m_connected = false;}

    void    launch(int local_port);

    void    send(void* buffer, int byte_count);

    void    task();

protected:

    bool        m_connected;
    int         m_local_port;
    NetSock*    m_psock;
};
