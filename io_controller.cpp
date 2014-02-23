#include "io_controller.h"

#include "main.h"

int IOController::process_events() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			return 1;
		} else if (event.type == SDL_KEYDOWN && event.key.repeat) {
			handle_keypress(event.key.keysym);
		} else if (event.type == SDL_KEYUP && event.key.repeat) {
			handle_keyrelease(event.key.keysym);
		} else if (event.type == SDL_MOUSEBUTTONDOWN) {
			// return 1;
		}
	}
	return 0;
}

void IOController::handle_keypress(SDL_Keysym key_info) {

}

void IOController::handle_keyrelease(SDL_Keysym key_info) {

}

Uint8 IOController::get_keypressed(SDL_Keycode key_info) {
	return SDL_GetKeyboardState(NULL)[SDL_GetScancodeFromKey(key_info)];
}