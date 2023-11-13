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

					//determine an ID for the new client
					sf::Int32 newID = clients.size() - 1;

					//send this new ID to the connecting client
					idPacket << newID;
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
							
						std::cout << std::endl << "Player " << i << " position - x:" << playerInfo[i].position.x<< " y:" << playerInfo[i].position.y;
					}
				}
				else if (status == sf::Socket::Disconnected) {
					std::cout << std::endl << "Player " << i << " has disconnected";
					
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
