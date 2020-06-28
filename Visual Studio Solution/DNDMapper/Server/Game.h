#pragma once
#include "Player.h"

class Game{
public:
	Game();
	~Game();


	Player* getDM() { return DM; }


private:
	Player* DM;

	std::vector<Player*> players;

};

