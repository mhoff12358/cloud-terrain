

#ifndef __PERLIN_GRID_H_INCLUDED__
#define __PERLIN_GRID_H_INCLUDED__

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <array>
#include <math.h>

#include "perlin.h"

using std::string;
using std::hash;
using std::list;
using std::vector;
using std::array;

class PerlinGrid {
private:
	unsigned int num_samples; //Number of samples per page of the grid
	array<int, 4> page_indices = {{0, 0, 0, 0}}; //minx, miny, sizex, sizey

	PerlinNoisePage page_array[3][4][3];

	PerlinNoisePage& get_page(unsigned int x, unsigned int y, unsigned int z); //Page index of the page you want to get

public:
	PerlinGrid(array<int, 4>, unsigned int);

	void set_page_range(array<int, 4>);

	float get_height(unsigned int x, unsigned int y);
	array<float, 3> get_normal(unsigned int x, unsigned int y, const array<float, 2>&);
};

#endif