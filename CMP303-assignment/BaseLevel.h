#pragma once
#include "SFML/Graphics.hpp"
class BaseLevel
{
public:
	BaseLevel();
	~BaseLevel();

	virtual void handleInput(float dt)=0;
	virtual void update()=0;
	virtual void render() = 0;

protected:
	void beginDraw();
	void endDraw();

	sf::RenderWindow* window;

};

