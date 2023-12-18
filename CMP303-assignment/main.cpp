//Fraser McCann 2100629
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "ClientInterface.h"
#include "ServerInterface.h"
#include "Input.h"
#include <iostream>
#include <windows.h> 
#include "Arena.h"


//Function used to handle the windows events in SFML, including user inputs and other window interactions
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
			//window->setView(sf::View(sf::FloatRect(0.f, 0.f, (float)event.size.width, (float)event.size.height)));
			//this has been commented out to stop players from being able to extend their view beyond the bounds of the arena
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

	//initialise game
	sf::Clock clock;
	int selection;
	bool valid = false;

	//input validation
	while (!valid) {

		//get selection from player to either start a server or start a client instance
		std::cout << "type 1 for client and 2 for server: ";
		std::cin >> selection;
		valid = true;


		//if 1 is chosen, create a server
		if (selection == 1) {

			ClientInterface client;
			sf::IpAddress adr;
			int prt;
			bool valid = false;

			//get the desired address and port to connect to the game
			while (!valid) {
				//loop until a valud port and socket is inputted
				valid = true;
				std::cout << "input address to connect to: ";
				std::cin >> adr;
				client.setAddress(adr);

				std::cout << "input port to connect to: ";
				std::cin >> prt;
				client.setPort(prt);

				//create a TCP socket to interface with the server
				if (!client.connectSocket()) {
					valid = false;
				}
			}


			//create game window
			sf::RenderWindow window(sf::VideoMode(1920, 1080), "CMP303_Coursework");
			window.setKeyRepeatEnabled(false);
			window.setMouseCursorVisible(false);

			//hide the console
			HWND hWnd = GetConsoleWindow();
			ShowWindow(hWnd, SW_HIDE);

			//create input object for handling input
			Input input;

			//create level for game play
			Arena arena(&window, &input,&client);
			
			//initialise variable for frame time calculations
			float deltaTime;

			//main game loop
			while (window.isOpen()) {
				windowProcess(&window, &input);

				//store the time taken to render frame
				deltaTime = clock.restart().asSeconds();

				//update the game world
				arena.handleInput(deltaTime);
				arena.update(deltaTime);
				arena.render();

			}
		}
		else if (selection == 2)
		{
			//create the object to use for the server
			ServerInterface server;

			//get user to choose what port to host server on
			int prt;
			std::cout << "input port to bind to: ";
			std::cin >> prt;

			//set the port of server object to inputted port
			server.setPort(prt);
			server.bindListener();

			//create clock and float to store the time taken to cycle the server
			sf::Clock clock;
			float deltaTime;

			//server processing loop
			while (true) {
				
				//store deltatime
				deltaTime = clock.restart().asSeconds();

				//update the server, and send data of the players to all the players
				server.update(deltaTime);
				server.sendData();

			}
		}
		else {
			//output error message to player
			valid = false;
			std::cout << "enter a valid selection!\n";
		}
	}

	

}