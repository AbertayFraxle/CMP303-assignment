#include "utils.h"

sf::Packet& operator << (sf::Packet& packet, const PlayerInfo& m) {
	return packet << m.position.x << m.position.y << m.angle;
}

sf::Packet& operator >> (sf::Packet& packet, PlayerInfo& m) {
	return packet >> m.position.x >> m.position.y >> m.angle;
}
