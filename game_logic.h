
#ifndef __GAME_LOGIC_H_INCLUDED__
#define __GAME_LOGIC_H_INCLUDED__

#include <iostream>
#include <time.h>
#include <queue>
#include <array>

using std::queue;
using std::array;

class Game;

class GameLogic {
private:
	Game& game;

	unsigned int game_clock;

	bool move_toggles[4] = {false, false, false, false};

	float player_loc[2] = {0.0, 0.0}; //x, y
	float player_ori[2] = {0.0, 0.0}; //theta, phi
	queue<array<int, 4>> mouse_motion_queue; //xrel, yrel, x, y

public:
	GameLogic(Game& g) : game(g) {}

	void update_world();

	float get_player_loc(int);
	float get_player_ori(int);
	void add_mouse_motion(array<int, 4>);
};

#endif