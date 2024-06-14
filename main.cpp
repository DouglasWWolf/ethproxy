#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include "config_file.h"
#include "client.h"
#include "server.h"

using namespace std;
int32_t remote_port, local_port;
string  remote_ip;

CServer Server;
CClient Client;


//=================================================================================================
// read_config_file() - Akram, don't you dare skip commenting this code :-)
//=================================================================================================
void read_config_file(string filename)
{
   CConfigFile config;

   config.read(filename);

   config.get("remote_ip",   &remote_ip);
   config.get("remote_port", &remote_port);
   config.get("local_port",  &local_port);
}
//=================================================================================================




//=================================================================================================
// main() - execution begins here
//=================================================================================================
int main()
{
   read_config_file("ethproxy.config");

   Server.launch(local_port);
   Client.launch(remote_ip, remote_port);


   while(true) sleep(999999999);

}
//=================================================================================================