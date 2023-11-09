#pragma once
#include "GameObject.h"
#include "SFML/System.hpp"
#include "Input.h"
class Player: public GameObject
{
public:
	Player();
	~Player();

	void handleInput(float dt);
	void update(float dt);

	void setInput(Input* in);

	sf::Int32 getID() { return ID; };
private:

	sf::Texture playerTex;
	sf::Vector2f inputVec;
	sf::Int32 ID;

	Input* input;
};

