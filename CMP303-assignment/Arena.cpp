#include "Arena.h"

Arena::Arena(sf::RenderWindow * hwnd, Input * in, ClientInterface* cli) {
	window = hwnd;
	input = in;
	client = cli;

	localPlayer.setInput(input);
	localPlayer.setWindow(hwnd);

	for (int i = 0; i < 6; i++) {
		if (i != client->getClientID()) {
			networkPlayers[i].setTeam(inactive);
			networkPlayers[i].setWindow(hwnd);
		}
	}

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

	if (localPlayer.getTeam() != client->getTeam()) {
		localPlayer.setTeam(client->getTeam());
	}

	client->recieveData();

	localPlayer.update(dt);

	if (elapsed >= timeStep) {
		if (localPlayer.getUpdated()) {
			client->sendData(&localPlayer);
			localPlayer.setFPos(localPlayer.getPosition());
			localPlayer.setFAngle(localPlayer.getRotation());
			localPlayer.setFFiring(localPlayer.getFiring());
			if (localPlayer.getFiring()) {
				localPlayer.setFiring(false);
			}
		}
		elapsed = 0.f;
	}

	for (int i = 0; i < 6; i++) {
		

		if (client->getID(i) != client->getClientID()) {
			if (client->getNTeam(i) != inactive) {
				networkPlayers[i].setPosition(client->getPosition(i));
				networkPlayers[i].setRotation(client->getRotation(i));
				networkPlayers[i].netUpdate(dt);
				if (networkPlayers[i].getTeam() != client->getNTeam(i)) {
					networkPlayers[i].setTeam(client->getNTeam(i));
				}
				
				networkPlayers[i].setFiring(client->getFiring(i));
				
			
				
			}
			
		}
		else {
			if (client->getHit(i)) {
				localPlayer.setPosition(sf::Vector2f(-100, -100));
			}
		}
	}



}

void Arena::render()
{
	beginDraw();

	localPlayer.render();
	window->draw(localPlayer);
	

	for (int i = 0; i < 6; i++) {
		if (client->getNTeam(i) != inactive) {
			if (client->getID(i) != client->getClientID()) {
				networkPlayers[i].render();
				window->draw(networkPlayers[i]);
			}
		}
	}

	endDraw();
}

