
#include <array>
#include <cmath>
#include <iostream>
#include <map>

#include "my_math.h"
#include "terrain_map.h"


class ShadowHeightMap {
public:
	ShadowHeightMap(const std::array<int, 2>& mcm, const std::array<int, 2>& mcs) : map_coords_minimum(mcm), map_coords_size(mcs) {};

	float GetShadowHeight(const std::array<int, 2>& point_coords);
	void SetShadowHeight(const std::array<int, 2>& point_coords, float new_height);

private:
	// The coordinates representing the minimum x, y values for the shadow height map
	std::array<int, 2> map_coords_minimum;
	// The width and height of the coordinates for the shadow height map
	std::array<int, 2> map_coords_size;
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
	float CastShadowHeight(const std::array<int, 2>& point_coords, const std::array<float, 3>& dependancy_offset);

	// Sets the shadow height at point_coords based on light shining from the given
	// direction. This is dependant on the shadow height already being correct for
	// the point at the given dependancy offset.
	void UpdateShadowHeight(TerrainMap& height_map, const std::array<int, 2>& point_coords, const std::array<float, 3>& dependancy_offset);

	// Update the shadow height map for the given point coordinates assuming that
	// it is lit.
	void UpdateShadowHeightAssumeLit(TerrainMap& height_map, const std::array<int, 2>& point_coords);

	// Updates all shadow heights in the grid based on the given height map and
	// lighting.
	void UpdateAllShadowHeights(TerrainMap& height_map, const std::array<float, 3>& light_dir);
};