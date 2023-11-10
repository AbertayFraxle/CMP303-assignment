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
	float timeStep = 1.f / 64.f;
	elapsed += dt;
	client->recieveData();

	localPlayer.update(dt);
	

	if (elapsed >= timeStep) {
		if (localPlayer.getUpdated()) {
			client->sendData(&localPlayer);
			localPlayer.setFPos(localPlayer.getPosition());
		}
		elapsed = 0.f;
	}

	for (int i = 0; i < 6; i++) {
		if (i != client->getClientID()) {
			networkPlayers[i].setPosition(client->getPosition(i));
		}
	}



}

void Arena::render()
{
	beginDraw();

	window->draw(localPlayer);

	for (int i = 0; i < 6; i++) {
		if (i != client->getClientID()) {
			window->draw(networkPlayers[i]); 
		}
	}

	endDraw();
}
