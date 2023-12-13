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
	NetworkPlayer networkPlayers[6];

	float elapsed;
	float timeStep;

	float localTimer;
	float respawnTimer;
	float lerpAmount[6];

	bool waiting;
	bool doubleSend;
	bool dead;
	bool gameOver;

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

