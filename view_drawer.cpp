#include <iostream>

#include "view_drawer.h"
#include "main.h"


void ViewDrawer::initialize() {
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
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

	//Reorient the camera based on the player's orientation
	float theta = -game.get_logic().get_player_ori(0);
	glRotatef(theta, 0.0, 1.0, 0.0);
	glRotatef(-game.get_logic().get_player_ori(1), cos(theta*M_PI/180.0), 0.0, sin(theta*M_PI/180.0));

	//Draw the skybox
	game.get_terrain().draw_skybox();

	glPushMatrix();
	//Orient the world to the camera
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	// glScalef(1, 1, 10);

	//Move the camera based on the player's location
	// glTranslatef(-game.get_logic().get_player_loc(0), -game.get_logic().get_player_loc(1), -game.get_logic().get_player_loc(2));
	glTranslatef(-game.get_logic().get_player_loc(0), -game.get_logic().get_player_loc(1), -game.get_logic().get_player_loc(2));

	game.get_terrain().draw_terrain();

	glPopMatrix();
}