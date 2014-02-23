
#include "game_logic.h"

#include "main.h"

void GameLogic::update_world() {
	unsigned int time_diff = clock() - game_clock;
	game_clock += time_diff;

	float time_scale = ((float)time_diff)/CLOCKS_PER_SEC;

	IOController& ioc = game.get_controller();
	if (ioc.get_keypressed(ioc.MOVE_KEYS[0])) {
		player_loc[1] += 10*time_scale;
	} if (ioc.get_keypressed(ioc.MOVE_KEYS[1])) {
		player_loc[0] += 10*time_scale;
	} if (ioc.get_keypressed(ioc.MOVE_KEYS[2])) {
		player_loc[1] -= 10*time_scale;
	} if (ioc.get_keypressed(ioc.MOVE_KEYS[3])) {
		player_loc[0] -= 10*time_scale;
	}
}

float GameLogic::get_player_loc(int dim) {
	return player_loc[dim];
}