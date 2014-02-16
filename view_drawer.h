
#ifndef __VIEW_DRAWER_H_INCLUDED__
#define __VIEW_DRAWER_H_INCLUDED__

// #include "main.h"
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

class Game;

class ViewDrawer {
private:
	int var;

	const int SCREEN_WIDTH = 1000;
	const int SCREEN_HEIGHT = 800;

public:
	ViewDrawer(int bah);

	void initialize();
	void deinitialize();

	void draw_screen();
};

#endif