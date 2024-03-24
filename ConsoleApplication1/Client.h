#ifndef CLIENT_H
#define CLIENT_H

#include <enet/enet.h>
#include <string>;
#include <map>;

class ClientData {

private:
	int m_id;
	std::string m_username;
public:
	ClientData(int id);
	void SetUsername(std::string username);
	int GetId();
	std::string GetUsername();
};
static int CLIENT_ID;
class Client
{
	
public:
	Client(char username[], std::string adress = "127.0.0.1", int port = 7777);
	std::map<int, ClientData*> client_map;

	char username[80];
	

	void SendPacket(ENetPeer* peer, const char* data);

	void ParseData(char* data);

	void* ReceiveLoop(ENetHost* client);

private:
	int m_port, m_adres;
};

#endif

