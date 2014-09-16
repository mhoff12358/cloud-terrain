#include <iostream>
#include <sstream>
#include "terrain_writer.h"

int main() {
	TerrainPoint point(4.0f, {{14.42, 51345.542, 54326.53}});
	TerrainPoint point2(8.0f, {{14.42, 51345.542, 54326.53}});
	std::stringstream s;
	s << point;
	s >> point2;
	std::cout << point2 << std::endl;
}