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

	void incrementScore(sf::Uint8 team);

	bool checkLineAgainstBox(const sf::FloatRect& rect, const sf::Vector2f p1, const sf::Vector2f p2);

private:
	sf::TcpListener listener;
	sf::SocketSelector selector;
	int port;
	std::vector<sf::TcpSocket*> clients;

	sf::Uint8 teamCount[2];

	sf::Uint8 teamScore[2];


	PlayerInfo playerInfo[6];
	sf::FloatRect hitbox[6];
};

