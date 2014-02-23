#include <iostream>

#include "view_drawer.h"
#include "main.h"

void ViewDrawer::initialize() {
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-0.125, 0.125, -0.1, 0.1, 0.1, 1000);
	// gluPerspective(45.0f,(GLfloat)SCREEN_WIDTH/(GLfloat)SCREEN_HEIGHT,0.1f,100.0f);

	// SDL_GL_SetSwapInterval(1);
}

void ViewDrawer::deinitialize() {

}

void ViewDrawer::draw_screen() {
	// glClearColor(0, 0, 1.0, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Orient the world to the camera
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glScalef(.01, .01, .1);

	//Move the camera based on the player's location
	glTranslatef(-game.get_logic().get_player_loc(0), -game.get_logic().get_player_loc(1), 0.0);

	//Reorient the camera based on the player's orientation
	glRotatef(-game.get_logic().get_player_ori(0), 0.0, 0.0, 1.0);
	// glRotatef(-)

	game.get_terrain().draw_terrain();
	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 0.0f);
      glVertex3f( 1.0f, 1.0f, -1.0f);
      glVertex3f(-1.0f, 1.0f, -1.0f);
      glVertex3f(-1.0f, 1.0f,  1.0f);
      glVertex3f( 1.0f, 1.0f,  1.0f);
	glEnd();
}