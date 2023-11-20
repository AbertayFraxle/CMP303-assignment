#pragma once
#include "SFML/System.hpp"
#include "SFML/Network.hpp"

enum Team : sf::Uint8 {
	red = 0,
	blue = 1,
	inactive = 3
	};
	
	struct PlayerInfo {
		sf::Vector2f position;
		sf::Uint8 ID;
		float angle;
		sf::Uint8 team;
		bool firing;

		bool hit;
	};

	sf::Packet& operator << (sf::Packet& packet, const PlayerInfo& m);
	sf::Packet& operator >> (sf::Packet& packet, PlayerInfo& m);

