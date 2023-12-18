//Fraser McCann 2100629
#pragma once
#include "SFML/Graphics.hpp"
#include "Input.h"
class BaseLevel
{
public:
	BaseLevel();
	~BaseLevel();

	virtual void handleInput(float dt)=0;
	virtual void update(float dt)=0;
	virtual void render() = 0;

protected:
	void beginDraw();
	void endDraw();

	sf::RenderWindow* window;
	Input* input;
};

