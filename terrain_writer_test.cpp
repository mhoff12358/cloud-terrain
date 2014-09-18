#include <iostream>
#include <fstream>
#include <array>
#include "terrain_map.h"
#include "terrain_generator.h"
#include "terrain_writer.h"

int main() {
	TerrainGenerator tg;
	TerrainWriter tw;
	TerrainMap tm(&tg, &tw);
	TerrainPoint& point = tm.getPoint(std::array<int, 2>({{1, 2}}));
	std::cout << point.height << std::endl;

}