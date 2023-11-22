#pragma once
#include "SFML/Network.hpp"
#include "NetworkPlayer.h"
#include <iostream>
#include "utils.h"
class ClientInterface
{


public:
	
	void setAddress(sf::IpAddress adr);
	void setPort(int prt);

	void sendData(Player* player);
	void recieveData();

	bool connectSocket();

	sf::Vector2f getPosition(int index);
	float getRotation(int index);
	sf::Uint8 getID(int index) { return nPlayers[index].ID; };
	bool getFiring(int index) { return nPlayers[index].firing; };
	
	bool getHit(int index) { return nPlayers[index].hit; };
	bool getInvuln(int index) { return nPlayers[index].invuln; };

	int getClientID() { return clientID; };

	sf::Uint8 getTeam();
	sf::Uint8 getNTeam(int i);

	sf::Uint8 getTScore(int i) { return teamScore[i]; };

private:
	sf::TcpSocket socket;
	
	sf::IpAddress address;

	PlayerInfo nPlayers[6];

	int port;
	sf::Uint8 clientID;

	sf::Uint8 pTeam;

	sf::Uint8 teamScore[2];

};

