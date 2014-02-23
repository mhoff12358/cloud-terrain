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

class CloudGrid {
private:
	std::string seed;

	float max_hash;
	size_t cloud_hashcap;
	float max_radius;
	int over_scan;

	array<int, 4> cloud_size = {{0, 0, 0, 0}}; //Size is minx, miny, maxx, maxy
	vector<vector<float>> cloud;

public:
	CloudGrid(const string, const float, const float);
	CloudGrid(const float, const float);

	hvt pointhash(const int32_t x, const int32_t y, const uint64_t mod);
	hvt pointhash(const int32_t x, const int32_t y);
	bool check_point(const int x, const int y);
	bool check_point(const hvt hashval);
	float point_height(const int x, const int y);
	float point_height(const hvt hashval);
	float point_radius(const int x, const int y);
	float point_radius(const hvt hashval);

	float height_finalize_fn(const int, const int, const float);
	float height_add_fn(const float, const float);
	float distance_fn(const float, const float, const float);

	void write_cloud(const string);
	void resize_cloud(const array<int, 4>);
	void generate_cloud();
	void generate_cloud(const array<int, 4>);
	void add_cloud(const int, const int, const array<int, 4>);

	float get_point(const int, const int);
	void set_point(const int, const int, const float);
	array<int, 4> get_size();
	void set_size(const array<int, 4>);
	// void compute_cloud();
};
