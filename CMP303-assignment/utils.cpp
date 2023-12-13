#include "utils.h"

sf::Packet& operator << (sf::Packet& packet, const PlayerInfo& m) {
	return packet << m.ID<< m.team <<m.position.x << m.position.y << m.angle <<m.firing << m.hit << m.invuln << m.time <<m.dead;
}

sf::Packet& operator >> (sf::Packet& packet, PlayerInfo& m) {
	return packet >> m.ID >> m.team >>m.position.x >> m.position.y >> m.angle >> m.firing >> m.hit >> m.invuln >>m.time>>m.dead;
}
