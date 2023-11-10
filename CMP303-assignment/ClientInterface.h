#pragma once
#include "SFML/Network.hpp"
#include "NetworkPlayer.h"
#include <iostream>
class ClientInterface
{
public:

	void setAddress(sf::IpAddress adr);
	void setPort(int prt);

	void sendData(Player* player);
	void recieveData();

	bool connectSocket();

	sf::Vector2f getPosition(int index);

private:
	sf::TcpSocket socket;
	
	sf::IpAddress address;

	sf::Vector2f playerPositions[6];

	int port;

};

