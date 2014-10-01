
#include <array>
#include <cmath>

#include "my_math.h"


class ShadowHeightMap {
public:
	ShadowHeightMap() {};

	GetShadowHeight(const std::array<int, 2>& point_coords);

private:
	std::map<std::array<int, 2>, float> shadow_height_map;

	// Given a vector to the light source, scale it to proivde a dependancy offset.
	// This offset points from a given coordinate point to a point on the grid that
	// can be used to estimate the shadow height for the coordinate point.
	// This is important because any location within the one unit square around a
	// point is interpolated using that point, so the value used should be exactly
	// on that square.
	std::array<float, 3> ScaleLightDependancy(const std::array<float, 3>& light_dir);

	// Find the shadow height cast onto the given point by light from the given
	// direction
	float CastShadowHeight(const std::array<int, 2>& point_coords, const std::array<float, 3>& light_dir);
};