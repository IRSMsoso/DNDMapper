#pragma once
#include "Player.h"

class Game{
public:
	Game(sf::Uint16, Player*);
	~Game();


	void setDM(Player* newDM) { DM = newDM; }
	Player* getDM() { return DM; }
	bool isDM(Player*);
	bool removePlayer(Player*);
	bool addPlayer(Player*);
	void passMessage(sf::Packet, Player*);
	sf::Uint16 getID() { return id; }


private:
	Player* DM;

	sf::Uint16 id;

	std::vector<Player*> players;



};
