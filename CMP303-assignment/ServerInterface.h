#pragma once
#include "SFML/Network.hpp"
#include <iostream>
#include <vector>
class ServerInterface
{
public:
	void bindListener();
	void update();
	void setPort(int prt);

private:
	sf::TcpListener listener;
	int port;
	std::vector<sf::TcpSocket*> clients;
};

