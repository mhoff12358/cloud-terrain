
#include "perlin.h"

//Hashing/randomness nitty gritty stuff
#define mix(h) ({					\
			(h) ^= (h) >> 23;		\
			(h) *= 0x2127599bf4325c37ULL;	\
			(h) ^= (h) >> 47; })
uint64_t PerlinNoisePage::fasthash64(const void *buf, size_t len, uint64_t seed)
{
	const uint64_t    m = 0x880355f21e6d1965ULL;
	const uint64_t *pos = (const uint64_t *)buf;
	const uint64_t *end = pos + (len / 8);
	const unsigned char *pos2;
	uint64_t h = seed ^ (len * m);
	uint64_t v;

	while (pos != end) {
		v  = *pos++;
		h ^= mix(v);
		h *= m;
	}

	pos2 = (const unsigned char*)pos;
	v = 0;

	switch (len & 7) {
	case 7: v ^= (uint64_t)pos2[6] << 48;
	case 6: v ^= (uint64_t)pos2[5] << 40;
	case 5: v ^= (uint64_t)pos2[4] << 32;
	case 4: v ^= (uint64_t)pos2[3] << 24;
	case 3: v ^= (uint64_t)pos2[2] << 16;
	case 2: v ^= (uint64_t)pos2[1] << 8;
	case 1: v ^= (uint64_t)pos2[0];
		h ^= mix(v);
		h *= m;
	}

	return mix(h);
}

uint32_t PerlinNoisePage::pointhash(const uint32_t x, const uint32_t y, const uint64_t mod) {
	int64_t data = x;
	data += (int64_t)y << 32;
	uint32_t retval = fasthash64(&data, 8, mod);
	return retval;
}

//Constructor/Destructor
PerlinNoisePage::PerlinNoisePage(array<unsigned int, 2> p_o, unsigned int n_s, unsigned int freq):
page_offset(p_o), num_samples(n_s), frequency(freq) {
}

PerlinNoisePage::~PerlinNoisePage() {
	stop_caching(PNP_GRADIENTS);
	stop_caching(PNP_HEIGHTS);
	stop_caching(PNP_NORMALS);
}

//Actual computation
array<float, 2> PerlinNoisePage::compute_gradient(unsigned int x, unsigned int y) {
	x = page_offset[0]*frequency + x;
	y = page_offset[1]*frequency + y;
	array<float, 2> retval = array<float, 2>({{
		(float)pointhash(x, y, frequency)/(float)(uint32_t)(-1)*2-1,
		(float)pointhash(x, y, frequency ^ (uint64_t)1<<63)/(float)(uint32_t)(-1)*2-1
	}});
	return retval;
}

float PerlinNoisePage::interpolate_weight_fn(float s) {
	return ((6*s-15)*s+10)*s*s*s;
}

float PerlinNoisePage::grid_value_interpolate(float a, float b, float s) {
	float scale = interpolate_weight_fn(s);
	return (1-scale)*a+(scale)*b;
}

float PerlinNoisePage::grid_value(float px, float py, unsigned int gx, unsigned int gy) { //Computes the contribution of a grid point
	array<float, 2> grad = get_gradient(gx, gy);
	return grad[0]*(gx-px)+grad[1]*(gy-py);
}
float PerlinNoisePage::compute_height(unsigned int x, unsigned int y, float pxo, float pyo) {
	float px = (float)x/num_samples + pxo;
	float py = (float)y/num_samples + pyo;
	unsigned int gx = (int)(px*(frequency+1));
	unsigned int gy = (int)(py*(frequency+1));
	return grid_value_interpolate(
		grid_value_interpolate(
			grid_value(px, py, gx, gy),
			grid_value(px, py, gx+1, gy),
			px-gx
		),
		grid_value_interpolate(
			grid_value(px, py, gx, gy+1),
			grid_value(px, py, gx+1, gy+1),
			px-gx
		),
		py-gy
	);
}

array<float, 3> PerlinNoisePage::vertex_normalize(array<float, 3> in_vert) {
	float mag = pow(pow(in_vert[0], 2) + pow(in_vert[1], 2) + pow(in_vert[2], 2), 0.5);
	return array<float, 3>({{in_vert[0]/mag, in_vert[1]/mag, in_vert[2]/mag}});
}

array<float, 3> PerlinNoisePage::compute_normal(unsigned int x, unsigned int y) {
	float epsilon = 0.001;
	float h = get_height(x, y);
	float hx = compute_height(x, y, epsilon, 0);
	float hy = compute_height(x, y, 0, epsilon);
	return vertex_normalize(array<float, 3>({{-epsilon*(hx-h), -epsilon*(hy-h), epsilon*epsilon}}));

}

//Outward interface
array<float, 2> PerlinNoisePage::get_gradient(unsigned int x, unsigned int y) {
	if (cache_flag[PNP_GRADIENTS]) {
		pair<bool, array<float, 2>>* cache_pointer = gradient_cache+(y*(frequency+1))+x;
		if (!cache_pointer->first) {
			cache_pointer->second = compute_gradient(x, y);
		}
		return cache_pointer->second;
	} else {
		return compute_gradient(x, y);
	}
}

float PerlinNoisePage::get_height(unsigned int x, unsigned int y) {
	if (cache_flag[PNP_GRADIENTS]) {
		pair<bool, float>* cache_pointer = height_cache+(y*num_samples)+x;
		if (!cache_pointer->first) {
			cache_pointer->second = compute_height(x, y, 0, 0);
		}
		return cache_pointer->second;
	} else {
		return compute_height(x, y, 0, 0);
	}
}

array<float, 3> PerlinNoisePage::get_normal(unsigned int x, unsigned int y) {
	if (cache_flag[PNP_GRADIENTS]) {
		pair<bool, array<float, 3>>* cache_pointer = normal_cache+(y*(frequency+1))+x;
		if (!cache_pointer->first) {
			cache_pointer->second = compute_normal(x, y);
		}
		return cache_pointer->second;
	} else {
		return compute_normal(x, y);
	}
}

void PerlinNoisePage::begin_caching(PNP_Cache_Id cache_id) {
	unsigned int num_vals;
	if (!cache_flag[cache_id]) {
		if (cache_id == PNP_GRADIENTS) {
			num_vals = (frequency+1)*(frequency+1);
			gradient_cache = new pair<bool, array<float, 2>>[num_vals];
		} else if (cache_id == PNP_HEIGHTS) {
			num_vals = (num_samples)*(num_samples);
			height_cache = new pair<bool, float>[num_vals];
		} else if (cache_id == PNP_NORMALS) {
			num_vals = (num_samples)*(num_samples);
			normal_cache = new pair<bool, array<float, 3>>[num_vals];
		}
	}
}

void PerlinNoisePage::stop_caching(PNP_Cache_Id cache_id) {
	if (cache_flag[cache_id]) {
		if (cache_id == PNP_GRADIENTS) {
			delete[] gradient_cache;
		} else if (cache_id == PNP_HEIGHTS) {
			delete[] height_cache;
		} else if (cache_id == PNP_NORMALS) {
			delete[] normal_cache;
		}
	}
}
