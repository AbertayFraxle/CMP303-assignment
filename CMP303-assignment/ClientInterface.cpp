#include "ClientInterface.h"


void ClientInterface::setAddress(sf::IpAddress adr)
{
	address = adr;
}

void ClientInterface::setPort(int prt)
{
	port = prt;
}

void ClientInterface::sendData(Player* player)
{
	sf::Vector2f playerPos = player->getPosition();

	sf::Packet packet;
	packet <<clientID << playerPos.x << playerPos.y;

	socket.send(packet);

}

void ClientInterface::recieveData() {
	
	sf::Packet packet;

	socket.receive(packet);

	if (packet >> playerPositions[0].x >> playerPositions[0].y >> playerPositions[1].x >> playerPositions[1].y >> playerPositions[2].x >> playerPositions[2].y >> playerPositions[3].x >> playerPositions[3].y >> playerPositions[4].x >> playerPositions[4].y >> playerPositions[5].x >> playerPositions[5].y) {

	}
}

bool ClientInterface::connectSocket()
{
	sf::Socket::Status status = socket.connect(address, port);
	

	if (status != sf::Socket::Done) {
		std::cout << "failed to connect to server\n";
		return false;
	}
	else {
		std::cout << "connected to server with address " << address << " and port " << port << "\n";

		sf::Packet IDpacket;
		socket.receive(IDpacket);

		if (IDpacket >> clientID) {
			socket.setBlocking(false);
			return true;
		}
		return false;
		
	}
}

sf::Vector2f ClientInterface::getPosition(int index)
{
	

	return playerPositions[index];
}

