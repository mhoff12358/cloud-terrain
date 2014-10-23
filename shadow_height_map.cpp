#include "shadow_height_map.h"

std::array<float, 3> ShadowHeightMap::ScaleLightDependancy(const std::array<float, 3>& light_dir) {
	// The goal is to scale light_dir so that it lies on the 2x2 square
	// centered on the origin (scaling the z-value, but projecting onto the x-y
	// plane). This means the larger of x, y should be scaled to one, with the
	// other values similarly scaled.

	float scale_factor = 1.0f/std::max(std::abs(light_dir[0]), std::abs(light_dir[1]));

	return {{light_dir[0]*scale_factor, light_dir[1]*scale_factor, light_dir[2]*scale_factor}};
}

float ShadowHeightMap::CastShadowHeight(const std::array<int, 2>& point_coords, const std::array<float, 3>& dependancy_offset) {
	// dependancy_offset[0],[1] point to the interpolated location that is
	// casting a shadow onto our given point. The height of the shadow at the
	// point is then dependancy_offset[2] lower than the interpolated point.

	if (abs(dependancy_offset[0]) >= abs(dependancy_offset[1])) {
		// This means dependancy_offset[0] should be the +/-1 value
		int xoffset = (dependancy_offset[0] > 0) ? 1 : -1;
		// Now find the approximated value for (x+xoffset, y+dependancy_offset[1])
		int yoffset = (dependancy_offset[1] > 0) ? 1 : -1;
		return lerp(GetShadowHeight(std::array<int,2>({{point_coords[0]+xoffset, point_coords[1]}})),
			        GetShadowHeight(std::array<int,2>({{point_coords[0]+xoffset, point_coords[1]+yoffset}})),
			        abs(yoffset));
	} else {
		// This means dependancy_offset[0] should be the +/-1 value
		int yoffset = (dependancy_offset[1] > 0) ? 1 : -1;
		// Now find the approximated value for (x+xoffset, y+dependancy_offset[1])
		int xoffset = (dependancy_offset[0] > 0) ? 1 : -1;
		return lerp(GetShadowHeight({{point_coords[0], point_coords[1]+yoffset}}),
			        GetShadowHeight({{point_coords[0]+xoffset, point_coords[1]+yoffset}}),
			        abs(xoffset));
	}
}

void ShadowHeightMap::UpdateAllShadowHeights(TerrainMap& height_map, const std::array<float, 3>& light_dir) {
	// First the light dependancy must be scaled to be a grid dependancy
	std::array<float, 3> dependancy_offset = ScaleLightDependancy(light_dir);

	// Updates the shadow height for the initial boundaries of the grid.
	// precompute_value is the x and y values that need to be assumed to be lit
	// in order to fill out our dependancies.
	// iterator_dir is the direction that the grid will be iterated over to
	// ensure the dependencies are always fulfilled.
	std::array<int, 2> precompute_value;
	std::array<int, 2> iterator_dir;
	for (int i = 0; i < 2; i++) {
		precompute_value[i] = map_coords_minimum[i];
		iterator_dir[i] = 1;
		if (dependancy_offset[i] >= 0) {
			precompute_value[i] += map_coords_size[i] - 1;
			iterator_dir[i] = -1;
		}
	}

	// Iterate over all valid x values updating at y's precompute value
	for (int update_x = map_coords_minimum[0]; update_x < map_coords_minimum[0]+map_coords_size[0]; update_x++) {
		UpdateShadowHeightAssumeLit(height_map, {{update_x, precompute_value[1]}});
	}
	// Iterate over all valid y values updating at x's precompute value
	for (int update_y = map_coords_minimum[1]; update_y < map_coords_minimum[1]+map_coords_size[1]; update_y++) {
		UpdateShadowHeightAssumeLit(height_map, {{precompute_value[0], update_y}});
	}

	// Now fill in the rest of the graph in a way that fulfils the dependencies
	for (int update_x = precompute_value[0]+iterator_dir[0];
		 update_x >= map_coords_minimum[0] && update_x < map_coords_minimum[0]+map_coords_size[0];
		 update_x += iterator_dir[0]) {
		for (int update_y = precompute_value[1]+iterator_dir[1];
			 update_y >= map_coords_minimum[1] && update_y < map_coords_minimum[1]+map_coords_size[1];
			 update_y += iterator_dir[1]) {
			UpdateShadowHeight(height_map, {{update_x, update_y}}, dependancy_offset);
		}
	}
}

void ShadowHeightMap::UpdateShadowHeight(TerrainMap& height_map, const std::array<int, 2>& point_coords, const std::array<float, 3>& dependancy_offset) {
	float cast_height = CastShadowHeight(point_coords, dependancy_offset);
	float terrain_height = height_map.getPoint(point_coords).height;
	SetShadowHeight(point_coords, std::max(cast_height, terrain_height));
}

void ShadowHeightMap::UpdateShadowHeightAssumeLit(TerrainMap& height_map, const std::array<int, 2>& point_coords) {
	SetShadowHeight(point_coords, height_map.getPoint(point_coords).height);
}

void ShadowHeightMap::SetShadowHeight(const std::array<int, 2>& point_coords, float new_height) {
	shadow_height_map[point_coords] = new_height;
}

float ShadowHeightMap::GetShadowHeight(const std::array<int, 2>& point_coords) {
	auto found_point = shadow_height_map.find(point_coords);
	if (found_point == shadow_height_map.end()) {
		std::cout << "Attempting to lookup a shadow height that doesn't exist" << std::endl;
		return 0.0f;
	}
	return found_point->second;
}