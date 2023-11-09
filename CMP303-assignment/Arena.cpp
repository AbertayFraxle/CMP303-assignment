#include "Arena.h"

Arena::Arena(sf::RenderWindow * hwnd, Input * in, ClientInterface* cli) {
	window = hwnd;
	input = in;
	client = cli;

	localPlayer.setInput(input);
}

Arena::~Arena() {

}

void Arena::handleInput(float dt) {

	localPlayer.handleInput(dt);
	

}

void Arena::update(float dt)
{
	localPlayer.update(dt);
	client->sendData(&localPlayer);

}

void Arena::render()
{
	beginDraw();

	window->draw(localPlayer);


	endDraw();
}
