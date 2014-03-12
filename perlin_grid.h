

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

#define hvt uint32_t

using std::string;
using std::hash;
using std::list;
using std::vector;
using std::array;

class PerlinGrid {
private:
	uint64_t seed;

	uint64_t fasthash64(const void *, size_t, uint64_t);
	uint32_t hashfn(void *, size_t);
	hvt pointhash(int32_t, int32_t, uint64_t);

	float interpolate_weight_fn(float);
	float interpolate_weight_fn_der(float);
	float grid_value_interpolate(float, float, float);
	float grid_value_interpolate_der(float, float, float);
	float grid_value(float, float, float, float);

	array<float, 2> point_gradient(int, int, uint64_t);

	array<float, 3> vertex_normalize(array<float, 3>);

public:
	PerlinGrid(const uint64_t);

	float get_height(const float, const float);
	float get_point(const int, const int);
	array<float, 3> get_tangent(const float, const float, const float, const float, const array<float, 2>&);
	array<float, 3> get_normal(const float, const float, const array<float, 2>&);

	void write_grid(const string);
};

#endif