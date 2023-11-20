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

	//send the player position and angle to the server
	sf::Vector2f playerPos = player->getPosition();
	float angle = player->getRotation();


	sf::Packet packet;
	packet << clientID << pTeam <<playerPos.x << playerPos.y << angle;

	socket.send(packet);

}

void ClientInterface::recieveData() {
	
	//recieve all the positions and angles from the server

	sf::Packet packet;

	socket.receive(packet);

	if (packet >> nPlayers[0] >> nPlayers[1] >> nPlayers[2] >> nPlayers[3]>> nPlayers[4]>> nPlayers[5]) {

	}
}

bool ClientInterface::connectSocket()
{
	//attempt to connect this client socket to the server address and port
	sf::Socket::Status status = socket.connect(address, port);
	
	
	if (status != sf::Socket::Done) {
		//if the status isn't successful, return an error message and return false
		std::cout << "failed to connect to server\n";
		return false;
	}
	else {
		//if the status is successful, display success message
		std::cout << "connected to server with address " << address << " and port " << port << "\n";

		//recieve a packet dictating the client ID from the server
		sf::Packet IDpacket;

		socket.receive(IDpacket);

		//if this client packet is recieved successfully, return true, else return false
		if (IDpacket >> clientID >> pTeam) {
			
			socket.setBlocking(false);
			return true;
		}
		return false;
		
	}
}

sf::Vector2f ClientInterface::getPosition(int index)
{
	//return the position of the network player at index
	return nPlayers[index].position;
}

float ClientInterface::getRotation(int index)
{
	//return the angle of the network player at index
	return nPlayers[index].angle;
}

sf::Uint8 ClientInterface::getNTeam(int i)
{ 
	return nPlayers[i].team; 
}

sf::Uint8 ClientInterface::getTeam() { 
	return pTeam;
};