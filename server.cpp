#include <unistd.h>
#include <cstdio>
#include <thread>
#include "client.h"
#include "server.h"

extern CClient Client;


static void launch_task(CServer* p) {p->task();}    


void CServer::launch(int local_port)
{
    m_local_port = local_port;
    std::thread th(launch_task, this);
    th.detach();
}



void CServer::task()
{
    char buffer[0x10000];

again:

    // If we have a server socket already, throw it away
    if (m_psock)
    {
        m_connected = false;
        delete m_psock;
        m_psock = nullptr;
    }

    // Create a new server socket
    m_psock = new NetSock;
    m_psock->create_server(m_local_port);

    // Wait for someone to connect
    m_psock->listen_and_accept();

    // We have a client connected to us
    m_connected = true;
    printf("Remote client connected to server\n");

    while (true)
    {
        // Wait for data to arrive.  If the client closes the socket, break
        if (!m_psock->wait_for_data(-1)) break;
      
        // How many bytes are available to read?
        int bytes_ready = m_psock->bytes_available();

        // If the other side closed the connection, break
        if (bytes_ready < 1) break;

        // Don't ready more bytes than our buffer can hold!
        if (bytes_ready > sizeof(buffer)) bytes_ready = sizeof(buffer);
        
        // Fetch the data-bytes that are available
        int bytes_rcvd = m_psock->receive(buffer, bytes_ready);

        // If we didn't get all of our bytes, the other side closed the connection
        if (bytes_rcvd < bytes_ready) break;

        // Send the received data to the client side
        Client.send(buffer, bytes_rcvd);
    }


    m_connected = false;
    printf("Remote client disconnected from server\n");
    goto again;

}

void CServer::send(void* buffer, int byte_count)
{
    if (m_psock && m_connected)
    {
        m_psock->send(buffer, byte_count);      
    }    
}
