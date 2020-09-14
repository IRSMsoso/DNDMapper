#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>



class Token : public sf::Transformable {

public:

	Token(sf::Color, sf::Vector2f, sf::Uint16);
	~Token();


	void setSize(sf::Vector2i);
	sf::Vector2i getSize() { return size; }

	void setName(std::string newName) { name = newName; }
	std::string getName() { return name; }

	sf::Uint16 getID() { return id; }
	void setID(sf::Uint16 newID) { id = newID; }

	sf::Color getColor() { return color; }
	void setColor(sf::Color newColor) { color = newColor; }


private:
	sf::Vector2i size;
	std::string name;
	sf::Color color;
	sf::Uint16 id;

	//Serialization: differentiated due to lack of access to sf::Transformable's position.
	
	friend class cereal::access;

	template<class Archive>
	void save(Archive & archive) const {
		archive(size, name, getPosition(), color);
	}

	template<class Archive>
	void load(Archive & archive) {
		sf::Vector2f newPos;
		archive(size, name, newPos, color);
		setPosition(newPos);
	}
};

//Serialization for sf::Vector2f and sf::Color defined externally.

namespace sf {
	template<class Archive>
	void serialize(Archive & archive,
		sf::Vector2i & m)
	{
		archive(m.x, m.y);
	}


	template<class Archive>
	void serialize(Archive & archive,
		sf::Vector2f & m)
	{
		archive(m.x, m.y);
	}

	template<class Archive>
	void serialize(Archive & archive,
		sf::Color & m)
	{
		archive(m.r, m.g, m.b, m.a);
	}
}