#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "ClientInterface.h"
#include "ServerInterface.h"
#include <iostream>

int main() {

	int selection;
	bool valid = false;
	while (!valid) {

		std::cout << "type 1 for client and 2 for server: ";
		std::cin >> selection;
		valid = true;

		if (selection == 1) {

			ClientInterface client;
			sf::IpAddress adr;
			int prt;
			bool valid = false;

			while (!valid) {
				valid = true;
				std::cout << "input address to connect to: ";
				std::cin >> adr;
				client.setAddress(adr);

				std::cout << "input port to connect to: ";
				std::cin >> prt;
				client.setPort(prt);

				if (!client.connectSocket()) {
					valid = false;
				}
			}

			sf::RenderWindow window(sf::VideoMode(1920, 1080), "CMP303_Coursework");

			sf::Clock clock;
			float deltaTime;
			window.setKeyRepeatEnabled(false);

			while (window.isOpen()) {
				deltaTime = clock.restart().asSeconds();

			}



			
			
			
		}
		else if (selection == 2)
		{
			ServerInterface server;
			int prt;

			std::cout << "input port to bind to: ";
			std::cin >> prt;

			server.setPort(prt);
			server.bindListener();

			while (true) {
				server.update();
			}
		}
		else {
			valid = false;
			std::cout << "enter a valid selection!\n";
		}
	}

	

}