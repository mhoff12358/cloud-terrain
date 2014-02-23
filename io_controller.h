
#ifndef __IO_CONTROLLER_H_INCLUDED__
#define __IO_CONTROLLER_H_INCLUDED__

#include <iostream>
#include <array>

#include <SDL2/SDL.h>
// #include <SDL2/SDL_keycode.h>

using std::array;

class Game;

class IOController {
private:
	Game& game;

	void handle_keypress(SDL_Keysym);
	void handle_keyrelease(SDL_Keysym);

public:
	IOController(Game& g) : game(g) {}

	int process_events();
	Uint8 get_keypressed(SDL_Keycode);
	
	//Movement key bindings: up, right, back, left
	const array<SDL_Keycode, 4> MOVE_KEYS = {{SDLK_w, SDLK_d, SDLK_s, SDLK_a}};
	const array<float, 2> LOOK_SENSITIVITY = {{0.75, 0.25}};
};

#endif