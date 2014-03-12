

#ifndef __PERLIN_H_INCLUDED__
#define __PERLIN_H_INCLUDED__

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <array>
#include <utility>
#include <math.h>

#define hvt unsigned int32_t

using std::string;
using std::hash;
using std::list;
using std::vector;
using std::array;
using std::pair;

enum PNP_Cache_Id {PNP_GRADIENTS = 0, PNP_HEIGHTS = 0, PNP_NORMALS = 0};

class PerlinNoisePage {
private:
	array<unsigned int, 2> page_offset = {{0, 0}};
	unsigned int num_samples = 0;
	unsigned int frequency = 0;

	pair<bool, array<float, 2>>* gradient_cache = NULL;
	pair<bool, float>* height_cache = NULL;
	pair<bool, array<float, 3>>* normal_cache = NULL;


	array<bool, 3> cache_flag = {{false, false, false}};
	
	array<float, 2> compute_gradient(unsigned int x, unsigned int y); //x and y = [0, frequency)
	float interpolate_weight_fn(float);
	float grid_value_interpolate(float, float, float);
	float grid_value(float, float, unsigned int, unsigned int);
	float compute_height(unsigned int x, unsigned int y, float, float); //x and y = [0, num_samples)
	array<float, 3> vertex_normalize(array<float, 3>);
	array<float, 3> compute_normal(unsigned int x, unsigned int y); //x and y = [0, num_samples)

	uint64_t fasthash64(const void*, size_t, uint64_t);
	uint32_t pointhash(const uint32_t, const uint32_t, const uint64_t);

public:
	PerlinNoisePage(array<unsigned int, 2>, unsigned int, unsigned int);
	~PerlinNoisePage();

	//The get functions are how this program gets its values. First they check
	//if the value is cached, and if so return it, else they call the corresponding
	//compute function and set the cache.
	array<float, 2> get_gradient(unsigned int x, unsigned int y); //x and y = [0, frequency)
	float get_height(unsigned int x, unsigned int y); //x and y = [0, num_samples)
	array<float, 3> get_normal(unsigned int x, unsigned int y); //x and y = [0, num_samples)

	void begin_caching(PNP_Cache_Id);
	void stop_caching(PNP_Cache_Id);
};

#endif