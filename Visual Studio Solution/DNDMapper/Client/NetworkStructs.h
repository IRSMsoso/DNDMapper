#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

struct TokenData {
	sf::Vector2i size;
	std::string name;
	sf::Color color;
	sf::Uint16 id;
};

struct TileData {
	sf::Color color;
	bool isFogged;
};

struct MapData {
	std::vector<std::vector<TileData>> tileGrid;
	std::vector<TokenData> tokenList;
};

enum CommandType {
	VersionConfirmation,
	TokenCreated,
	TokenDeleted,
	TokenMoved,
	TokenRenamed,
	TilePainted,
	FogPainted,
	FogRemoved,
	connectGame,
};

struct Command {

	CommandType type;
	sf::Uint16 version;
	sf::Uint16 id;
	sf::Vector2i gridLocation;
	sf::Vector2f worldLocation;
	sf::Color color;
	std::string name;
	bool OK;
};


//Packet Extending. Not sure why I have to do this for sfml stuff, but whatever.
sf::Packet& operator <<(sf::Packet& packet, const sf::Vector2i&);
sf::Packet& operator >>(sf::Packet& packet, sf::Vector2i&);
sf::Packet& operator <<(sf::Packet& packet, const sf::Color&);
sf::Packet& operator >>(sf::Packet& packet, sf::Color&);
sf::Packet& operator <<(sf::Packet& packet, const Command&);
sf::Packet& operator >>(sf::Packet& packet, Command&);
sf::Packet& operator <<(sf::Packet& packet, const sf::Vector2f&);
sf::Packet& operator >>(sf::Packet& packet, sf::Vector2f&);
