#include "ServerInterface.h"

void ServerInterface::bindListener()
{
	if (listener.listen(port) != sf::Socket::Done) {
		std::cout << "ERROR ERROR ES IST VERBOTEN!!!";
	}
}

void ServerInterface::update()
{
	
	sf::TcpSocket client;
	if (listener.accept(client) != sf::Socket::Done) {
		

	}
	else {
		std::cout << "a client is attempting to join the server\n";
		if (clients.size() < 6) {
			std::cout << "a client has successfully connected to the server";
			clients.push_back(&client);
		}
	}

}

void ServerInterface::setPort(int prt)
{
	port = prt;

}
