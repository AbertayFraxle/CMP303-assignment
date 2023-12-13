#include "Arena.h"

Arena::Arena(sf::RenderWindow * hwnd, Input * in, ClientInterface* cli) {
	window = hwnd;
	input = in;
	client = cli;

	//hand input and window to the local player
	localPlayer.setInput(input);
	localPlayer.setWindow(hwnd);

	//for all network players, initiase them
	for (int i = 0; i < 6; i++) {
		if (i != client->getClientID()) {
			networkPlayers[i].setTeam(inactive);
			networkPlayers[i].setWindow(hwnd);
			lerpAmount[i] = 0.f;
		}
	}

	//load font for UI
	uiFont.loadFromFile("fonts/UI.ttf");

	//set the score display to defaults
	score.setFont(uiFont);
	score.setPosition(sf::Vector2f(1920/2-(score.getLocalBounds().width/2), 30));
	score.setFillColor(sf::Color::White);
	score.setCharacterSize(50);


	//set the timer display to defaults
	timer.setFont(uiFont);
	timer.setPosition(sf::Vector2f(1920 / 2 - (score.getLocalBounds().width / 2), 5));
	timer.setFillColor(sf::Color::White);
	timer.setCharacterSize(10);

	//load the border and initialise it
	uiTex.loadFromFile("textures/border.png");
	UI.setSize(sf::Vector2f(1920, 1080));
	UI.setTexture(&uiTex);
	
	//intiialse the message to wait for some players
	waitMessage.setFont(uiFont);
	waitMessage.setFillColor(sf::Color::White);
	waitMessage.setCharacterSize(60);
	waitMessage.setString("Waiting For Second Player");
	waitMessage.setPosition(sf::Vector2f((1920/2) - (waitMessage.getLocalBounds().width/2), 1080 / 2 - (waitMessage.getLocalBounds().height / 2)));

	//initialise background for wait message
	waitBox.setFillColor(sf::Color::Black);
	waitBox.setSize(sf::Vector2f(1920, 1080));

	waiting = true;

	localTimer = 0.f;

	gameOver = false;

	//define a timestep to be 1/64th of a second, the client will send an update to the server at this rate
	timeStep = 1.f / 32.f;
}

Arena::~Arena() {

}

void Arena::handleInput(float dt) {

	//execute players handle input event
	localPlayer.handleInput(dt);
	

}

void Arena::update(float dt)
{

	//increase the elapsed counter to only send updates at the designated timestep
	elapsed += dt;

	//if the player's team is not the same as what the server has assigned, change the player to the correct team
	if (localPlayer.getTeam() != client->getTeam()) {
		localPlayer.setTeam(client->getTeam());
	}

	//recieve any updates from the server;
	client->recieveData();


	if (!gameOver) {

		int count = 0;

		//if there's more than one player in game, change the display
		for (int i = 0; i < 6; i++) {
			if (client->getNTeam(i) != inactive) {
				count++;
			}
		}
		if (count > 1) {
			waiting = false;
		}
		else {
			waiting = true;
		}

		//if there is less than 2 players connected to the server, stay in a waiting state until then
		if (client->getCountdown() > 0) {
			waitBox.setFillColor(sf::Color::Black);
			if (waiting) {

				//set the waiting message to instruct a second player to connect
				waitMessage.setString("Waiting For Second Player");
				waitMessage.setPosition(sf::Vector2f((1920 / 2) - (waitMessage.getLocalBounds().width / 2), 1080 / 2 - (waitMessage.getLocalBounds().height / 2)));

			}
			else {
				//set the waiting message to a countdown to the game beginning
				waitMessage.setString("Game starting in " + std::to_string((int)(std::ceil(client->getCountdown()))));
				waitMessage.setPosition(sf::Vector2f((1920 / 2) - (waitMessage.getLocalBounds().width / 2), 1080 / 2 - (waitMessage.getLocalBounds().height / 2)));
			}
		}
		else {
			//uptick local timer
			localTimer += dt;

			//when we reach 180 seconds we're at the end of the game
			if (localTimer >= 180) {
				gameOver = true;
			}

			//if the player is dead as hell
			if (dead) {

				//set the screen message to say when they'll respawn
				waitMessage.setString("Respawn in " + std::to_string((int)(std::ceil(respawnTimer))));
				waitMessage.setPosition(sf::Vector2f((1920 / 2) - (waitMessage.getLocalBounds().width / 2), 1080 / 2 - (waitMessage.getLocalBounds().height / 2)));
				waitBox.setFillColor(sf::Color(0, 0, 0, 122));

				//tick down the respawn timer
				if (respawnTimer > 0) {
					respawnTimer -= dt;
				}
				else {
					localPlayer.spawn();
					dead = false;
				}
			}

			//get the timer and extract the minutes and seconds values from it
			int mins = (180 - (int)(localTimer)) / 60;
			int seconds = 180 - (int)(localTimer);
			bool zeroPad = false;
			while (seconds > 60) {
				seconds -= 60;
			}
			if (seconds == 60) { seconds = 0; };
			if (seconds < 10) { zeroPad = true; }

			//pad the timer with a zero if needed
			if (zeroPad) {
				timer.setString(std::to_string(mins) + ":" + "0" + std::to_string(seconds));
			}
			else {
				timer.setString(std::to_string(mins) + ":" + std::to_string(seconds));
			}
			timer.setPosition(sf::Vector2f(1920 / 2 - (timer.getLocalBounds().width / 2), 5));

			//display the current score
			score.setString(std::to_string(client->getTScore(red)) + " " + std::to_string(client->getTScore(blue)));
			score.setPosition(sf::Vector2f(1920 / 2 - (score.getLocalBounds().width / 2), 10));

			//update the local player
			localPlayer.update(dt);
		}

		//this variable sets itself to false when an update has been detected, its used to double send the last known position if player stops updating to stop predicitions
		if (localPlayer.getUpdated()) {
			doubleSend = false;
		}

		//if the time elapsed is greater than the defined timestep, send an update of this player to the server if it has updated
		if (elapsed >= timeStep) {
			if (localPlayer.getUpdated() || client->getCountdown() > 0 || doubleSend) {
				client->sendData(&localPlayer, localTimer, dead);

				//this stores the last known values sent to the server to check for when the values are updated and need to be sent again
				localPlayer.setFPos(localPlayer.getPosition());
				localPlayer.setFAngle(localPlayer.getRotation());
				localPlayer.setFFiring(localPlayer.getFiring());
				localPlayer.setFInvuln(localPlayer.getInvulnerable());

				//keep the player firing state true until now, as otherwise it might be missed
				if (localPlayer.getFiring()) {
					localPlayer.setFiring(false);
				}

				if (doubleSend) {
					doubleSend = false;
				}

				if (localPlayer.getUpdated()) {
					doubleSend = true;

				}
			}

			//reset elapsed timer
			elapsed = 0.f;
		}


		//loop for all remote clients
		for (int i = 0; i < 6; i++) {

			//check if it's remote by comparing ID
			if (client->getID(i) != client->getClientID()) {
				if (client->getNTeam(i) != inactive) {

					//currently, set the position and rotation of the network players


					//a timer in between each fixed timestep for the network players, to interpolate correctly
					if (lerpAmount[i] / timeStep < 1) {
						lerpAmount[i] += dt;
					}
					else {
						lerpAmount[i] = 0;

						//store the current target to be the origin position
						sf::Vector2f swap = toPos[i];

						//access method to check if a packet has been recieved, and set the new target to move to to the predicted position using the new gotten position
						if (client->getUpdated(i)) {

							//reset the update variable
							client->setUpdated(i, false);

							toPos[i] = client->getPredictedPosition(i);

						}
						else {

							//if not recieved a packet and if a packet is expected, continue predicting and set target appropriately
							client->setTemp(i, client->getPredictedPosition(i));
							client->doContinuedPrediction(i, timeStep);

							toPos[i] = client->getPredictedPosition(i);

						}

						//set the current target to stored origin point
						fromPos[i] = swap;
					}

					//interpolate between the origin and the target position
					float lerpPosX = std::lerp(fromPos[i].x, toPos[i].x, std::min(1.f, lerpAmount[i] / timeStep));
					float lerpPosY = std::lerp(fromPos[i].y, toPos[i].y, std::min(1.f, lerpAmount[i] / timeStep));

					//if the player isnt at the right location, move it using the interpolated values
					if (!client->getDead(i)) {
						if (networkPlayers[i].getPosition() != client->getPredictedPosition(i)) {
							networkPlayers[i].setPosition(sf::Vector2f(lerpPosX, lerpPosY));
						}
					}
					else {
						networkPlayers[i].setPosition(client->getPosition(i));
					}

					//networkPlayers[i].setPosition(client->getPredictedPosition(i));
					networkPlayers[i].setRotation(client->getRotation(i));
					networkPlayers[i].setInvulnerable(client->getInvuln(i));

					//update the network player objects
					networkPlayers[i].netUpdate(dt);

					//make sure network players team is correct
					if (networkPlayers[i].getTeam() != client->getNTeam(i)) {
						networkPlayers[i].setTeam(client->getNTeam(i));
					}

					//set the firing state of the player
					networkPlayers[i].setFiring(client->getFiring(i));
				}

			}
			else {
				//handle the local player's death
				if (client->getHit(i)) {
					dead = true;
					respawnTimer = 5.f;
					localPlayer.setPosition(0, 0);
				}
			}
		}
	}
else {

	//display which team has won the game

	std::string winTeam;

	if (client->getTScore(red) != client->getTScore(blue)) {

		if (client->getTScore(red) > client->getTScore(blue)) {
			winTeam = "Red";
		}
		else {
			winTeam = "Blue";
		}

		waitMessage.setString(winTeam + " Team won the game!");
		waitMessage.setPosition(sf::Vector2f((1920 / 2) - (waitMessage.getLocalBounds().width / 2), 1080 / 2 - (waitMessage.getLocalBounds().height / 2)));
	}
	else {
		waitMessage.setString("The game ended in a draw");
		waitMessage.setPosition(sf::Vector2f((1920 / 2) - (waitMessage.getLocalBounds().width / 2), 1080 / 2 - (waitMessage.getLocalBounds().height / 2)));
	}
	}
}

void Arena::render()
{
	beginDraw();

	sf::View gameView(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(1920, 1080)));
	//sf::View gameView(sf::FloatRect(localPlayer.getPosition() - sf::Vector2f(640, 360), sf::Vector2f(1280, 720)));
	sf::View uiView(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(1920, 1080)));
	window->setView(gameView);


	//if the game is started, render the game, else draw a waiting message to the screen
	if (client->getCountdown() <= 0) {

		//render the local player
		if (!dead) {
			localPlayer.render();
			window->draw(localPlayer);
		}

		
		//render all of the network players
		for (int i = 0; i < 6; i++) {
			if (client->getNTeam(i) != inactive) {
				if (client->getID(i) != client->getClientID()) {
					if (!client->getDead(i)) {
							networkPlayers[i].render();
							window->draw(networkPlayers[i]);
					}
				}
			}
		}

		//set the view for the UI
		window->setView(uiView);
		window->draw(UI);
		window->draw(score);
		window->draw(timer);
	}


	//draw the box and message if needed
	if (dead || gameOver || client->getCountdown() > 0) {

		window->draw(waitBox);
		window->draw(waitMessage);
	}

	endDraw();
}

