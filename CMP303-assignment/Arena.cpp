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
	client->recieveData();

	localPlayer.update(dt);

	client->sendData(&localPlayer);

	for (int i = 0; i < 5; i++) {
		networkPlayers[i].setPosition(client->getPosition(i + 1));
	}

}

void Arena::render()
{
	beginDraw();

	window->draw(localPlayer);
	
	for (int i = 0; i < 5; i++) {
		window->draw(networkPlayers[i]);
	}

	endDraw();
}
