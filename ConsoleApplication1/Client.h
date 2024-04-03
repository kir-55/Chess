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
private:
	int m_port, m_adres;

	int m_color = 0; // ain't sure if that's correct
					// 0 - hasn't been given 
				   // 1 - white
				  // 2 - black
	
	//int m_lastMove = 0; // last move that OPONENT did

	ENetHost* m_client;
	ENetPeer* m_peer;
	
public:
	
	Client();
	Client(const char username[], const char* host = "127.0.0.1", int port = 7777);
	~Client();

	std::map<int, ClientData*> client_map;
	int lastMove = 0;
	bool recieve = true;
	char username[80];
	bool ready = false;

	void SendPacket(const char* data);

	void ParseData(char* data);

	void ReceiveLoop();

	bool GetColor();

	ENetHost* GetClient();

};

#endif

