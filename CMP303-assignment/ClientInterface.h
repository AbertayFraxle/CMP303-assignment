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


private:
	sf::TcpSocket socket;
	
	sf::IpAddress address;

	int port;

};

