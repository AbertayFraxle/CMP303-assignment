#pragma once
#include "SFML/Network.hpp"
#include "SFML/Graphics.hpp"
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

	sf::Uint8 redCount;
	sf::Uint8 blueCount;

	PlayerInfo playerInfo[6];
	sf::Rect<float> hitbox[6];
};

