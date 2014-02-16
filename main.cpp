#include <iostream>
#include <SDL2/SDL.h>

#include "main.h"

void Game::exit_program(int exit_code) {
	vd.deinitialize();

	vs.destroy_renderer();
	vs.destroy_window();
	vs.deinitialize();

	exit(exit_code);
}

void Game::mainloop() {
	vs.initialize();
	vs.create_window();
	vs.create_renderer();
	vs.create_GL_context();

	vd.initialize();

	while (1) {
		if (ioc.process_events() != 0){
			exit_program(1);
		}

		vd.draw_screen();
		vs.render_screen();
	}
}

int main(int argc, char ** argv) {
	Game g;

	g.mainloop();

	return 0;

}