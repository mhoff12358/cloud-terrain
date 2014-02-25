
#ifndef __WORLD_TERRAIN_H_INCLUDED__
#define __WORLD_TERRAIN_H_INCLUDED__

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <array>
#include <vector>
#include <stdlib.h>

#include "cloud.h"

class Game;

class WorldTerrain {
private:
	Game& game;
	CloudGrid cg = CloudGrid("bah", 0.01, 24.0);
	const array<float, 2> terrain_scale = {{10, 100}};

	void generate_stars(unsigned int);
	int star_size = 0.02;
	vector<array<float, 2>> stars;

public:
	WorldTerrain(Game& g);

	void initialize();

	void draw_terrain();
	void draw_skybox();
	float get_height(const float, const float);
};

#endif