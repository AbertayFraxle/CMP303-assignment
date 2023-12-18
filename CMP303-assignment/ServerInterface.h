//Fraser McCann 2100629
#pragma once
#include "SFML/Network.hpp"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <list>
#include <vector>
#include <thread>
#include <mutex>
#include "utils.h"
class ServerInterface
{
public:
	void bindListener();
	void update(float dt);
	void sendData();
	void setPort(int prt);

	void incrementScore(sf::Uint8 team);

	void createClient();
	void handleClient(int i);

	bool checkLineAgainstBox(const sf::FloatRect& rect, const sf::Vector2f p1, const sf::Vector2f p2);

	void timerFunction();

private:
	sf::TcpListener listener;
	sf::SocketSelector selector;

	int port;

	std::vector<sf::TcpSocket*> clients;

	sf::Uint8 teamCount[2];

	sf::Uint8 teamScore[2];

	float roundTime;
	float countdown;

	PlayerInfo playerInfo[6];
	sf::FloatRect hitbox[6];


};

