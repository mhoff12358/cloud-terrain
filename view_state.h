
#ifndef __VIEW_STATE_H_INCLUDED__
#define __VIEW_STATE_H_INCLUDED__

#include <SDL2/SDL.h>
#include <string>

class ViewState {
private:
	const int SCREEN_WIDTH = 1000;
	const int SCREEN_HEIGHT = 800;

	SDL_Window * window = NULL;
	SDL_Renderer * renderer = NULL;
	SDL_GLContext context;
	std::string creation_errors;

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
	int check_valid();

	void render_screen();

};

#endif