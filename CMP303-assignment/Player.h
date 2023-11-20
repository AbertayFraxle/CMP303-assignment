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

	void netUpdate(float dt);

	void render();


	void setWindow(sf::RenderWindow* hwnd);
	void setInput(Input* in);

	sf::Uint8 getID() { return ID; };
	bool getUpdated() { return updated; };
	void setFPos(sf::Vector2f);
	void setFAngle(float nAngle);
	void setFiring(bool nFire);
	void setFFiring(bool nFFire);
	
	bool getFiring() { return firing; };

	void setTeam(sf::Uint8 nTeam);
	sf::Uint8 getTeam();



private:

	sf::Texture blueTex;
	sf::Texture redTex;

	sf::Vector2f inputVec;
	sf::Int32 ID;

	float angle;
	float angleR;

	sf::Vector2i fPos;
	float fAngle;
	bool fFiring;

	sf::Uint8 team;

	bool updated;
	bool firing;
	float drawTimer;
	float fireTimer;

	sf::RenderWindow* window;
	Input* input;

	sf::Vertex line[2];

};

