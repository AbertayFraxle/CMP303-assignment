#include "Player.h"
#define MOVESPEED 100

Player::Player() {

	team = Team::red;

	playerTex.loadFromFile("textures/red.png");


	setTexture(&playerTex);

	setSize(sf::Vector2f(100, 100));
	setPosition(sf::Vector2f(100, 100));
	fPos = sf::Vector2i(getPosition().x, getPosition().y);

	setOrigin(sf::Vector2f(25, 50));
}

Player::~Player() {

}

void Player::handleInput(float dt)
{
	float iX = input->isKeyDown(sf::Keyboard::D) - input->isKeyDown(sf::Keyboard::A);
	float iY = input->isKeyDown(sf::Keyboard::S) - input->isKeyDown(sf::Keyboard::W);

	inputVec = VectorHelper::normalise(sf::Vector2f(iX,iY));

	sf::Vector2f diff = getPosition() - sf::Vector2f(input->getMouseX(), input->getMouseY());

	angle = atan2f(diff.y,diff.x) *(180/3.14)+180;

}

void Player::update(float dt)
{
	
	sf::Vector2f movement = sf::Vector2f(MOVESPEED*inputVec.x, MOVESPEED* inputVec.y);

	setRotation(angle);


	move(movement * dt);

	if (sf::Vector2i(getPosition().x, getPosition().y) != fPos || angle != fAngle) {
		updated = true;
	}
	else {
		updated = false;
	}
}

void Player::setInput(Input * in)
{
	input = in;
}

void Player::setFPos(sf::Vector2f pos)
{
	fPos = sf::Vector2i(pos.x, pos.y);
}

void Player::setFAngle(float nAngle)
{
	fAngle = nAngle;

}

void Player::setTeam(Team nTeam)
{
	team = nTeam;
	if (team = Team::red) {
		playerTex.loadFromFile("textures/red.png");
	}
	else {
		playerTex.loadFromFile("textures/blue.png");
	}

	setTexture(&playerTex);
}
