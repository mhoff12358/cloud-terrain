#include <array>
#include <iostream>

class TerrainPoint {
public:
	float height;
	std::array<float, 3> color;

	TerrainPoint(float sheight, std::array<float, 3> scolor) : height(sheight), color(scolor) {};

	friend std::ostream& operator<<(std::ostream& out, TerrainPoint& tpoint);
	friend std::istream& operator>>(std::istream& in, TerrainPoint& tpoint);

};

