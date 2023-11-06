#include "ClientInterface.h"

void ClientInterface::setAddress(sf::IpAddress adr)
{
	address = adr;
}

void ClientInterface::setPort(int prt)
{
	port = prt;
}

bool ClientInterface::connectSocket()
{
	sf::Socket::Status status = socket.connect(address, port);

	if (status != sf::Socket::Done) {
		std::cout << "failed to connect to server\n";
		return false;
	}
	else {
		std::cout << "connected to server with address " << address << " and port " << port << "\n";
		return true;
	}
}
