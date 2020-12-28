// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Manager.h"
#include "Token.h"

#include <fstream>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>  



int main()
{
	
	srand(time(NULL));

	Manager manager;

	manager.run();

	std::system("pause");
	return 0;
}