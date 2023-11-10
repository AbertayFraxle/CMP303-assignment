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
	if (selector.wait()){
		if (selector.isReady(listener)) {
			sf::TcpSocket* client = new sf::TcpSocket;
			if (listener.accept(*client) != sf::Socket::Done) {


			}
			else {
				std::cout << "a client is attempting to join the server\n";
				if (clients.size() < 6) {
					std::cout << "a client has successfully connected to the server";
					clients.push_back(client);
					selector.add(*client);
					sf::Packet idPacket;

					sf::Int32 newID = clients.size() - 1;

					idPacket << newID;
					client->send(idPacket);

				}
			}

		}
		else {
			for (int i = 0; i < clients.size();i++) {

				sf::TcpSocket& client = *clients[i];

				if (selector.isReady(client)) {
					sf::Packet packet;

					if (client.receive(packet) == sf::Socket::Done) {
						sf::Vector2f playerPos;
						sf::Int32 ID;

						if (packet >> ID >>playerPos.x >> playerPos.y) {
							
							
							playerPositions[i].x = playerPos.x;
							playerPositions[i].y = playerPos.y;
							std::cout << std::endl << "Player " << ID << " position - x:" << playerPos.x<< " y:" << playerPos.y;
						}
					}
				}
			}
		}
		
		

	}

}

void ServerInterface::sendData()
{

	sf::Packet packet;
	
	packet << playerPositions[0].x << playerPositions[0].y << playerPositions[1].x << playerPositions[1].y << playerPositions[2].x << playerPositions[2].y << playerPositions[3].x << playerPositions[3].y << playerPositions[4].x << playerPositions[4].y << playerPositions[5].x << playerPositions[5].y;


	for (std::vector<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it) {
		
		sf::TcpSocket& client = **it;
		
		client.send(packet);
		
	}

}

void ServerInterface::setPort(int prt)
{
	port = prt;

}
