#include "perlin_grid.h"

unsigned int perlin_grid_mod(int a, unsigned int b) {
	if (a < 0) {
		return b+(a%b);
	}
	return a%b;
}

unsigned int perlin_grid_div(int a, unsigned int b) {
	if (a >= 0) {
		return a / b;
	} else {
		return -(-a-1)/b-1;
	}
}

PerlinGrid::PerlinGrid(array<int, 4> page_area, unsigned int n_s) : num_samples(n_s) {
	set_page_range(page_area);
	for (unsigned int x_i = 0; (int)x_i < page_area[2]; x_i++) {
		for (unsigned int y_i = 0; (int)y_i < page_area[3]; y_i++) {
			for (unsigned int z_i = 0; (int)z_i < 3; z_i++) {
				get_page(x_i, y_i, z_i).set_values(array<unsigned int, 2>({{x_i, y_i}}), 256, 16*pow(2, z_i));
				get_page(x_i, y_i, z_i).cache_all();
			}
		}
	}
}

void PerlinGrid::set_page_range(array<int, 4> page_area) {
	page_indices = page_area;
}

PerlinNoisePage& PerlinGrid::get_page(unsigned int x, unsigned int y, unsigned int z) {
	return page_array[x][y][z];
}

float PerlinGrid::get_height(unsigned int x, unsigned int y) {
	float height = 0.0;
	for (unsigned int z = 0; z < 3; z++) {
		height += pow(0.5, z)*get_page(perlin_grid_div(x, num_samples), perlin_grid_div(y, num_samples), z).get_height(perlin_grid_mod(x, num_samples), perlin_grid_mod(y, num_samples));
	}
	return height;
}

array<float, 3> PerlinGrid::get_normal(unsigned int x, unsigned int y, const array<float, 2>& scale) {
	return get_page(perlin_grid_div(x, num_samples), perlin_grid_div(y, num_samples), 2).get_normal(perlin_grid_mod(x, num_samples), perlin_grid_mod(y, num_samples), scale);
}