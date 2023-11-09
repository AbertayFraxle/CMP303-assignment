#pragma once
#include "SFML/Network.hpp"
#include <iostream>
#include <list>
class ServerInterface
{
public:
	void bindListener();
	void update();
	void setPort(int prt);

private:
	sf::TcpListener listener;
	sf::SocketSelector selector;
	int port;
	std::list<sf::TcpSocket*> clients;
};

