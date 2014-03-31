
#include "perlin.h"

int perlin_mod(int a, int b) {
	if (a < 0) {
		return a%b;
	}
	return b+(a%b);
}

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

uint32_t PerlinNoisePage::pointhash(const int32_t x, const int32_t y, const uint64_t mod) {
	int64_t data = x;
	data += (int64_t)y << 32;
	uint32_t retval = fasthash64(&data, 8, mod);
	return retval;
}

//Constructor/Destructor
PerlinNoisePage::PerlinNoisePage() {}

PerlinNoisePage::PerlinNoisePage(array<int, 2> p_o, unsigned int n_s, unsigned int freq):
page_offset(p_o), num_samples(n_s), frequency(freq) {
	cache_all();
}

PerlinNoisePage::~PerlinNoisePage() {
	cache_none();
}

void PerlinNoisePage::set_values(array<int, 2> p_o, unsigned int n_s, unsigned int freq) {
	page_offset = p_o;
	num_samples = n_s;
	frequency = freq;
}

void PerlinNoisePage::cache_all() {
	begin_caching(PNP_GRADIENTS);
	begin_caching(PNP_HEIGHTS);
	begin_caching(PNP_NORMALS);
}

void PerlinNoisePage::cache_none() {
	stop_caching(PNP_GRADIENTS);
	stop_caching(PNP_HEIGHTS);
	stop_caching(PNP_NORMALS);
}

//Actual computation
array<float, 2> PerlinNoisePage::compute_gradient(unsigned int x, unsigned int y) {
	assert (x <= frequency);
	assert (y <= frequency);
	int totalx = page_offset[0]*frequency + x;
	int totaly = page_offset[1]*frequency + y;
	// std::cout << page_offset[0] << "\t" << x << "\t" << page_offset[1] << "\t" << y << "\t" << totalx << "\t" << totaly << std::endl;
	array<float, 2> retval = array<float, 2>({{
		(float)pointhash(totalx, totaly, frequency)/(float)(uint32_t)(-1)*2-1,
		(float)pointhash(totalx, totaly, frequency ^ (uint64_t)1<<63)/(float)(uint32_t)(-1)*2-1
	}});
	return retval;
}

float PerlinNoisePage::interpolate_weight_fn(float s) {
	return ((6*s-15)*s+10)*s*s*s;
}

float PerlinNoisePage::grid_value_interpolate(float a, float b, float s) {
	assert (s >= 0 && s <= 1);
	float scale = interpolate_weight_fn(s);
	return (1-scale)*a+(scale)*b;
}

float PerlinNoisePage::grid_value(float px, float py, unsigned int gx, unsigned int gy) { //Computes the contribution of a grid point
	//g_ are the indices of the gradient that should be used
	//p_ are the vector that points from the gradient point to x, y
	array<float, 2> grad = get_gradient(gx, gy);
	return grad[0]*(px)+grad[1]*(py);
}

float PerlinNoisePage::raw_compute_height(float x, float y) {
	float px = x * frequency;
	float py = y * frequency;
	unsigned int gx = (unsigned int)px; //These should be gradient indeces
	unsigned int gy = (unsigned int)py;
	px -= gx; //Now these should be proportions between gradients
	py -= gy;

	assert (gx < frequency);
	assert (gy < frequency);
	assert (0 <= px && px <= 1);
	assert (0 <= py && py <= 1);

	// float retval = grid_value_interpolate(
	// 	grid_value_interpolate(
	// 		grid_value(px, py, gx, gy),
	// 		grid_value(px-1, py, gx+1, gy),
	// 		px
	// 	),
	// 	grid_value_interpolate(
	// 		grid_value(px, py-1, gx, gy+1),
	// 		grid_value(px-1, py-1, gx+1, gy+1),
	// 		px
	// 	),
	// 	py
	// );
	// return retval;
	return lerp(lerp(grid_value(px, py, gx, gy),
		             grid_value(px-1, py, gx+1, gy),
		             interpolate_weight_fn(px)),
	            lerp(grid_value(px, py-1, gx, gy+1),
	            	 grid_value(px-1, py-1, gx+1, gy+1),
	            	 interpolate_weight_fn(px)),
	            interpolate_weight_fn(py));
}

float PerlinNoisePage::finalize_height(float height, float x, float y) {
	return get_amplitude(x, y)*height;
}

float PerlinNoisePage::compute_height(unsigned int x, unsigned int y, float pxo, float pyo) {
	//x and y should be in the range [0,num_samples)
	//pxo and pyo should be in the range [0,1)
	//thus x+pxo should be in the range [0,num_samples)
	//thus (x+pxo)/num_samples should be the proportion through the page.
	//At this point the use of simple integer x and y values for easy caching
	//is no longer necessary so they can be turned into fractions on the range
	//[0, 1) by doing (x+pxo)/num_samples
	assert (x < num_samples); // x in [0, num_samples)
	assert (y < num_samples);
	assert (pxo >= 0 && pxo <= 1); // pxo is a fraction
	assert (pyo >= 0 && pyo <= 1); // pxo is a fraction

	float pro_x = (((float)x)+pxo)/num_samples;
	float pro_y = (((float)y)+pyo)/num_samples;
	assert (0 <= pro_x && pro_x <= 1);
	assert (0 <= pro_y && pro_y <= 1);
	return finalize_height(raw_compute_height(pro_x, pro_y), pro_x, pro_y);
}


array<float, 3> PerlinNoisePage::compute_normal(unsigned int x, unsigned int y, const array<float, 2>& scale) {
	float epsilon = 0.001;
	float h = get_height(x, y);
	float hx = compute_height(x, y, epsilon, 0);
	float hy = compute_height(x, y, 0, epsilon);
	return vertex_normalize(array<float, 3>({{-epsilon*(hx-h), -epsilon*(hy-h), epsilon*epsilon}}));
}

//Outward interface
array<float, 2> PerlinNoisePage::get_gradient(unsigned int x, unsigned int y) {
	if (x < num_samples && y < num_samples) {
		if (cache_flag[PNP_GRADIENTS]) {
			pair<bool, array<float, 2>>* cache_pointer = gradient_cache+(y*(frequency+1))+x;
			if (!cache_pointer->first) {
				cache_pointer->second = compute_gradient(x, y);
			}
			return cache_pointer->second;
		} else {
			return compute_gradient(x, y);
		}
	} else {
		std::cout << "GET GRADIENT FAILURE: x: " << x << "\ty: " << y << std::endl;
		return array<float, 2>({{1.0, 0.0}});
	}
}

float PerlinNoisePage::get_height(unsigned int x, unsigned int y) {
	if (x < num_samples && y < num_samples) {
		if (cache_flag[PNP_GRADIENTS]) {
			pair<bool, float>* cache_pointer = height_cache+(y*num_samples)+x;
			if (!cache_pointer->first) {
				cache_pointer->second = compute_height(x, y, 0, 0);
			}
			return cache_pointer->second;
		} else {
			return compute_height(x, y, 0, 0);
		}
	} else {
		std::cout << "GET HEIGHT FAILURE: x: " << x << "\ty: " << y << std::endl;
		return 0.0;
	}
}

float PerlinNoisePage::get_height(unsigned int x, unsigned int y, float pxo, float pyo) {
	return compute_height(x, y, pxo, pyo);
}

array<float, 3> PerlinNoisePage::get_normal(unsigned int x, unsigned int y, const array<float, 2>& scale) {
	if (cache_flag[PNP_GRADIENTS]) {
		pair<bool, array<float, 3>>* cache_pointer = normal_cache+(y*(frequency+1))+x;
		if (!cache_pointer->first) {
			cache_pointer->second = compute_normal(x, y, scale);
		}
		return cache_pointer->second;
	} else {
		return compute_normal(x, y, scale);
	}
}

void PerlinNoisePage::begin_caching(PNP_Cache_Id cache_id) {
	// std::cout << "Beginning to cache" << std::endl;
	unsigned int num_vals;
	if (!cache_flag[cache_id]) {
		if (cache_id == PNP_GRADIENTS) {
			num_vals = (frequency+1)*(frequency+1);
			gradient_cache = new pair<bool, array<float, 2>>[num_vals];
			cache_flag[cache_id] = true;
		} else if (cache_id == PNP_HEIGHTS) {
			num_vals = (num_samples)*(num_samples);
			height_cache = new pair<bool, float>[num_vals];
			cache_flag[cache_id] = true;
		} else if (cache_id == PNP_NORMALS) {
			num_vals = (num_samples)*(num_samples);
			normal_cache = new pair<bool, array<float, 3>>[num_vals];
			cache_flag[cache_id] = true;
		}
	}
}

void PerlinNoisePage::stop_caching(PNP_Cache_Id cache_id) {
	// std::cout << "Ceasing to cache" << std::endl;
	if (cache_flag[cache_id]) {
		if (cache_id == PNP_GRADIENTS) {
			delete[] gradient_cache;
			cache_flag[cache_id] = false;
		} else if (cache_id == PNP_HEIGHTS) {
			delete[] height_cache;
			cache_flag[cache_id] = false;
		} else if (cache_id == PNP_NORMALS) {
			delete[] normal_cache;
			cache_flag[cache_id] = false;
		}
	}
}

//The perlin page needs to have its value field outward facing
float PerlinNoisePage::get_amplitude(float x, float y) {
	float retval = amps.get_amplitude(x, y);
	// std::cout << retval << "\t";
	return retval;
}

void PerlinNoisePage::set_amp_field(float * amp_values, unsigned int new_size) {
	amps.set_size(new_size);
	amps.assign_field(amp_values);
}

//Onto the Perlin Value Field
PerlinValueField::PerlinValueField() {
	set_size(0);
}

PerlinValueField::PerlinValueField(unsigned int new_size) {
	set_size(new_size);
}

void PerlinValueField::assign_field(float * values) {
	memcpy(amplitudes.data(), values, sizeof(float)*size*size);
}

void PerlinValueField::assign_field(unsigned int x, unsigned int y, float val) {
	amplitudes[size*x+y] = val;
}

unsigned int PerlinValueField::get_size() {
	return size;
}

void PerlinValueField::set_size(unsigned int new_size) {
	size = new_size;
	amplitudes.resize(size*size);
}

float PerlinValueField::get_raw_amplitude(int x, int y) {
	if (x < (int)size && y < (int)size) {
		return amplitudes.at(size*x+y);
	} else {
		return 0;
	}
}

float PerlinValueField::get_amplitude(float x, float y) {
	//x and y are what proportion through the whole of the value field the
	//requested point is. They should be constrained by [0,1]
	x *= size-1;
	y *= size-1;
	int xind = (int)floor(x);
	float xpart = x - xind;
	int yind = (int)floor(y);
	float ypart = y - yind;

	//Now we've got the indices and the interpolation proportions
	return lerp(lerp(get_raw_amplitude(xind, yind), get_raw_amplitude(xind+1, yind), xpart),
                lerp(get_raw_amplitude(xind, yind+1), get_raw_amplitude(xind+1, yind+1), xpart), ypart);
}