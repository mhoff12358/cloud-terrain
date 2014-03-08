
#ifndef __WORLD_TERRAIN_H_INCLUDED__
#define __WORLD_TERRAIN_H_INCLUDED__

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/wglew.h>

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
	const array<float, 2> terrain_scale = {{10, 700}};

	void generate_stars(unsigned int);
	float star_size = 0.08;
	vector<array<float, 2>> stars;

	void create_ground_vbo();
	GLuint ground_vbo;
	unsigned int ground_vbo_size;
	void add_cloud_vertex(int, int, float *, float *, float *);


public:
	WorldTerrain(Game& g);

	void initialize();

	void draw_terrain();
	void draw_skypbox();
	float get_height(const float, const float);
};

#endif