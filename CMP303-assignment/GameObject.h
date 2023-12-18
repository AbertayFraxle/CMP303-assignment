//Fraser McCann 2100629
#pragma once
#include "SFML\graphics.hpp"
#include "VectorHelper.h"
class GameObject : public sf::RectangleShape
{
public:
	GameObject();
	~GameObject();

	virtual void handleInput(float dt);
	virtual void update(float dt);

	//control sprite speed and direction
	void setVelocity(sf::Vector2f vel);
	sf::Vector2f getVelocity();

	bool isAlive() { return alive; };
	void setAlive(bool b) { alive = b; };

protected:
	sf::Vector2f velocity;
	bool alive;

};

