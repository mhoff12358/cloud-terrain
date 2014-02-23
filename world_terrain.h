
#ifndef __WORLD_TERRAIN_H_INCLUDED__
#define __WORLD_TERRAIN_H_INCLUDED__

#include "cloud.h"
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>

class Game;

class WorldTerrain {
private:
	Game& game;
	CloudGrid cg = CloudGrid("bah", 0.01, 24.0);

public:
	WorldTerrain(Game& g);

	void draw_terrain();
};

#endif