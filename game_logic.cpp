
#include "game_logic.h"

#include "main.h"

void GameLogic::update_world() {
	unsigned int time_diff = clock() - game_clock;
	game_clock += time_diff;

	float time_scale = ((float)time_diff)/CLOCKS_PER_SEC;
	// std::cout << "FPS: " << 1.0/time_scale << std::endl;
	// game.get_terrain().position_sun(((float)game_clock)/CLOCKS_PER_SEC);
	game.get_terrain().position_sun({{0, 0, 1}});

	//Handle mouse looking
	array<int, 2> motion_rel = {{0, 0}};
	while (mouse_motion_queue.size() != 0) {
		array<int, 4>& motion_event = mouse_motion_queue.front();
		motion_rel[0] += motion_event[0];
		motion_rel[1] += motion_event[1];
		mouse_motion_queue.pop();
	}
	for (int o = 0; o < 2; o++) {
		// player_ori[o] = (player_ori[o]-(float)motion_rel[o]*game.get_controller().LOOK_SENSITIVITY[o]);
		player_ori[o] = (player_ori[o]-(float)motion_rel[o]*0.75);
	}
	while (player_ori[0] < 0) {player_ori[0] += 360;}
	while (player_ori[0] > 360) {player_ori[0] -= 360;}
	player_ori[1] = fmax(-90, fmin(90, player_ori[1]));

	//Handle keyboard movement
	IOController& ioc = game.get_controller();
	bool moved = false;
	for (int dir = 0; dir < 4; dir++) {
		if (ioc.get_keypressed(ioc.MOVE_KEYS[dir])) {
			player_loc[0] += time_scale*move_scale*cos(M_PI*(player_ori[0]-90*dir)/180);
			player_loc[1] += time_scale*move_scale*sin(M_PI*(player_ori[0]-90*dir)/180);
			moved = true;
		}
	}
	if (moved) {
		player_loc[2] = game.get_terrain().get_height(player_loc[0], player_loc[1])+0.6f;
	}
}

float GameLogic::get_player_loc(int dim) {
	return player_loc[dim];
}

float GameLogic::get_player_ori(int dim) {
	return player_ori[dim];
}

void GameLogic::add_mouse_motion(array<int, 4> new_motion) {
	//The motions that are added should be in the form of xrel, yrel, x, y
	mouse_motion_queue.emplace(new_motion);
}