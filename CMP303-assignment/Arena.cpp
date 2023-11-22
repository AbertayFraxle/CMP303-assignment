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

	uiFont.loadFromFile("fonts/UI.ttf");

	score.setFont(uiFont);
	score.setPosition(sf::Vector2f(1920/2-(score.getLocalBounds().width/2), 30));
	score.setFillColor(sf::Color::White);
	score.setCharacterSize(50);

	uiTex.loadFromFile("textures/border.png");
	UI.setSize(sf::Vector2f(1920, 1080));
	UI.setTexture(&uiTex);
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
	score.setString(std::to_string(client->getTScore(red)) + " " + std::to_string(client->getTScore(blue)));
	score.setPosition(sf::Vector2f(1920 / 2 - (score.getLocalBounds().width / 2), 10));
	localPlayer.update(dt);

	if (elapsed >= timeStep) {
		if (localPlayer.getUpdated()) {
			client->sendData(&localPlayer);
			localPlayer.setFPos(localPlayer.getPosition());
			localPlayer.setFAngle(localPlayer.getRotation());
			localPlayer.setFFiring(localPlayer.getFiring());
			localPlayer.setFInvuln(localPlayer.getInvulnerable());
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
				networkPlayers[i].setInvulnerable(client->getInvuln(i));
				networkPlayers[i].netUpdate(dt);
				if (networkPlayers[i].getTeam() != client->getNTeam(i)) {
					networkPlayers[i].setTeam(client->getNTeam(i));
				}
				
				networkPlayers[i].setFiring(client->getFiring(i));
				
			
				
			}
			
		}
		else {
			if (client->getHit(i)) {
				localPlayer.spawn();
			}
		}
	}



}

void Arena::render()
{
	beginDraw();

	//sf::View gameView(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(1920, 1080)));
	sf::View gameView(sf::FloatRect(localPlayer.getPosition() - sf::Vector2f(640, 360), sf::Vector2f(1280, 720)));
	sf::View uiView(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(1920, 1080)));
	window->setView(gameView);

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

	window->setView(uiView);
	window->draw(UI);
	window->draw(score);


	endDraw();
}

