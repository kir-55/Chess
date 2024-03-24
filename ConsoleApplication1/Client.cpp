#include "Client.h"
#include <iostream>
#include <enet/enet.h>
#include <thread>



ClientData::ClientData(int id) : m_id(id) {};
void ClientData::SetUsername(std::string usernameL) { m_username = usernameL; }
int ClientData::GetId() { return m_id; }
std::string ClientData::GetUsername() { return m_username; }


void Client::SendPacket(ENetPeer* peer, const char* data) {
	ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet);
}

void Client::ParseData(char* data) {
	int data_type;
	int id;
	sscanf_s(data, "%d|%d", &data_type, &id);

	switch (data_type) {
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
		CLIENT_ID = id;
		break;
	}
}

void* Client::ReceiveLoop(ENetHost* client) {
	while (true) {
		ENetEvent event;
		while (enet_host_service(client, &event, 0) > 0) {
			if (event.type == ENET_EVENT_TYPE_RECEIVE) {
				ParseData((char*)(event.packet->data));
				enet_packet_destroy(event.packet);

			}
		}
	}

}

Client::Client(char username[], std::string adress, int port) {
	if (enet_initialize() != 0) {
		fprintf(stderr, "an error occurred (1)!");
		enet_deinitialize();

	}
	//atexit(enet_deinitialize);

	ENetHost* client;
	client = enet_host_create(NULL, 1, 1, 0, 0);

	if (client == NULL) {
		fprintf(stderr, "an error occurred (2)!");
		enet_deinitialize();

	}

	ENetAddress address;
	ENetEvent event;
	ENetPeer* peer;

	enet_address_set_host(&address, "127.0.0.1");
	address.port = 7777;

	peer = enet_host_connect(client, &address, 1, 0);

	if (peer == NULL) {
		fprintf(stderr, "an error occurred (3)!");
		enet_deinitialize();

	}

	if (enet_host_service(client, &event, 5000) > 0 &&
		event.type == ENET_EVENT_TYPE_CONNECT) {
	}
	else {
		enet_peer_reset(peer);
		puts("Connection to 127.0.0.1:7777 failed!");
		enet_deinitialize();
	}


	char str_data[80] = "2|";
	strcat_s(str_data, username);
	SendPacket(peer, str_data);


}

//int main(int argc, char** argv)
//{
//	//std::cout << "Enter your username:" << std::endl;
//	//std::cin >> username;
//
//
//	//if (enet_initialize() != 0) {
//	//	fprintf(stderr, "an error occurred (1)!");
//	//	return EXIT_FAILURE;
//	//}
//	//atexit(enet_deinitialize);
//
//	/*ENetHost* client;
//	client = enet_host_create(NULL, 1, 1, 0, 0);*/
//
//	/*if (client == NULL) {
//		fprintf(stderr, "an error occurred (2)!");
//		return EXIT_FAILURE;
//	}*/
//
//	/*ENetAddress address;
//	ENetEvent event;
//	ENetPeer* peer;
//
//	enet_address_set_host(&address, "127.0.0.1");
//	address.port = 7777;
//
//	peer = enet_host_connect(client, &address, 1, 0);
//
//	if (peer == NULL) {
//		fprintf(stderr, "an error occurred (3)!");
//		return EXIT_FAILURE;
//	}*/
//
//	/*if (enet_host_service(client, &event, 5000) > 0 &&
//		event.type == ENET_EVENT_TYPE_CONNECT) {
//	}
//	else {
//		enet_peer_reset(peer);
//		puts("Connection to 127.0.0.1:7777 failed!");
//		return EXIT_SUCCESS;
//	}
//
//
//	char str_data[80] = "2|";
//	strcat_s(str_data, username);
//	SendPacket(peer, str_data);*/
//
//	std::thread receive(ReceiveLoop, client);
//
//
//	while (true) {
//		std::string msg;
//		std::cin >> msg;
//		std::cout << username << ": " << msg.c_str() << std::endl;
//
//		char message[80] = "1|";
//		strcat_s(message, msg.c_str());
//		SendPacket(peer, message);
//	}
//	receive.join();
//	enet_peer_disconnect(peer, 0);
//	while (enet_host_service(client, &event, 3000) > 0) {
//		switch (event.type) {
//		case ENET_EVENT_TYPE_RECEIVE:
//			enet_packet_destroy(event.packet);
//			break;
//
//		case ENET_EVENT_TYPE_DISCONNECT:
//			puts("Disconnection succeeded!");
//			break;
//		}
//	}
//}

