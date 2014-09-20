#include "terrain_map.h"

/*
Tries to pull in the terrain data for the given index from memory if possible,
falling back to requesting a new one if necessary. 
*/
TerrainPoint& TerrainMap::getPoint(std::array<int, 2> point_index) {
	auto found_point = point_map.find(point_index);
	if (found_point == point_map.end()) {
		found_point = point_map.emplace(point_index, generator->createPoint(point_index)).first;
	}
	return found_point->second;
}

void TerrainMap::savePoints() {
	writer->writeAllPointsToFile(point_map);
}

void TerrainMap::loadPoints() {
	writer->readAllPointsFromFile(point_map);
}