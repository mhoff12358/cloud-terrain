#include "cloud.h"
#include <time.h>

// using namespace std;

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
