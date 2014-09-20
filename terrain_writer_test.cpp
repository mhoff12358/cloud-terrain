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
	tm.getPoint(std::array<int, 2>({{1, 2}}));
	tm.getPoint(std::array<int, 2>({{2, 2}}));
	tm.getPoint(std::array<int, 2>({{3, 2}}));
	tm.getPoint(std::array<int, 2>({{4, 2}}));
	tm.getPoint(std::array<int, 2>({{5, 2}}));

	tw.prepareFileForWrite("terrain.txt");
	tm.savePoints();
	tw.releaseFile();

	tw.prepareFileForRead("terrain.txt");
	tm.loadPoints();
	// int i1, i2;
	// float f;
	// std::fstream fs;
	// std::cout << sizeof(int) << " " << sizeof(float) << std::endl;
	// fs.open("terrain.txt", std::fstream::in | std::fstream::binary);
	// for (int i  = 0; i < 5; i++) {
	// 	fs.read((char*)&i1, sizeof(int));
	// 	fs.read((char*)&i2, sizeof(int));
	// 	fs.read((char*)&f, sizeof(float));
	// 	std::cout << i1 << " " << i2 << " " << f << std::endl;
	// }
}