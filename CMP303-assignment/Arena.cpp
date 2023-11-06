#include "Arena.h"

Arena::Arena(sf::RenderWindow * hwnd) {
	window = hwnd;

}

Arena::~Arena() {

}

void Arena::handleInput(float dt) {
	if (localPlayer.isAlive()) {
		localPlayer.handleInput(dt);
	}


}

void Arena::update(float dt)
{
	localPlayer.update(dt);

}

void Arena::render()
{
	beginDraw();

	window->draw(localPlayer);


	endDraw();
}
