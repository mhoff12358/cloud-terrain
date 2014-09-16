#include "terrain_writer.h"

std::ostream& operator<<(std::ostream& out, TerrainPoint& tpoint) {
	out << tpoint.height;
	return out;
}

std::istream& operator>>(std::istream& in, TerrainPoint& tpoint) {
	in >> tpoint.height;
	return in;
}