#include "view_state.h"
#include <iostream>

int ViewState::initialize() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		return 1;
	}

	return 0;
}

void ViewState::deinitialize() {
	SDL_Quit();
}

void ViewState::create_window() {
	window = SDL_CreateWindow("_", 50, 50, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	if (window == NULL) {
		// creation_errors << SDL_GetError() << std::endl;
	}
}

void ViewState::destroy_window() {
	if (window != NULL) {
		SDL_DestroyWindow(window);
		window = NULL;
	}
}

void ViewState::create_renderer() {
	if (window == NULL) {
		renderer = NULL;
	} else {
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (renderer == NULL) {
			// creation_errors << SDL_GetError() << std::endl;
		}
	}
}
void ViewState::destroy_renderer() {
	if (renderer != NULL) {
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}
}

void ViewState::create_GL_context() {
	context = SDL_GL_CreateContext(window);
}

int ViewState::check_valid() {
	if (window != NULL && renderer != NULL) {
		return 0;
	}
	return 1;
}

void ViewState::render_screen() {
	SDL_GL_SwapWindow(window);
	// SDL_RenderPresent(renderer);
}