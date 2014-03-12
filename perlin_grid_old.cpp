#include "perlin_grid.h"


PerlinGrid::PerlinGrid(const uint64_t set_seed) {
	seed = set_seed;
}

#define mix(h) ({					\
			(h) ^= (h) >> 23;		\
			(h) *= 0x2127599bf4325c37ULL;	\
			(h) ^= (h) >> 47; })

uint64_t PerlinGrid::fasthash64(const void *buf, size_t len, uint64_t seed)
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

#define MASK_32 (((uint64_t)1<<32)-1)
uint32_t PerlinGrid::hashfn(void * data, size_t numbytes) {
	uint64_t hashval = 0xcbf29ce484222325;
	while (numbytes--) {
		hashval = (*((char*)data+1) ^ hashval) * 0x100000001b3;
	}
	return (uint32_t)(hashval>>32) ^ (hashval & MASK_32);
}

hvt PerlinGrid::pointhash(const int32_t x, const int32_t y, const uint64_t mod) {
	int64_t data = x;
	data += (int64_t)y << 32;
	hvt retval = fasthash64(&data, 8, mod);
	return retval;
}

array<float, 2> PerlinGrid::point_gradient(int x, int y, uint64_t freq) {
	return array<float, 2>({{
		(float)pointhash(x, y, freq)/(float)(hvt)(-1)*2-1,
		(float)pointhash(x, y, freq ^ (uint64_t)1<<63)/(float)(hvt)(-1)*2-1
	}});
}

float dot(float x0, float y0, float x1, float y1) {
	return x0*x1 + y0*y1;
}

float PerlinGrid::grid_value(float x, float y, float px, float py) {
	array<float, 2> grid_grad = point_gradient(px, py, 1);
	return dot(grid_grad[0], grid_grad[1], x-px, y-py);
}

float PerlinGrid::interpolate_weight_fn(float s) {
	return ((6*s-15)*s+10)*s*s*s;
}

float PerlinGrid::interpolate_weight_fn_der(float s) {
	return ((30*s-60)*s+30)*s*s;
}

float PerlinGrid::grid_value_interpolate_der(float a, float b, float s) {
	float scale = interpolate_weight_fn_der(s);
	return scale*(b-a);
	// return scale;
}

float PerlinGrid::grid_value_interpolate(float a, float b, float s) {
	float scale = interpolate_weight_fn(s);
	return (1-scale)*a+(scale)*b;
}

float PerlinGrid::get_height(float x, float y) {
	return grid_value_interpolate(
		grid_value_interpolate(
			grid_value(x, y, floor(x), floor(y)),
			grid_value(x, y, floor(x)+1, floor(y)),
			x-floor(x)
		),
		grid_value_interpolate(
			grid_value(x, y, floor(x), floor(y+1)),
			grid_value(x, y, floor(x)+1, floor(y+1)),
			x-floor(x)
		),
		y-floor(y)
	);
}

float PerlinGrid::get_point(int x, int y) {
	return get_height(x, y);
}

array<float, 3> PerlinGrid::get_tangent(float x0, float y0, float x1, float y1, const array<float, 2>& scale) {
	return array<float, 3>({{
		scale[0]*(x1-x0),
		scale[0]*(y1-y0),
		scale[1]*(get_height(x1, y1)-get_height(x0, y0))
	}});
}

array<float, 3> PerlinGrid::get_normal(const float x, const float y, const array<float, 2>& scale) {
	// float dx = grid_value_interpolate(
	// 	grid_value_interpolate_der(
	// 		grid_value(x, y, floor(x), floor(y)),
	// 		grid_value(x, y, floor(x+1), floor(y)),
	// 		x-floor(x)),
	// 	grid_value_interpolate_der(
	// 		grid_value(x, y, floor(x), floor(y+1)),
	// 		grid_value(x, y, floor(x+1), floor(y+1)),
	// 		x-floor(x)),
	// 	y-floor(y)
	// 	);
	float epsilon = 0.001;
	float h = get_height(x, y);
	float hx = get_height(x+epsilon, y);
	float hy = get_height(x, y+epsilon);
	return vertex_normalize(array<float, 3>({{-epsilon*(hx-h), -epsilon*(hy-h), epsilon*epsilon}}));
}

// array<float, 3> PerlinGrid::get_normal(const float x, const float y, const array<float, 2>& scale) {
// 	array<float, 3> normal = {{interpolate_weight_fn_der(x-floor(x))*scale[1]/scale[0], interpolate_weight_fn_der(y-floor(y))*scale[1]/scale[0], 1}};
// 	return vertex_normalize(normal);
// }

array<float, 3> PerlinGrid::vertex_normalize(array<float, 3> in_vert) {
	float mag = pow(pow(in_vert[0], 2) + pow(in_vert[1], 2) + pow(in_vert[2], 2), 0.5);
	return array<float, 3>({{in_vert[0]/mag, in_vert[1]/mag, in_vert[2]/mag}});
}

void PerlinGrid::write_grid(const string filename) {
	std::ofstream ofile;
	ofile.open(filename);
	for (int x = 0; x < 100; x++) {
		for (int y = 0; y < 100; y++) {
			ofile << get_height(x/4.0, y/4.0) << '|';
		}
		ofile << std::endl;
	}
	ofile.close();
}
