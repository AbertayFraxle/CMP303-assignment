#pragma once
#include "BaseLevel.h"
#include "Player.h"
class Arena : public BaseLevel
{
	Arena(sf::RenderWindow* hwnd);
	~Arena();

	void handleInput(float dt) override;
	void update(float dt);
	void render();
private:

	Player localPlayer;
};

