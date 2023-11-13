#pragma once
#include "SFML/System.hpp"
#include "SFML/Network.hpp"

	enum Team {
		red,
		blue
	};
	
	struct PlayerInfo {
		sf::Vector2f position;
		float angle;
		Team team;
	};

	sf::Packet& operator << (sf::Packet& packet, const PlayerInfo& m);
	sf::Packet& operator >> (sf::Packet& packet, PlayerInfo& m);
