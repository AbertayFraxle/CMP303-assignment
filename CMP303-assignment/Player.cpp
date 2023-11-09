#include "Player.h"
#define MOVESPEED 100

Player::Player() {


	playerTex.loadFromFile("textures/Untitled.png");
	setTexture(&playerTex);
	setSize(sf::Vector2f(100, 100));
	setPosition(sf::Vector2f(100, 100));
}

Player::~Player() {

}

void Player::handleInput(float dt)
{
	float iX = input->isKeyDown(sf::Keyboard::D) - input->isKeyDown(sf::Keyboard::A);
	float iY = input->isKeyDown(sf::Keyboard::S) - input->isKeyDown(sf::Keyboard::W);

	inputVec = VectorHelper::normalise(sf::Vector2f(iX,iY));

}

void Player::update(float dt)
{

	sf::Vector2f movement = sf::Vector2f(MOVESPEED*inputVec.x, MOVESPEED* inputVec.y);


	move(movement * dt);

}

void Player::setInput(Input * in)
{
	input = in;
}
