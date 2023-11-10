#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "ClientInterface.h"
#include "ServerInterface.h"
#include "Input.h"
#include <iostream>
#include <windows.h> 
#include "Arena.h"



void windowProcess(sf::RenderWindow* window, Input* in)
{
	sf::Event event;
	while (window->pollEvent(event)) {
		switch (event.type)
		{
		case sf::Event::Closed:
			window->close();
			break;
		case sf::Event::Resized:
			window->setView(sf::View(sf::FloatRect(0.f, 0.f, (float)event.size.width, (float)event.size.height)));
			break;
		case sf::Event::KeyPressed:
			in->setKeyDown(event.key.code);
			break;
		case sf::Event::KeyReleased:
			in->setKeyUp(event.key.code);
			break;
		case sf::Event::MouseMoved:
			in->setMousePosition(event.mouseMove.x, event.mouseMove.y);
			break;
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				//update input class
				in->setLeftMouse(Input::MouseState::DOWN);
			}
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				in->setRightMouse(Input::MouseState::DOWN);
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				//update input class
				in->setLeftMouse(Input::MouseState::UP);
			}
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				in->setRightMouse(Input::MouseState::UP);
			}
			break;
		default:
			// don't handle other events
			break;


		}
	}
}


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

			HWND hWnd = GetConsoleWindow();
			ShowWindow(hWnd, SW_HIDE);

			Input input;
			Arena arena(&window, &input,&client);
			

			sf::Clock clock;
			float deltaTime;
			window.setKeyRepeatEnabled(false);

			

			while (window.isOpen()) {
				windowProcess(&window, &input);

				deltaTime = clock.restart().asSeconds();

				arena.handleInput(deltaTime);
				arena.update(deltaTime);
				arena.render();

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
			//sf::Clock clock;
			//float timeStep = 1.f/64.f;
			//float elapsed = 0.f;


			while (true) {
				
				//elapsed += clock.getElapsedTime().asSeconds();
				//clock.restart();

				//if (elapsed >= timeStep) {
					server.update();
					server.sendData();
					//elapsed = 0.f;
				//}
			}
		}
		else {
			valid = false;
			std::cout << "enter a valid selection!\n";
		}
	}

	

}