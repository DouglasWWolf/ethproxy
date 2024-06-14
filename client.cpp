#include <unistd.h>
#include <cstdio>
#include <thread>
#include "client.h"
#include "server.h"


extern CServer Server;

static void launch_task(CClient* p) {p->task();}    


void CClient::launch(std::string remote_ip, int remote_port)
{
    m_remote_ip = remote_ip;
    m_remote_port = remote_port;
    std::thread th(launch_task, this);
    th.detach();
}



void CClient::task()
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

    // Create a new client socket
    m_psock = new NetSock;

    // Connect to the remote server
    while (!m_psock->connect(m_remote_ip, m_remote_port))
    {
        printf("Failed to connect to %s:%i.  Retrying\n", m_remote_ip.c_str(), m_remote_port);
        sleep(1);        
    }

    // We have a valid connection
    m_connected = true;

    // Tell the world that we're connected
    printf("Connected to remote server at %s\n", m_remote_ip.c_str());

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

        // Send the received data to the server side
        Server.send(buffer, bytes_rcvd);
    }

    m_connected =
    printf("Remote server dropped connection\n");
    goto again;

}





void CClient::send(void* buffer, int byte_count)
{
    if (m_psock && m_connected)
    {
        m_psock->send(buffer, byte_count);      
    }    
}
