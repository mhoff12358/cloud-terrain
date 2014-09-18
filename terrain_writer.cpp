#include "terrain_writer.h"

void TerrainWriter::writePointToFile(std::fstream* fs, const TerrainPoint& point) {
	fs->write((char *)&point.height, sizeof(point.height));
	fs->write((char *)&point.color, sizeof(point.color));
}

void TerrainWriter::readTerrainFromFile(std::fstream* fs, TerrainPoint& point) {
	fs->read((char *)&point.height, sizeof(point.height));
	fs->read((char *)&point.color, sizeof(point.color));
}

void TerrainWriter::writeIndexToFile(std::fstream* fs, const std::array<int, 2>& index) {
	fs->write((char *)index.data(), sizeof(int)*2);
}

void TerrainWriter::readIndexFromFile(std::fstream* fs, std::array<int, 2>& index) {
	fs->read((char *)index.data(), sizeof(int)*2);
}

void TerrainWriter::writeAllPointsToFile(std::fstream* fs, const std::map<std::array<int, 2>, TerrainPoint>& points) {
	for (const std::pair<std::array<int, 2>, TerrainPoint>& terrain_point : points) {
		if (logging) {
			std::cout << "Writing " << terrain_point.first[0] << ", " << terrain_point.first[1] <<
			": " << terrain_point.second.height;
		}
		writeIndexToFile(fs, terrain_point.first);
		writePointToFile(fs, terrain_point.second);
	}
}

void TerrainWrtier::readAllPointsFromFile(std::fstream* fs, std::std::map<std::array<int, 2>, TerrainPoint>& points) {
	
}

void TerrainWrtier::readPointsFromFile(std::fstream* fs, std::std::map<std::array<int, 2>, TerrainPoint>& points, int num_points) {
	for (int i = 0; i < num_points; i++) {
		std::array<int, 2> new_index;
		TerrainPoint new_terrain;
		readIndexFromFile(fs, new_index);
		readTerrainFromFile(fs, new_terrain);
		if (logging) {
			std::cout << "Loading " << new_index[0] << ", " << new_index[1] <<
			": " << new_terrain.height;
		}
		points[new_index] = new_terrain;
	}
}
