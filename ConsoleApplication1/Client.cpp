#include "Client.h"
#include <iostream>
#include <enet/enet.h>
#include <thread>


ClientData::ClientData(int id) : m_id(id) {};
void ClientData::SetUsername(std::string usernameL) { m_username = usernameL; }
int ClientData::GetId() { return m_id; }
std::string ClientData::GetUsername() { return m_username; }



void Client::SendPacket(const char* data) {
	ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(m_peer, 0, packet);
}

void Client::ParseData(char* data) {
	int data_type;
	int id;
	sscanf_s(data, "%d|%d", &data_type, &id);

	switch (data_type) {
	case 0:
		if (id != CLIENT_ID) {
			
			sscanf_s(data, "%*[^|]|%*[^|]|%d", &lastMove);
			std::cout << "recieved move: " << lastMove << "from id: " << id << "\n";
		}
		break;
	case 1:
		if (id != CLIENT_ID) {
			char msg[80];
			sscanf_s(data, "%*d|%*d|%[^|]", &msg);
			std::cout << client_map[id]->GetUsername().c_str() << ": " << msg << std::endl;
		}
		break;
	case 2:
		if (id != CLIENT_ID) {
			char username[80];
			sscanf_s(data, "%*d|%*d|%[^|]", &username);

			std::cout << "connected user: " << username << std::endl;

			client_map[id] = new ClientData(id);
			client_map[id]->SetUsername(username);
		}
		break;
	case 3:
		sscanf_s(data, "%*[^|]|%*[^|]|%d", &m_color);
		std::cout << "recieved color: " << m_color << "\n";
		CLIENT_ID = id;
		break;
	}
}

void Client::ReceiveLoop() {
	
		ENetHost* client = GetClient();
		while (recieve) {
			try {
				ENetEvent event;
				std::cout << "c: "<< rand() << "\n";
				while (enet_host_service(client, &event, 0) > 0) {
					if (event.type == ENET_EVENT_TYPE_RECEIVE) {
						ParseData((char*)(event.packet->data));
						enet_packet_destroy(event.packet);
					}
				}
			}
			catch (std::exception& e)
			{
				std::cout << e.what() << '\n';
			}
		}
	
}

bool Client::GetColor() {
	while (true) {
		if (m_color != 0)
			return m_color == 1 ? true : false;
		else
			std::cout << "color is zero\n";
	}
		
}

ENetHost* Client::GetClient() {
	return m_client;
}

Client::Client() {

}

Client::~Client(){
	fprintf(stderr, "exitet ENet!");
	enet_deinitialize();
}

Client::Client(const char username[], const char *host, int port) {
	recieve = true;

	if (enet_initialize() != 0) {
		fprintf(stderr, "an error occurred (1)!");
		enet_deinitialize();
		return;
	}
	//atexit(enet_deinitialize);

	m_client = enet_host_create(NULL, 1, 1, 0, 0);

	if (m_client == NULL) {
		fprintf(stderr, "an error occurred (2)!");
		enet_deinitialize();
		return;
	}

	ENetAddress address;
	ENetEvent event;
	

	enet_address_set_host(&address, host);
	address.port = port;

	m_peer = enet_host_connect(m_client, &address, 1, 0);

	if (m_peer == NULL) {
		fprintf(stderr, "an error occurred (3)!");
		enet_deinitialize();
		return;
	}

	if (enet_host_service(m_client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
		ready = true;
	}
	else {
		enet_peer_reset(m_peer);
		puts("Connection to 127.0.0.1:7777 failed!");
		enet_deinitialize();
		return;
	}


	char str_data[80] = "2|";
	strcat_s(str_data, username);
	SendPacket(str_data);



}

