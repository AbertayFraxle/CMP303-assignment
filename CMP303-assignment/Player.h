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
	bool getUpdated() { return updated; };
	void setFPos(sf::Vector2f);
private:

	sf::Texture playerTex;
	sf::Vector2f inputVec;
	sf::Int32 ID;

	sf::Vector2i fPos;

	bool updated;

	Input* input;
};

