#pragma once
#include "SFML/Network.hpp"
#include <iostream>
#include <list>
#include <vector>
#include "utils.h"
class ServerInterface
{
public:
	void bindListener();
	void update();
	void sendData();
	void setPort(int prt);

private:
	sf::TcpListener listener;
	sf::SocketSelector selector;
	int port;
	std::vector<sf::TcpSocket*> clients;

	PlayerInfo playerInfo[6];
};

