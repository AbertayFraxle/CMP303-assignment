#include "ClientInterface.h"

//utility methods

void ClientInterface::sendData(Player* player, float time,bool dead)
{

	//send the player position and angle to the server
	sf::Vector2f playerPos = player->getPosition();
	float angle = player->getRotation();

	bool firing = player->getFiring();

	bool invuln = player->getInvulnerable();

	sf::Packet packet;
	packet << clientID << pTeam <<playerPos.x << playerPos.y << angle << firing << false << invuln << time << dead;

	socket.send(packet);

}

void ClientInterface::recieveData() {
	
	//recieve all the positions and angles from the server
	sf::Packet packet;
	socket.receive(packet);


	PlayerInfo newInfo[6];

	if (packet >> newInfo[0] >> newInfo[1] >> newInfo[2] >> newInfo[3] >> newInfo[4] >> newInfo[5]  >> teamScore[red] >> teamScore[blue] >> countdown >> roundTime) {

		//unpack all the new information into the structure for storing player information
		for (int i = 0; i < 6;i++) {

				//shift up the previous player information and set it to have recieved a packet
				nPlayers0[i] = nPlayers[i];
				nPlayers[i] = newInfo[i];
				updated[i] = true;

		}

		//using the current and previous position, determine where the player will be on the next tick
		doPrediction();
	}
}

void ClientInterface::doPrediction() {

	for (int i = 0; i < 6; i++) {
		
		//if it's not the local player
		if (nPlayers[i].ID != clientID) {

			//if the time change makes sense (and to avoid dividing by 0)
			if ((nPlayers[i].time - nPlayers0[i].time) > 0) {
				
				//calculate the predicted position using the time and positions from the previous packet
				sf::Vector2f speed = (nPlayers[i].position - nPlayers0[i].position) / (nPlayers[i].time - nPlayers0[i].time);
				sf::Vector2f displacement = speed * (nPlayers[i].time - nPlayers0[i].time);
				predictedPlayers[i].position = nPlayers[i].position + displacement;
				predictedPlayers[i].ID = nPlayers[i].ID;
			}
		}
	}
}

void ClientInterface::doContinuedPrediction(int i,float timeStep)
{
	//if the time change makes sense (and to avoid dividing by 0)
	if ((nPlayers[i].time - nPlayers0[i].time) > 0) {

		//calculate the predicted position using the speed and displacement of the last recieved packets but applied to the currently passed in position
		sf::Vector2f speed = (nPlayers[i].position - nPlayers0[i].position) / (nPlayers[i].time - nPlayers0[i].time);
		sf::Vector2f displacement = speed * (nPlayers[i].time - nPlayers0[i].time);
		predictedPlayers[i].position = tempPlayers[i].position + displacement;
	}
}

bool ClientInterface::connectSocket()
{
	//attempt to connect this client socket to the server address and port
	sf::Socket::Status status = socket.connect(address, port);
	
	
	if (status != sf::Socket::Done) {
		//if the status isn't successful, return an error message and return false
		std::cout << "failed to connect to server\n";
		return false;
	}
	else {
		//if the status is successful, display success message
		std::cout << "connected to server with address " << address << " and port " << port << "\n";

		//recieve a packet dictating the client ID from the server
		sf::Packet IDpacket;

		socket.receive(IDpacket);

		//if this client packet is recieved successfully, return true, else return false
		if (IDpacket >> clientID >> pTeam) {
			
			socket.setBlocking(false);

			countdown = 5.f;

			return true;
		}
		return false;
		
	}
}

//getter methods

float ClientInterface::getRotation(int index)
{
	//return the angle of the network player at index
	return nPlayers[index].angle;
}

sf::Uint8 ClientInterface::getID(int index)
{
	return nPlayers[index].ID;
}

bool ClientInterface::getUpdated(int index)
{
	return updated[index];
}

bool ClientInterface::getFiring(int index)
{
	return nPlayers[index].firing;
}

bool ClientInterface::getDead(int index)
{
	return nPlayers[index].dead;
}

bool ClientInterface::getHit(int index)
{
	bool retValue = nPlayers[index].hit;

	if (retValue) {
		nPlayers[index].hit = false;
	}

	return retValue;
}

bool ClientInterface::getInvuln(int index)
{
	return nPlayers[index].invuln;
}

int ClientInterface::getClientID()
{
	return clientID;
}

sf::Uint8 ClientInterface::getNTeam(int i)
{
	return nPlayers[i].team;
}

sf::Uint8 ClientInterface::getTScore(int i)
{
	return teamScore[i];
}

float ClientInterface::getCountdown()
{
	return countdown;
}

sf::Uint8 ClientInterface::getTeam() {
	return pTeam;
}

sf::Vector2f ClientInterface::getTemp(int index)
{
	return tempPlayers[index].position;
}

sf::Vector2f ClientInterface::getPosition(int index)
{
	//return the position of the network player at index
	return nPlayers[index].position;
}

sf::Vector2f ClientInterface::getPredictedPosition(int index)
{
	return predictedPlayers[index].position;
}



//setter methods

void ClientInterface::setTemp(int index, sf::Vector2f pos)
{
	tempPlayers[index].position = pos;
}

void ClientInterface::setUpdated(int index,bool nState)
{
	updated[index] = nState;
}

void ClientInterface::setAddress(sf::IpAddress adr)
{
	address = adr;
}

void ClientInterface::setPort(int prt)
{
	port = prt;
}