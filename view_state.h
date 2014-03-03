
#ifndef __VIEW_STATE_H_INCLUDED__
#define __VIEW_STATE_H_INCLUDED__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include <string>
#include <cstdio>

#include "shader.h"

class ViewState {
private:
	const int SCREEN_WIDTH = 1000;
	const int SCREEN_HEIGHT = 800;

	SDL_Window * window = NULL;
	SDL_Renderer * renderer = NULL;
	SDL_GLContext context;
	std::string creation_errors;

	ShaderProgram skybox_shad;

public:
	SDL_Window * get_window() {return window;};
	SDL_Renderer * get_renderer() {return renderer;};

	int initialize();
	void deinitialize();

	void create_window();
	void destroy_window();
	void create_renderer();
	void destroy_renderer();
	void create_GL_context();
	void create_shaders();
	int check_valid();

	void load_images();
	void unload_images();

	void render_screen();

	SDL_Texture * grump;
	GLuint grumptex;

	GLuint get_skybox_prog();
	GLuint get_ground_prog();
	
	ShaderProgram ground_shad;
};

#endif