#pragma once
#include "SFML/System.hpp"
#include "SFML/Network.hpp"

//enumerator to track what team each player is on using a transmission safe type
enum Team : sf::Uint8 {
	red = 0,
	blue = 1,
	inactive = 3
	};
	
//struct defined to transfer player information to and from the server, saves all player information on server
struct PlayerInfo {
	sf::Vector2f position;
	sf::Uint8 ID;
	float angle;
	sf::Uint8 team;
	bool firing;
	bool hit;
	bool invuln;
	bool dead;
	float time;
};

//overload the operators to be able to pack my playerinfo struct
sf::Packet& operator << (sf::Packet& packet, const PlayerInfo& m);
sf::Packet& operator >> (sf::Packet& packet, PlayerInfo& m);

