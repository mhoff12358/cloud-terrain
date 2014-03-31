#include "perlin_grid.h"

unsigned int perlin_grid_mod(int a, int b) {
	int retval = a%b;
	if (retval < 0) {
		return retval + b;
	}
	return retval;
}

int perlin_grid_div(int a, int b) {
	if (a >= 0) {
		return a / b;
	} else {
		return -((-a-1)/b+1);
	}
}

#define amp_val (float)pow(0.5f, z_i)*6.0f
#define amp_vals (float)pow(0.5f, z_i)*2.0f
PerlinGrid::PerlinGrid(array<int, 4> page_area, unsigned int n_s) : num_samples(n_s) {
	set_page_range(page_area);
	for (int x_i = page_area[0]; (int)x_i < page_area[2]; x_i++) {
		for (int y_i = page_area[1]; (int)y_i < page_area[3]; y_i++) {
			for (unsigned int z_i = 0; (int)z_i < 3; z_i++) {
				PerlinNoisePage& pnp = get_page(x_i, y_i, z_i);
				float ampfield[9] = {amp_val, amp_vals, amp_val, amp_vals, amp_vals, amp_vals, amp_val, amp_vals, amp_val};
				pnp.set_amp_field(ampfield, 3);
				pnp.set_values(array<int, 2>({{x_i, y_i}}), num_samples, pow(2, z_i));
				pnp.cache_all();
			}
		}
	}
}

void PerlinGrid::set_page_range(array<int, 4> page_area) {
	page_indices = page_area;
}

PerlinNoisePage& PerlinGrid::get_page(int x, int y, int z) {
	return page_array[x-page_indices[0]][y-page_indices[1]][z];
}

float PerlinGrid::get_height(int x, int y) {
	float height = 0.0;
	for (unsigned int z = 0; z < 3; z++) {
		height += get_page(perlin_grid_div(x, num_samples), perlin_grid_div(y, num_samples), z).get_height(perlin_grid_mod(x, num_samples), perlin_grid_mod(y, num_samples));
	}
	return height;
}

float PerlinGrid::get_height(float x, float y) {
	int xind = (int)floor(x);
	float xoff = x-xind;
	int yind = (int)floor(y);
	float yoff = y-yind;

	float height = 0.0;
	for (unsigned int z = 0; z < 3; z++) {
		height += get_page(perlin_grid_div(xind, num_samples), perlin_grid_div(yind, num_samples), z).get_height(perlin_grid_mod(xind, num_samples), perlin_grid_mod(yind, num_samples), xoff, yoff);
	}
	return height;
}

float PerlinGrid::get_height_interp(float x, float y) {
	//This just lineraly interpolates between the points 4 points around x, y
	int x1 = floor(x);
	int x2 = x1+1;
	int y1 = floor(y);
	int y2 = y1+1;
	return lerp(lerp(get_height(x1, y1), get_height(x2, y1), x-x1),
		        lerp(get_height(x1, y2), get_height(x2, y2), x-x1), y-y1);
}

array<float, 3> PerlinGrid::get_normal(int x, int y, const array<float, 2>& scale) {
	// return get_page(perlin_grid_div(x, num_samples), perlin_grid_div(y, num_samples), 2).get_normal(perlin_grid_mod(x, num_samples), perlin_grid_mod(y, num_samples), scale);
	float epsilon = 0.001;
	float h = get_height(x, y)*scale[1];
	float hx = get_height((float)x+epsilon, (float)y)*scale[1];
	float hy = get_height((float)x, (float)y+epsilon)*scale[1];
	//epsilon should be scaled by scale[0], but all values in the vertex normalize have one epsilon
	//so it only needs to be applied to the one with epsilon squared.
	return vertex_normalize(array<float, 3>({{-epsilon*(hx-h), -epsilon*(hy-h), epsilon*epsilon*scale[0]}}));

}



