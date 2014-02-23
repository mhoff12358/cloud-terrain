
#ifndef __GAME_LOGIC_H_INCLUDED__
#define __GAME_LOGIC_H_INCLUDED__

#include <iostream>
#include <time.h>

class Game;

class GameLogic {
private:
	Game& game;

	unsigned int game_clock;

	bool move_toggles[4] = {false, false, false, false};

	float player_loc[2] = {0.0, 0.0};

public:
	GameLogic(Game& g) : game(g) {}

	void update_world();

	float get_player_loc(int);
};

#endif