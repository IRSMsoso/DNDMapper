// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Manager.h"
#include "Token.h"

#include <fstream>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>



int main()
{
	
	Token token(sf::Color::Blue, sf::Vector2f(2, 3));
	token.setName("Dad");

	
	{
		std::ofstream file("test.cam");

		cereal::BinaryOutputArchive archive(file);

		archive(token);
	}


	Token newToken(sf::Color::Black, sf::Vector2f(5, 5));
	newToken.setName("ff");


	{
		std::ifstream file("test.cam");

		cereal::BinaryInputArchive archive(file);

		archive(newToken);
	}

	std::cout << newToken.getName() << std::endl;

	std::cout << newToken.getPosition().x << ", " << newToken.getPosition().y << std::endl;

	


	std::system("pause");
	return 0;
}