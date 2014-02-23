
#ifndef __VIEW_DRAWER_H_INCLUDED__
#define __VIEW_DRAWER_H_INCLUDED__

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

class Game;

class ViewDrawer {
private:
	Game& game;

	const int SCREEN_WIDTH = 1000;
	const int SCREEN_HEIGHT = 800;

public:
	ViewDrawer(Game& g) : game(g) {}

	void initialize();
	void deinitialize();

	void draw_screen();
};

#endif