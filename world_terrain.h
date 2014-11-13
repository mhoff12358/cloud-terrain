
#ifndef __WORLD_TERRAIN_H_INCLUDED__
#define __WORLD_TERRAIN_H_INCLUDED__

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/wglew.h>

#include <iostream>
#include <array>
#include <vector>
#include <cmath>
#include <stdlib.h>

#include "cloud.h"
#include "perlin_grid.h"
#include "perlin.h"
#include "my_math.h"

#include "terrain_writer.h"
#include "terrain_map.h"
#include "simplex_generator.h"

#include "shadow_height_map.h"

#define skybox_num_tri 36*3
#define _USE_MATH_DEFINES

class Game;

class WorldTerrain {
private:
	Game& game;
	array<int, 4> grid_size = {{-128, -128, 128, 128}}; //minx, miny, maxx, maxy
	// CloudGrid world_grid = CloudGrid("bah", 0.01, 24.0);
	// PerlinGrid world_grid = PerlinGrid(array<int, 4>({{-2, -2, 2, 2}}), 64);
	SimplexGenerator generator;
	TerrainWriter writer;
	TerrainMap world_map;

	ShadowHeightMap shadow_map;

	const array<float, 2> terrain_scale = {{10, 100}};
	const float sample_scale = .1;

	// Position of the sun relative to the viewer. This means the sun is
	// shining in a direction of -sun_position.
	std::array<float, 3> sun_position = {{0.0f, 0.0f, 0.0f}};
	float ambient_brightness = 0.5;
	
	void create_skybox_vbo();
	GLuint skybox_vbo;

	void create_stars_vbo();
	GLuint stars_vbo;
	GLuint sun_vbo;
	unsigned int sun_vbo_size = 6*3;

	void generate_stars(unsigned int);
	float star_size = 0.01;
	vector<array<float, 2>> stars;

	void create_ground_vbo();
	GLuint ground_vbo;
	unsigned int ground_vbo_size;
	unsigned int loc_vbo_size;
	unsigned int col_vbo_size;
	void add_cloud_vertex(int, int, float *, float *);

	void create_ocean_vbo();
	GLuint ocean_vbo;
	unsigned int ocean_vbo_size = 6*3;

public:
	WorldTerrain(Game& g);

	void initialize();

	void draw_terrain();
	void draw_skypbox();
	float get_height(const float, const float);
	const float* get_scale();

	void position_sun(array<float, 3> new_position);

	void write_grid(const string);
};

#endif