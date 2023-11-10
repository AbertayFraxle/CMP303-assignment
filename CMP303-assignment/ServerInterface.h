#pragma once
#include "SFML/Network.hpp"
#include <iostream>
#include <list>
#include <vector>
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
	std::list<sf::TcpSocket*> clients;
	sf::Vector2f playerPositions[6];
};

