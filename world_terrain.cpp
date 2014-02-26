#include "world_terrain.h"
#include "main.h"

void sphereVertex(float, float, float);

WorldTerrain::WorldTerrain(Game& g) : game(g) {
}

void WorldTerrain::initialize() {
	cg.resize_cloud({{-100, -100, 100, 100}});
	cg.write_cloud("terrain.txt");

	generate_stars(50);
}

void WorldTerrain::draw_terrain() {
	glPushMatrix();
	glScalef(terrain_scale[0], terrain_scale[0], terrain_scale[1]);
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
	glPopMatrix();
}

void WorldTerrain::draw_skypbox() {
	glUseProgram(game.get_state().get_skybox_prog());

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);

	//Sky!
	glColor3f(100./255, 149./255, 237./255);
	glColor3f(0, 0, 0);
	glBegin(GL_QUADS); //Front
		glVertex3f( 0.5f, -0.5f, -0.5f);
		glVertex3f(-0.5f, -0.5f, -0.5f);
		glVertex3f(-0.5f,  0.5f, -0.5f);
		glVertex3f( 0.5f,  0.5f, -0.5f);
	glEnd();
	glBegin(GL_QUADS); //Left
		glVertex3f( 0.5f, -0.5f,  0.5f);
		glVertex3f( 0.5f, -0.5f, -0.5f);
		glVertex3f( 0.5f,  0.5f, -0.5f);
		glVertex3f( 0.5f,  0.5f,  0.5f);
	glEnd();
	glBegin(GL_QUADS); //Back
		glVertex3f(-0.5f, -0.5f,  0.5f);
		glVertex3f( 0.5f, -0.5f,  0.5f);
		glVertex3f( 0.5f,  0.5f,  0.5f);
		glVertex3f(-0.5f,  0.5f,  0.5f);
	glEnd();
	glBegin(GL_QUADS); //Right
		glVertex3f(-0.5f, -0.5f, -0.5f);
		glVertex3f(-0.5f, -0.5f,  0.5f);
		glVertex3f(-0.5f,  0.5f,  0.5f);
		glVertex3f(-0.5f,  0.5f, -0.5f);
	glEnd();
	glBegin(GL_QUADS); //Top
		glVertex3f(-0.5f,  0.5f, -0.5f);
		glVertex3f(-0.5f,  0.5f,  0.5f);
		glVertex3f( 0.5f,  0.5f,  0.5f);
		glVertex3f( 0.5f,  0.5f, -0.5f);
	glEnd();
	glBegin(GL_QUADS); //Bottom
		glVertex3f(-0.5f, -0.5f, -0.5f);
		glVertex3f(-0.5f, -0.5f,  0.5f);
		glVertex3f( 0.5f, -0.5f,  0.5f);
		glVertex3f( 0.5f, -0.5f, -0.5f);
	glEnd();

	//Stars!
	float w = 1.0, h = 1.0;
	glBindTexture(GL_TEXTURE_2D, game.get_state().grumptex);
	for (vector<array<float, 2>>::const_iterator s = stars.cbegin(); s != stars.cend(); s++) {
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBegin(GL_QUADS);
		glColor4f(1, 1, 1, 1);
		glTexCoord2f(0, 0);  sphereVertex((*s)[0]-(star_size/sin((*s)[1]-star_size)), (*s)[1]-star_size, 1.0);
		glTexCoord2f(w, 0);  sphereVertex((*s)[0]+(star_size/sin((*s)[1]-star_size)), (*s)[1]-star_size, 1.0);
		glTexCoord2f(w, h);  sphereVertex((*s)[0]+(star_size/sin((*s)[1]+star_size)), (*s)[1]+star_size, 1.0);
		glTexCoord2f(0, h);  sphereVertex((*s)[0]-(star_size/sin((*s)[1]+star_size)), (*s)[1]+star_size, 1.0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}

	glPopAttrib();
}

void sphereVertex(float theta, float phi, float radius) {
	float x = cos(theta)*sin(phi)*radius;
	float z = sin(theta)*sin(phi)*radius;
	float y = cos(phi)*radius;
	glVertex3f(x, y, z);
}

void WorldTerrain::generate_stars(unsigned int num_stars) {
	for (unsigned int star_no = 0; star_no < num_stars; star_no++) {
		float theta = (float)(rand())/(float)(RAND_MAX/(2*M_PI));
		float phi = ((float)M_PI/2.0)+asin(((float)(rand())/(float)(RAND_MAX/2))-1);

		stars.emplace_back(array<float, 2>({{theta, phi}}));
	}
}

float WorldTerrain::get_height(const float x, const float y) {
	return cg.get_height(x/terrain_scale[0], y/terrain_scale[0])*terrain_scale[1];
}