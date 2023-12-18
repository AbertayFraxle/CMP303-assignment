//Fraser McCann 2100629
#include "GameObject.h"
GameObject::GameObject()
{
	
}

GameObject::~GameObject()
{

}

// Override this function to provide input handling for the derived class
void GameObject::handleInput(float dt)
{

}

// override this function to provide functionality
void GameObject::update(float dt)
{
}

// Sets the velocity of the sprite
void GameObject::setVelocity(sf::Vector2f vel)
{
	velocity = vel;
}

sf::Vector2f GameObject::getVelocity()
{
	return velocity;
}