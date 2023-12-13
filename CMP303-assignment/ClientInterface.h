#pragma once
#include "SFML/Network.hpp"
#include "NetworkPlayer.h"
#include <iostream>
#include "utils.h"
class ClientInterface
{
public:

	//utility methods
	void sendData(Player* player, float time,bool dead);
	void recieveData();
	void doPrediction();
	void doContinuedPrediction(int index,float timeStep);
	bool connectSocket();

	//getters
	bool getUpdated(int index);
	bool getFiring(int index);
	bool getDead(int index);
	bool getHit(int index);
	bool getInvuln(int index);
	int getClientID();
	float getRotation(int index);
	float getCountdown();
	sf::Uint8 getID(int index);
	sf::Uint8 getTeam();
	sf::Uint8 getNTeam(int i);
	sf::Uint8 getTScore(int i);
	sf::Vector2f getTemp(int index);
	sf::Vector2f getPosition(int index);
	sf::Vector2f getPredictedPosition(int index);

	//setters
	void setUpdated(int index, bool nState);
	void setAddress(sf::IpAddress adr);
	void setPort(int prt);
	void setTemp(int index, sf::Vector2f pos);

private:
	bool updated[6];

	int port;

	float countdown, roundTime;
	float roundTime0, roundTime1;

	sf::TcpSocket socket;
	sf::IpAddress address;

	PlayerInfo nPlayers[6];
	PlayerInfo nPlayers0[6];
	PlayerInfo tempPlayers[6];
	PlayerInfo predictedPlayers[6];

	sf::Uint8 clientID;
	sf::Uint8 pTeam;
	sf::Uint8 teamScore[2];
};

