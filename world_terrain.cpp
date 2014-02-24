#include "world_terrain.h"

WorldTerrain::WorldTerrain(Game& g) : game(g) {
	cg.resize_cloud({{-100, -100, 100, 100}});
	cg.write_cloud("terrain.txt");
}

void WorldTerrain::draw_terrain() {
	for (int x = cg.get_size()[0]; x < cg.get_size()[2]; x++) {
		glBegin(GL_TRIANGLE_STRIP);
		for (int y = cg.get_size()[1]; y+1 < cg.get_size()[3]; y++) {
			glColor3f(cg.get_point(x,y), 0.0f, 1-cg.get_point(x,y));
			glVertex3f((float)x, (float)y, cg.get_point(x,y));
			glColor3f(cg.get_point(x+1,y), 0.0f, 1-cg.get_point(x+1,y));
			glVertex3f((float)(x+1), (float)y, cg.get_point(x+1,y));
		}
		glEnd();
	}
}