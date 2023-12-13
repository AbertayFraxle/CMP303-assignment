#include "ServerInterface.h"
#define ROUNDTIME 180
#define COUNTDOWN 5

void ServerInterface::bindListener()
{
	//create listener socket and initialise server
	if (listener.listen(port) != sf::Socket::Done) {
		std::cout << "ERROR! Server could not be created!";
	}
	else {
		std::cout << "Server Successfuly Initialised!\n";

		//add the listener socket to the selector to handle multiple sockets
		selector.add(listener);
		listener.setBlocking(false);

		//initialise randomness
		srand(time(NULL));

		//intitialise count of team members
		teamCount[red] = 0;
		teamCount[blue] = 0;

		//initialise team scores
		teamScore[red] = 0;
		teamScore[blue] = 0;
		
		//set round time and game start countdown to defined values
		roundTime = ROUNDTIME;
		countdown = COUNTDOWN;
		
		//initialise the container for all stored players
		for (int i = 0; i < 6; i++) {
			playerInfo[i].team = inactive;
			hitbox[i].width = 66.f;
			hitbox[i].height = 66.f;
		}
		
	}
}

void ServerInterface::update(float dt)
{


	//make your choice https://youtu.be/vhSHXGM7kgE?si=ywD4bVVnkP-3cNpo
	if (selector.wait()){

		//if the selector has chosen the client listener
		if (selector.isReady(listener)) {

			//function creates a socket and adds it to the vector of clients
			createClient();

		}

		//loop for all client
		for (int i = 0; i < clients.size();i++) {

			//function checks client and recieves packet if there is one
			handleClient(i);
		}
		
	}

	//update the hitbox positions to be accurate to the player positions
	for (int i = 0; i < clients.size(); i++) {
			hitbox[i].top = playerInfo[i].position.y - 50;
			hitbox[i].left = playerInfo[i].position.x - 50;
	}



	//for each player firing their gun, check if they hit any players and add it to their packet
	for (int i = 0; i < clients.size(); i++) {
		if (playerInfo[i].firing) {

			//cast the angle from degrees to being in radians
			float radianAngle = playerInfo[i].angle * 0.0174533;

			//determine end point of the linecast
			sf::Vector2f endPoint = playerInfo[i].position + sf::Vector2f(cosf(radianAngle)* 10000, sinf(radianAngle)* 10000);
			
			//loop for all other clients to see if they are hit
			for (int j = 0; j < clients.size(); j++) {

				//if the current client checked isn't the firing player
				if (j != i) {
					
					//check if the linecast hits the target
					if (checkLineAgainstBox(hitbox[j], playerInfo[i].position, endPoint)) {

						//if the target hit isn't of the same team as attacker and not invulnerable, set it to be hit and increase attacking player's score
						if (playerInfo[i].team != playerInfo[j].team && !playerInfo[j].invuln) {
							teamScore[playerInfo[i].team]++;
							playerInfo[j].hit = true;

							std::cout << "Red Score: " << std::to_string(teamScore[0]) << " Blue Score: " << std::to_string(teamScore[1]);
						}
					}
				}
			}
			

		}
	}


	//if more than 2 players, count down, then count up the round time
	if (clients.size() > 1) {

		if (countdown > 0) {
			countdown -= dt;
		}
		else {
			roundTime += dt;
		}

	}
	else {
		//reinitialise the server timers
		countdown = COUNTDOWN;
		roundTime = ROUNDTIME;
		teamScore[red] = 0;
		teamScore[blue] = 0;
	}


}

void ServerInterface::sendData()
{
	//send all player's information and also the team scores to the clients
	sf::Packet packet;
	
	//pack a packet with the player information, team info and timers
	packet << playerInfo[0] << playerInfo[1] << playerInfo[2] << playerInfo[3] << playerInfo[4] << playerInfo[5] << teamScore[red] << teamScore[blue] << countdown << roundTime;

	//send this packet to all connected clients
	for (std::vector<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it) {
		
		sf::TcpSocket& client = **it;
		
		client.send(packet);
		
	}

}

void ServerInterface::setPort(int prt)
{
	port = prt;

}

bool ServerInterface::checkLineAgainstBox(const sf::FloatRect& rect, const sf::Vector2f p1, const sf::Vector2f p2)
{

	//line in box collision
	auto minX = std::min(p1.x, p2.x);
	auto maxX = std::max(p1.x, p2.x);

	if (maxX > rect.left + rect.width) {
		maxX = rect.left + rect.width;
	}

	if (minX < rect.left) {
		minX = rect.left;
	}

	if (minX > maxX) { return false; };

	auto minY = p1.y;
	auto maxY = p2.y;

	auto dx = p2.x - p1.x;
	if (std::abs(dx) > 0.0001f) {
		auto k = (p2.y - p1.y) / dx;
		auto b = p1.y - k * p1.x;
		minY = k * minX + b;
		maxY = k * maxX + b;
	}

	if (minY > maxY) {
		std::swap(minY, maxY);
	}

	if (maxY > rect.top + rect.height) {
		maxY = rect.top + rect.height;
	}

	if (minY < rect.top) {
		minY = rect.top;
	}

	// If Y-projections do not intersect then there's no intersection
	if (minY > maxY) { return false; }
	return true;
}

void ServerInterface::createClient() {
	//create a new socket for the client connection
	sf::TcpSocket* client = new sf::TcpSocket;
	if (listener.accept(*client) != sf::Socket::Done) {
		//error message
		std::cout << "ERROR ACCEPTING NEW CLIENT!\n";
	}
	else {
		std::cout << "a client is attempting to join the server\n";

		//check if the server maximum players has been reached
		if (clients.size() < 6) {

			//add the new socket to the list of connected clients
			std::cout << "a client has successfully connected to the server\n";
			clients.push_back(client);
			selector.add(*client);
			sf::Packet idPacket;

			int index = clients.size() - 1;

			//determine an ID for the new client
			sf::Uint8  newID = rand() % 255 + 1;

			sf::Uint8 pTeam;

			if (teamCount[red] <= teamCount[blue]) {
				pTeam = red;
			}
			else {
				pTeam = blue;
			}
			teamCount[pTeam]++;

			playerInfo[index].team = pTeam;

			//send this new ID to the connecting client
			idPacket << newID << pTeam;
			client->send(idPacket);
			client->setBlocking(false);

		}
		else {
			//delete the attempted client
			std::cout << "maximum amount of clients connected, new client has been rejected\n";
			delete client;
		}
	}
}

void ServerInterface::handleClient(int i)
{

	sf::TcpSocket& client = *clients[i];

	//check if the selector determines if the client is ready
	if (selector.isReady(client)) {

		//recieve the packet if it's ready, and if it's valid unpack it and write to console
		sf::Packet packet;
		sf::Socket::Status status = client.receive(packet);
		if (status == sf::Socket::Done) {

			if (packet >> playerInfo[i] && (countdown <=0)) {

				std::cout << std::endl << "Player " << std::to_string(playerInfo[i].ID) << " " << std::to_string(playerInfo[i].team) << " position - x:" << playerInfo[i].position.x << " y:" << playerInfo[i].position.y;
			}

		}
		else if (status == sf::Socket::Disconnected) {

			//if the client sends a disconnect packet, remove the client from the client vector, and remover from the selector
			std::cout << std::endl << "Player " << std::to_string(playerInfo[i].ID) << " has disconnected";

			sf::Uint8 rTeam = playerInfo[i].team;
			playerInfo[i].team = inactive;

			teamCount[rTeam]--;

			clients.erase(clients.begin() + i);
			selector.remove(client);
			delete& client;
		}
	}
}

void ServerInterface::timerFunction() {

	

}