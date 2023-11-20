#include "ServerInterface.h"

void ServerInterface::bindListener()
{
	if (listener.listen(port) != sf::Socket::Done) {
		std::cout << "ERROR ERROR ES IST VERBOTEN!!!";
	}
	else {
		std::cout << "Server Successfuly Initialised!\n";
		selector.add(listener);
		listener.setBlocking(false);
		srand(time(NULL));
		blueCount = 0;
		redCount = 0;

		for (int i = 0; i < 6; i++) {
			playerInfo[i].team = inactive;
		}
	}
}

void ServerInterface::update()
{


	//make your choice https://youtu.be/vhSHXGM7kgE?si=ywD4bVVnkP-3cNpo
	if (selector.wait()){

		//if the selector has chosen the client listener
		if (selector.isReady(listener)) {

			//create a new socket for the client connection
			sf::TcpSocket* client = new sf::TcpSocket;
			if (listener.accept(*client) != sf::Socket::Done) {
				//error message
				std::cout << "ERROR ACCEPTING NEW CLIENT!\n";
			}
			else {
				std::cout << "a client is attempting to join the server\n";

				//check if the server maximum players has been reached
				if (clients.size() < 6) {

					//add the new socket to the list of connected clients
					std::cout << "a client has successfully connected to the server\n";
					clients.push_back(client);
					selector.add(*client);
					sf::Packet idPacket;

					int index = clients.size() - 1;

					//determine an ID for the new client
					sf::Uint8  newID = rand() % 255 + 1;

					sf::Uint8 pTeam;

					if (redCount <= blueCount) {
						pTeam = red;
						redCount++;
					}
					else {
						pTeam = blue;
						blueCount++;
					}

					playerInfo[index].team = pTeam;

					//send this new ID to the connecting client
					idPacket << newID << pTeam;
					client->send(idPacket);
					client->setBlocking(false);

				}
				else {
					//delete the attempted client
					std::cout << "maximum amount of clients connected, new client has been rejected\n";
					delete client;
				}
			}

		}
		
		for (int i = 0; i < clients.size();i++) {

			sf::TcpSocket& client = *clients[i];

			if (selector.isReady(client)) {
				sf::Packet packet;
				sf::Socket::Status status = client.receive(packet);
				if ( status== sf::Socket::Done) {

					if (packet >> playerInfo[i]) {
							
						std::cout << std::endl << "Player " << std::to_string(playerInfo[i].ID) <<" " << std::to_string(playerInfo[i].team) << " position - x:" << playerInfo[i].position.x << " y:" << playerInfo[i].position.y;
					}
				}
				else if (status == sf::Socket::Disconnected) {
					std::cout << std::endl << "Player " << std::to_string(playerInfo[i].ID)  <<" has disconnected";
					
					sf::Uint8 rTeam = playerInfo[i].team;
					playerInfo[i].team = inactive;

					if (rTeam == red) {
						redCount--;
					}
					else {
						blueCount--;
					}
					

					clients.erase(clients.begin() + i);
					selector.remove(client);
					delete &client;
				}
			}
		}
		
		
	}

}

void ServerInterface::sendData()
{

	sf::Packet packet;
	
	packet << playerInfo[0] << playerInfo[1] << playerInfo[2] << playerInfo[3] << playerInfo[4] << playerInfo[5];


	for (std::vector<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it) {
		
		sf::TcpSocket& client = **it;
		
		client.send(packet);
		
	}

}

void ServerInterface::setPort(int prt)
{
	port = prt;

}
