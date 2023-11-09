#pragma once
#include "BaseLevel.h"
#include "Input.h"
#include "Player.h"
#include "ClientInterface.h"
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
	
};

