#include "io_controller.h"

int IOController::process_events() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			return 1;
		} else if (event.type == SDL_KEYDOWN) {
			std::cout << "hello" << std::endl;
		} else if (event.type == SDL_MOUSEBUTTONDOWN) {
			return 1;
		}
	}
	return 0;
}