//Fraser McCann 2100629

#pragma once
#include "BaseLevel.h"
#include "Input.h"
#include "Player.h"
#include "ClientInterface.h"
#include <cmath>
class Arena : public BaseLevel
{
	

public:
	Arena(sf::RenderWindow* hwnd, Input* in,ClientInterface * cli);
	~Arena();
	void handleInput(float dt) override;
	void update(float dt)override;
	void render()override;
private:

	ClientInterface* client;

	Player localPlayer;
	
	//object to render network players
	Player networkPlayers[6];

	//to get the time before sending a packet
	float elapsed;
	float timeStep;

	//timer to timestamp, and respawn timer to respawn
	float localTimer;
	float respawnTimer;

	//amount to move network players to their predicted positions
	float lerpAmount[6];

	//condition variables
	bool waiting;
	bool doubleSend;
	bool dead;
	bool gameOver;
	bool reset;

	sf::Text score;
	sf::Text timer;
	sf::Text waitMessage;
	sf::RectangleShape waitBox;

	sf::Font uiFont;
	sf::RectangleShape UI;
	sf::Texture uiTex;

	sf::Vector2f fromPos[6];
	sf::Vector2f toPos[6];


};

