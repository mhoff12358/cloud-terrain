#include <array>
#include <utility>
#include <map>
#include <iostream>
#include <fstream>

#include "terrain_point.h"

#ifndef __TERRAIN_WRITER_H_INCLUDED__
#define __TERRAIN_WRITER_H_INCLUDED__

class TerrainWriter {
public:
	void writePointToFile(std::fstream& fs, const TerrainPoint& point);
	void readPointFromFile(std::fstream& fs, TerrainPoint& point);

	void writeIndexToFile(std::fstream& fs, const std::array<int, 2>& index);
	void readIndexFromFile(std::fstream& fs, std::array<int, 2>& index);

	void writeAllPointsToFile(std::fstream& fs, const std::map<std::array<int, 2>, TerrainPoint>& points);
	void readAllPointsFromFile(std::fstream& fs, std::map<std::array<int, 2>, TerrainPoint>& loaded_map);

private:
	bool logging = true;	
};

#endif