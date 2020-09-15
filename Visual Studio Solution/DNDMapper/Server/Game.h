#pragma once
#include "Player.h"

class Game{
public:
	Game();
	~Game();


	void setDM(Player* newDM) { DM = newDM; }
	Player* getDM() { return DM; }
	bool isDM(Player*);
	bool removePlayer(Player*);


private:
	Player* DM;

	std::vector<Player*> players;



};
