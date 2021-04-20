#include "NetworkStructs.h"

sf::Packet& operator <<(sf::Packet& packet, const sf::Vector2i& vector) {
	sf::Int16 x = vector.x;
	sf::Int16 y = vector.y;
	return packet << x << y;
}

sf::Packet& operator >>(sf::Packet& packet, sf::Vector2i& vector) {
	return packet >> vector.x >> vector.y;
}

sf::Packet& operator <<(sf::Packet& packet, const sf::Color& color) {
	return packet << color.toInteger();
}

sf::Packet& operator >>(sf::Packet& packet, sf::Color& color) {
	sf::Uint32 colorInt;
	packet >> colorInt;
	color = sf::Color(colorInt);
	return packet;
}

sf::Packet& operator <<(sf::Packet& packet, const Command& command) {
	return packet << command.type << command.version << command.id << command.gridLocation << command.color << command.name;
}

sf::Packet& operator >>(sf::Packet& packet, Command& command) {
	int typeint;
	packet >> typeint >> command.version >> command.id >> command.gridLocation >> command.color >> command.name;
	command.type = (CommandType)typeint;
	return packet;
}

sf::Packet& operator <<(sf::Packet& packet, const sf::Vector2f& worldxy) {
	return packet << worldxy.x << worldxy.y;
}

sf::Packet& operator >>(sf::Packet& packet, sf::Vector2f& worldxy) {
	return packet >> worldxy.x >> worldxy.y;
}