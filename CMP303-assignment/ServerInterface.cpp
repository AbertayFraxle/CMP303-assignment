#include "ServerInterface.h"

void ServerInterface::bindListener()
{
	if (listener.listen(port) != sf::Socket::Done) {
		std::cout << "ERROR ERROR ES IST VERBOTEN!!!";
	}
	else {
		std::cout << "Server Successfuly Initialised!\n";
		selector.add(listener);
		listener.setBlocking(false);
		srand(time(NULL));
		teamCount[red] = 0;
		teamCount[blue] = 0;

		teamScore[red] = 0;
		teamScore[blue] = 0;

		for (int i = 0; i < 6; i++) {
			playerInfo[i].team = inactive;
			hitbox[i].width = 100.f;
			hitbox[i].height = 100.f;
		}
	}
}

void ServerInterface::update()
{


	//make your choice https://youtu.be/vhSHXGM7kgE?si=ywD4bVVnkP-3cNpo
	if (selector.wait()){

		//if the selector has chosen the client listener
		if (selector.isReady(listener)) {

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
		
		for (int i = 0; i < clients.size();i++) {

			sf::TcpSocket& client = *clients[i];

			if (selector.isReady(client)) {
				sf::Packet packet;
				sf::Socket::Status status = client.receive(packet);
				if ( status== sf::Socket::Done) {

					if (packet >> playerInfo[i]) {
							
						std::cout << std::endl << "Player " << std::to_string(playerInfo[i].ID) <<" " << std::to_string(playerInfo[i].team) << " position - x:" << playerInfo[i].position.x << " y:" << playerInfo[i].position.y;
					}
				}
				else if (status == sf::Socket::Disconnected) {
					std::cout << std::endl << "Player " << std::to_string(playerInfo[i].ID)  <<" has disconnected";
					
					sf::Uint8 rTeam = playerInfo[i].team;
					playerInfo[i].team = inactive;

					teamCount[rTeam]--;

					clients.erase(clients.begin() + i);
					selector.remove(client);
					delete &client;
				}
			}
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

			float radianAngle = playerInfo[i].angle * 0.0174533;

			sf::Vector2f endPoint = playerInfo[i].position + sf::Vector2f(cosf(radianAngle)* 10000, sinf(radianAngle)* 10000);
			
			for (int j = 0; j < clients.size(); j++) {
				if (j != i) {
					if (checkLineAgainstBox(hitbox[j], playerInfo[i].position, endPoint)) {

						if (playerInfo[i].team != playerInfo[j].team) {
							teamScore[playerInfo[i].team]++;
							playerInfo[j].hit = true;

							std::cout << "Red Score: " << std::to_string(teamScore[0]) << " Blue Score: " << std::to_string(teamScore[1]);
						}
					}
				}
			}
			

		}
	}


}

void ServerInterface::sendData()
{

	sf::Packet packet;
	
	packet << playerInfo[0] << playerInfo[1] << playerInfo[2] << playerInfo[3] << playerInfo[4] << playerInfo[5];


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
