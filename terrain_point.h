
#ifndef __TERRAIN_POINT_H_INCLUDED__
#define __TERRAIN_POINT_H_INCLUDED__

class TerrainPoint {
public:
	float height;
	std::array<float, 3> color;

	TerrainPoint(float sheight, std::array<float, 3> scolor) : height(sheight), color(scolor) {};
	TerrainPoint() : height(0.0f), color({{0.0f, 0.0f, 0.0f}}) {};

	static int write_size;
};

int TerrainPoint::write_size = sizeof(float)+sizeof(float)*3;

#endif