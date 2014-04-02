#include "view_state.h"
#include <iostream>

char* filetobuf(char *file, int * length)
{
    FILE *fptr;
    char *buf;

    fptr = fopen(file, "r"); /* Open file for reading */
    if (!fptr) /* Return NULL on failure */
        return NULL;
    fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
    *length = ftell(fptr); /* Find out how many bytes into the file we are */
    buf = new char[*length+1]; /* Allocate a buffer for the entire length of the file and a null terminator */
    fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
    fread(buf, *length, 1, fptr); /* Read the contents of the file in to the buffer */
    fclose(fptr); /* Close the file */
    buf[*length] = 0; /* Null terminator */

    return buf; /* Return the buffer */
}

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

void ViewState::create_shaders() {
	ground_shad.create_program();
	ground_shad.add_shader(GL_VERTEX_SHADER, "ground.vert");
	ground_shad.add_shader(GL_FRAGMENT_SHADER, "ground.frag");
	ground_shad.add_attribute("in_position");
	ground_shad.add_attribute("in_color");
	ground_shad.add_attribute("in_normal");
	ground_shad.add_attribute("frag_color");
	// glBindFragDataLocation(ground_shad.get_program(), 0, "frag_color");
	ground_shad.link_program();
	std::cout << "PROJ MATRIX VAL: " << glGetUniformLocation(ground_shad.get_program(), "proj_matrix") << std::endl;
	std::cout << "VIEW MATRIX VAL: " << glGetUniformLocation(ground_shad.get_program(), "view_matrix") << std::endl;
	std::cout << "SUN DIRECTION VAL: " << glGetUniformLocation(ground_shad.get_program(), "sun_direction") << std::endl;

	ocean_shad.create_program();
	ocean_shad.add_shader(GL_VERTEX_SHADER, "ocean.vert");
	ocean_shad.add_shader(GL_FRAGMENT_SHADER, "ocean.frag");
	ocean_shad.add_attribute("in_position");
	ocean_shad.add_attribute("frag_color");
	ocean_shad.link_program();

	skybox_shad.create_program();
	skybox_shad.add_shader(GL_VERTEX_SHADER, "skybox.vert");
	skybox_shad.add_shader(GL_FRAGMENT_SHADER, "skybox.frag");
	skybox_shad.add_attribute("in_position");
	skybox_shad.add_attribute("in_color");
	skybox_shad.add_attribute("frag_color");
	skybox_shad.link_program();
	std::cout << "IN POSITION LOC: " << glGetAttribLocation(skybox_shad.get_program(), "in_position") << std::endl;
	std::cout << "IN COLOR LOC: " << glGetAttribLocation(skybox_shad.get_program(), "in_color") << std::endl;

	stars_shad.create_program();
	stars_shad.add_shader(GL_VERTEX_SHADER, "stars.vert");
	stars_shad.add_shader(GL_FRAGMENT_SHADER, "stars.frag");
	stars_shad.add_attribute("in_position");
	stars_shad.link_program();

	sun_shad.create_program();
	sun_shad.add_shader(GL_VERTEX_SHADER, "sun.vert");
	sun_shad.add_shader(GL_FRAGMENT_SHADER, "sun.frag");
	sun_shad.add_attribute("in_position");
	sun_shad.link_program();
}

GLuint ViewState::get_skybox_prog() {
	return skybox_shad.get_program();
}

GLuint ViewState::get_ground_prog() {
	return ground_shad.get_program();
}

GLuint ViewState::get_ocean_prog() {
	return ocean_shad.get_program();
}

GLuint ViewState::get_stars_prog() {
	return stars_shad.get_program();
}

GLuint ViewState::get_sun_prog() {
	return sun_shad.get_program();
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

void ViewState::load_images() {
	SDL_Surface * sgrump = IMG_Load("images/star.png");
	if (!sgrump) {
		std::cout << "Image loading error" << std::endl;
	}

	glGenTextures(1, &grumptex);
	glBindTexture(GL_TEXTURE_2D, grumptex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sgrump->w, sgrump->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, sgrump->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(sgrump);
}

void ViewState::unload_images() {
	glDeleteTextures(1, &grumptex);
}