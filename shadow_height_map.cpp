#include "shadow_height_map.h"

std::array<float, 3> ScaleLightDependancy(const std::array<float, 3>& light_dir) {
	// The goal is to scale light_dir so that it lies on the 2x2 square
	// centered on the origin (scaling the z-value, but projecting onto the x-y
	// plane). This means the larger of x, y should be scaled to one, with the
	// other values similarly scaled.

	float scale_factor = 1.0f/std::max(std::abs(light_dir[0]), std::abs(light_dir[1]));

	return {{light_dir[0]*scale_factor, light_dir[1]*scale_factor, light_dir[2]*scale_factor}};
}

float CastShadowHeight(const std::array<int, 2>& point_coords, const std::array<float, 3>& light_dir) {
	std::array<float, 3> dependancy_offset = ScaleLightDependancy(light_dir);
	// dependancy_offset[0],[1] point to the interpolated location that is
	// casting a shadow onto our given point. The height of the shadow at the
	// point is then dependancy_offset[2] lower than the interpolated point.

	if (math::abs(dependancy_offset[0]) >= math::abs(dependancy_offset[1])) {
		// This means dependancy_offset[0] should be the +/-1 value
		int xoffset = (dependancy_offset[0] > 0) : 1 ? -1;
		// Now find the approximated value for (x+xoffset, y+dependancy_offset[1])
		int yoffset = (dependancy_offset[1] > 0) : 1 ? -1;
		lerp(FINDSHADOWHEIGHT(point_coords[0]+xoffset, point_coords[1]),
			 FINDSHADOWHEIGHT(point_coords[0]+xoffset, point_coords[1]+yoffset),
			 math::abs(yoffset));
	} else {
		// This means dependancy_offset[0] should be the +/-1 value
		int yoffset = (dependancy_offset[1] > 0) : 1 ? -1;
		// Now find the approximated value for (x+xoffset, y+dependancy_offset[1])
		int xoffset = (dependancy_offset[0] > 0) : 1 ? -1;
		lerp(FINDSHADOWHEIGHT({{point_coords[0], point_coords[1]+yoffset}}),
			 FINDSHADOWHEIGHT({{point_coords[0]+xoffset, point_coords[1]+yoffset}}),
			 math::abs(xoffset));
	}
}

float UpdateShadowHeight(const std::array<int, 2>& point_coords, const std::array<float, 3>& light_dir) {
	float cast_height = CastShadowHeight(point_coords, light_dir);
	float terrain_height = GetTerrainHeight(point_coords);
	SetShadowHeight(point_coords, std::max(cast_height, terrain_height));
}