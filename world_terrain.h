
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
#include "perlin_grid.h"
#include "perlin.h"

#define skybox_num_tri 36*3

class Game;

class WorldTerrain {
private:
	Game& game;
	array<int, 4> grid_size = {{0, 0, 256, 256}}; //minx, miny, maxx, maxy
	// CloudGrid world_grid = CloudGrid("bah", 0.01, 24.0);
	PerlinGrid world_grid = PerlinGrid(array<int, 4>({{0, 0, 4, 4}}), 64);

	const array<float, 2> terrain_scale = {{10, 100}};
	const float sample_scale = .1;

	array<float, 3>sun_dir = {{-1/pow(2, .5), 0, -1/pow(2, .5)}};
	
	void create_skybox_vbo();
	GLuint skybox_vbo;

	void create_stars_vbo();
	GLuint stars_vbo;

	void generate_stars(unsigned int);
	float star_size = 0.01;
	vector<array<float, 2>> stars;

	void create_ground_vbo();
	GLuint ground_vbo;
	unsigned int ground_vbo_size;
	unsigned int loc_vbo_size;
	unsigned int col_vbo_size;
	unsigned int nor_vbo_size;
	void add_cloud_vertex(int, int, float *, float *, float *);

public:
	WorldTerrain(Game& g);

	void initialize();

	void draw_terrain();
	void draw_skypbox();
	float get_height(const float, const float);
	const float* get_scale();

	void write_grid(const string);
};

#endif