#include "cloud.h"
#include <time.h>

// using namespace std;}

void out_vert(array<float, 3> a) {
	std::cout << a[0] << "\t" << a[1] << "\t" << a[2] << std::endl;
}

array<float, 3> cloud_cross_prod(array<float, 3> a, array<float, 3> b) {
	array<float, 3> cp;
	cp[0] = a[1]*b[2]-a[2]*b[1];
	cp[1] = a[2]*b[0]-a[0]*b[2];
	cp[2] = a[0]*b[1]-a[1]*b[0];
	return cp;
}

array<float, 3> cloud_norm_vec(array<float, 3> in_vec) {
	// out_vert(in_vec);
	float mag = pow(pow(in_vec[0], 2) + pow(in_vec[1], 2) + pow(in_vec[2], 2), 0.5);
	// std::cout << mag << std::endl;
	array<float, 3> norm_vec;
	for (unsigned int i = 0; i < 3; i++) {
		norm_vec[i] = in_vec[i] / mag;
	}
	return norm_vec;
}

#define mix(h) ({					\
			(h) ^= (h) >> 23;		\
			(h) *= 0x2127599bf4325c37ULL;	\
			(h) ^= (h) >> 47; })

uint64_t fasthash64(const void *buf, size_t len, uint64_t seed)
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

CloudGrid::CloudGrid(const string seed_val, const float cloud_prob, const float max_rad) {
	seed = seed_val;
	cloud_hashcap = (hvt)(((hvt)-1)*cloud_prob+.5);
	max_radius = max_rad;
	over_scan = (int)max_rad;
}

hvt CloudGrid::pointhash(const int32_t x, const int32_t y) {
	return pointhash(x, y, 0);
}

#define MASK_32 (((uint64_t)1<<32)-1)
uint32_t hashfn(void * data, size_t numbytes) {
	uint64_t hashval = 0xcbf29ce484222325;
	while (numbytes--) {
		hashval = (*((char*)data+1) ^ hashval) * 0x100000001b3;
	}
	return (uint32_t)(hashval>>32) ^ (hashval & MASK_32);
}

hvt CloudGrid::pointhash(const int32_t x, const int32_t y, const uint64_t mod) {
	int64_t data = x;
	data += (int64_t)y << 32;
	hvt retval = fasthash64(&data, 8, mod);
	return retval;
}

bool CloudGrid::check_point(const int x, const int y) {
	return check_point(pointhash(x, y));
}

bool CloudGrid::check_point(const hvt hashval) {
	return (hashval < cloud_hashcap);
}

float CloudGrid::point_height(const int x, const int y) {
	return point_height(pointhash(x, y));
}

float CloudGrid::point_height(const hvt hashval) {
	return log((float)hashval/(float)cloud_hashcap*(1-exp(-1))+exp(-1))+1;
}

float CloudGrid::point_radius(const int x, const int y) {
	return point_radius(pointhash(x, y, 50));
}

float CloudGrid::point_radius(const hvt hashval) {
	return max_radius*(float)hashval/(float)((hvt)-1);
}

float CloudGrid::height_finalize_fn(const int x, const int y, const float height) {
	return 1.0-height;
}

float CloudGrid::height_add_fn(const float h1, const float h2) {
	return h1 * h2;
}

float CloudGrid::distance_fn(const float base_height, const float dist, const float rad) {
	return 1-(1-base_height)*pow((rad-dist)/rad, 2);
}

void CloudGrid::set_point(const int x, const int y, const float newval) {
	if (x >= cloud_size[0] && x < cloud_size[2] &&
		y >= cloud_size[1] && y < cloud_size[3]) {
		cloud[x-cloud_size[0]][y-cloud_size[1]] = newval;
	}
}

float CloudGrid::get_point(const int x, const int y) {
	if (x >= cloud_size[0] && x < cloud_size[2] &&
		y >= cloud_size[1] && y < cloud_size[3]) {
		return cloud[x-cloud_size[0]][y-cloud_size[1]];
	}
	return 0.0;
}

float CloudGrid::get_height(const float x, const float y) {
	if (x >= cloud_size[0] && x+1 < cloud_size[2] &&
		y >= cloud_size[1] && y+1 < cloud_size[3]) {
		float xscale = ceil(x)-x; //Weighting of floor(x) over ceiling(x)
		float yscale = ceil(y)-y; //Weighting of floor(y) over ceiling(y)
		return yscale*(xscale*get_point(floor(x), floor(y)) + (1-xscale)*get_point(ceil(x), floor(y)))+
		(1-yscale)*(xscale*get_point(floor(x), ceil(y)) + (1-xscale)*get_point(ceil(x), ceil(y)));
	}
	return 0.0;
}

array<float, 3> CloudGrid::get_tangent(const float x0, const float y0, const float x1, const float y1, const array<float, 2>& scale) {
	return array<float, 3>({{(x1-x0)*scale[0], (y1-y0)*scale[0], (get_height(x1, y1)-get_height(x0, y0))*scale[1]}});
}

array<float, 3> CloudGrid::get_normal(const int x, const int y, const array<float, 2>& scale) {
	array<float, 3> summed_norm = {{0, 0, 0}};
	array<array<float, 3>, 4> tan_vectors;
	tan_vectors[0] = get_tangent(x, y, x+1, y, scale);
	tan_vectors[1] = get_tangent(x, y, x, y+1, scale);
	tan_vectors[2] = get_tangent(x, y, x-1, y, scale);
	tan_vectors[3] = get_tangent(x, y, x, y-1, scale);
	for (unsigned int i = 0; i < 4; i++) {
		array<float, 3> new_norm = cloud_norm_vec(cloud_cross_prod(tan_vectors[i], tan_vectors[(i+1)%4]));
		for (unsigned int j = 0; j < 3; j++) {
			summed_norm[j] += new_norm[j];
		}
	}
	return cloud_norm_vec(summed_norm);
}

void CloudGrid::set_size(const array<int, 4> new_size) {
	resize_cloud(new_size);
}

array<int, 4> CloudGrid::get_size() {
	return cloud_size;
}

void CloudGrid::resize_cloud(const array<int, 4> newsize) {
	unsigned int t;
	t = clock();
	if (newsize[1] < cloud_size[1]) {
		for (vector<vector<float>>::iterator extender = cloud.begin(); extender != cloud.end(); ++extender) {
			extender->insert(extender->begin(), cloud_size[1]-newsize[1], 1.0);
		}
	}
	if (newsize[3] > cloud_size[3]) {
		for (vector<vector<float>>::iterator extender = cloud.begin(); extender != cloud.end(); ++extender) {
			extender->insert(extender->end(), newsize[3]-cloud_size[3], 1.0);
		}
	}
	if (newsize[0] < cloud_size[0]) {
		vector<float> newcol(newsize[3]-newsize[1], 1.0);
		cloud.insert(cloud.begin(), cloud_size[0]-newsize[0], newcol);
	std::cout << cloud.size() << std::endl;;
	}
	if (newsize[2] > cloud_size[2]) {
		vector<float> newcol(newsize[3]-newsize[1], 1.0);
		cloud.insert(cloud.end(), newsize[2]-cloud_size[2], newcol);
	std::cout << cloud.size() << std::endl;;
	}
	t = clock() - t;
	std::cout << "Resizing time: " << ((float)t)/CLOCKS_PER_SEC << std::endl;

	array<int, 4> preserve_area = cloud_size;
	cloud_size = newsize;

	t = clock();
	generate_cloud(preserve_area);
	t = clock() - t;
	std::cout << "Regenerating time: " << ((float)t)/CLOCKS_PER_SEC << std::endl;
}

void CloudGrid::generate_cloud() {
	generate_cloud({{0, 0, 0, 0}});
}

void CloudGrid::generate_cloud(const array<int, 4> preserve_area) {
	/*
	Should add clouds for each point that could add to something outside the preserve_area.
	This means an x and y range from cloudmin-overscan to preservemin+overscan and
	preservemax-overscan to cloudmax+overscan

	These overlap awkwardly, especially since preservemin+overscan and preservemax-overscan can overlap
	*/

	if (preserve_area[1]+over_scan >= preserve_area[3]-over_scan) {
		//The y areas overlap (or at least bump right up against eachother) so this can just
		//iterate over all x and y values.
		for (int x = cloud_size[0]-over_scan; x < cloud_size[2]+over_scan; x++) {
			for (int y = cloud_size[1]-over_scan; y < cloud_size[3]+over_scan; y++) {
				add_cloud(x, y, preserve_area);
			}
		}
		for (int x = cloud_size[0]-over_scan; x < cloud_size[2]+over_scan; x++) {
			for (int y = cloud_size[1]-over_scan; y < cloud_size[3]+over_scan; y++) {
				set_point(x, y, height_finalize_fn(x, y, get_point(x, y)));
			}
		}
	} else {
		//There must be a gap in the y range, so iterate over x and then iterate over y, checking
		//to see if there is a gap that needs to be jumped based on the x range.
		for (int x = cloud_size[0]-over_scan; x < cloud_size[2]+over_scan; x++) {
			if (x <= preserve_area[0]+over_scan || x >= preserve_area[2]-over_scan) {
				//No gap in the y range, because everything will be in the x range
				for (int y = cloud_size[1]-over_scan; y < cloud_size[3]+over_scan; y++) {
					add_cloud(x, y, preserve_area);
				}
			} else {
				for (int y = cloud_size[1]-over_scan; y < preserve_area[1]+over_scan; y++) {
					add_cloud(x, y, preserve_area);
				}
				for (int y = preserve_area[3]-over_scan; y < cloud_size[3]+over_scan; y++) {
					add_cloud(x, y, preserve_area);
				}
			}
		}
		//Now do the finialization function
		for (int x = cloud_size[0]-over_scan; x < cloud_size[2]+over_scan; x++) {
			if (x <= preserve_area[0]+over_scan || x >= preserve_area[2]-over_scan) {
				//No gap in the y range, because everything will be in the x range
				for (int y = cloud_size[1]-over_scan; y < cloud_size[3]+over_scan; y++) {
					set_point(x, y, height_finalize_fn(x, y, get_point(x, y)));
				}
			} else {
				for (int y = cloud_size[1]-over_scan; y < preserve_area[1]+over_scan; y++) {
					set_point(x, y, height_finalize_fn(x, y, get_point(x, y)));
				}
				for (int y = preserve_area[3]-over_scan; y < cloud_size[3]+over_scan; y++) {
					set_point(x, y, height_finalize_fn(x, y, get_point(x, y)));
				}
			}
		}
	}
}

void CloudGrid::add_cloud(const int x, const int y, const array<int, 4> preserve_area) {
	hvt hashval = pointhash(x, y);
	if (check_point(hashval)) {
	float pheight = point_height(hashval);
	float pradius = point_radius(x, y);
	int rad = (int)(pradius+1);
	// std::cout << "rad: " << rad << std::endl;
	for (int px = x - rad; px <= x + rad; px++) {
		for (int py = y - rad; py <= y + rad; py++) {
			float dist = pow(pow(x-px,2)+pow(y-py,2),.5);
			if (dist < pradius) {
				set_point(px, py, height_add_fn(get_point(px, py), distance_fn(pheight, dist, pradius)));
			}
		}
	}
	}
}

void CloudGrid::write_cloud(const string filename) {
	std::ofstream ofile;
	ofile.open(filename);
	for (vector<vector<float>>::iterator col = cloud.begin(); col != cloud.end(); ++col) {
		for (vector<float>::iterator val = col->begin(); val != col->end(); ++val) {
			ofile << *val << '|';
		}
		ofile << std::endl;
	}
	ofile.close();
}

// void CloudGrid::compute_cloud(int minx, int maxx, int miny, int maxy) {

// }

