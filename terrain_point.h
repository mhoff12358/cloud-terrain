
#include <array>
#include <cmath>
#include <climits>

#ifndef __TERRAIN_POINT_H_INCLUDED__
#define __TERRAIN_POINT_H_INCLUDED__

class TerrainPoint {
public:
	float height;
	std::array<float, 3> color;

	TerrainPoint(float sheight, std::array<float, 3> scolor) : height(sheight), color(scolor) {};
	TerrainPoint() : height(0.0f), color({{0.0f, 0.0f, 0.0f}}) {};
};


TerrainPoint linearInterpolateTerrainPoint(TerrainPoint& a, TerrainPoint& b, float x);

TerrainPoint squareInterpolateTerrainPoint(TerrainPoint& aa, TerrainPoint& ba, TerrainPoint& ab, TerrainPoint& bb, float x, float y);

#endif