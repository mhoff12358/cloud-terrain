#include <map>
#include <array>

#include "terrain_writer.h"
#include "terrain_point.h"
#include "terrain_generator.h"

#ifndef __TERRAIN_MAP_H_INCLUDED__
#define __TERRAIN_MAP_H_INCLUDED__

class TerrainMap {
public:
	TerrainMap(TerrainGenerator* tg, TerrainWriter* tw) : generator(tg), writer(tw) {};

	TerrainPoint& getPoint(std::array<int, 2> point_index);

private:
	TerrainGenerator* generator;
	TerrainWriter* writer;

	std::map<std::array<int, 2>, TerrainPoint> point_map;
};

#endif
