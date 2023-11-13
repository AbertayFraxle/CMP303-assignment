#pragma once
#include "GameObject.h"
#include "SFML/System.hpp"
#include "Input.h"
#include "utils.h"



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
	void setFAngle(float nAngle);

	void setTeam(Team nTeam);
private:

	sf::Texture playerTex;
	sf::Vector2f inputVec;
	sf::Int32 ID;

	float angle;

	sf::Vector2i fPos;
	float fAngle;

	Team team;

	bool updated;

	Input* input;
};

