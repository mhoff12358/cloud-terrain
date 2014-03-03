#include "world_terrain.h"
#include "main.h"

void sphereVertex(float, float, float);

WorldTerrain::WorldTerrain(Game& g) : game(g) {
}

void WorldTerrain::initialize() {
	cg.resize_cloud({{-100, -100, 100, 100}});
	cg.write_cloud("terrain.txt");

	// create_ground_vbo();

	generate_stars(50);
}

void WorldTerrain::add_cloud_vertex(unsigned int x, unsigned int y, float * vertex_loc) {
	vertex_loc[0] = (float)x;
	vertex_loc[1] = (float)y;
	vertex_loc[2] = -1.0;
	// vertex_loc[2] = cg.get_point(x, y);
}

void WorldTerrain::create_ground_vbo() {
	glGenBuffers(1, &ground_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, ground_vbo);

	array<int, 4> cloud_grid_size = cg.get_size();
	unsigned int x_max = (cloud_grid_size[2]-cloud_grid_size[0]-1);
	unsigned int y_max = (cloud_grid_size[3]-cloud_grid_size[1]-1);
	unsigned int cloud_data_square_size = 3*3*2; //3 floats per vertex, 3 vertices per triangle, 2 triangles per square
	unsigned int number_squares = x_max*y_max;
	ground_vbo_size = cloud_data_square_size * number_squares;
	float * cloud_data_buffer = new float[cloud_data_square_size*number_squares];
	float * cdb_iter = cloud_data_buffer;
	for (unsigned int y = 0; y < y_max; y++) {
		for (unsigned int x = 0; x < x_max; x++) {
			add_cloud_vertex(x  , y  , cdb_iter+3*0);
			add_cloud_vertex(x  , y+1, cdb_iter+3*1);
			add_cloud_vertex(x+1, y  , cdb_iter+3*2);
			add_cloud_vertex(x+1, y  , cdb_iter+3*3);
			add_cloud_vertex(x  , y+1, cdb_iter+3*4);
			add_cloud_vertex(x+1, y+1, cdb_iter+3*5);
			cdb_iter += cloud_data_square_size;
		}
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*cloud_data_square_size*number_squares, cloud_data_buffer, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// glVertexAttribPointer(game.get_state().ground_shad.shader_attributes[0].first, 3, GL_FLOAT, GL_FALSE, 0, 0);

	delete[] cloud_data_buffer;

}

void WorldTerrain::draw_terrain() {
	glUseProgram(game.get_state().get_ground_prog());
	GLfloat mvmat[16], promat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mvmat);
	glGetFloatv(GL_PROJECTION_MATRIX, promat);
	glUniformMatrix4fv(glGetAttribLocation(game.get_state().get_ground_prog(), "view_matrix"), 1, false, mvmat);
	glUniformMatrix4fv(glGetAttribLocation(game.get_state().get_ground_prog(), "proj_matrix"), 1, false, promat);

	glBindBuffer(GL_ARRAY_BUFFER, ground_vbo);
	glDrawArrays(GL_TRIANGLES, 0, ground_vbo_size);

	// glPushMatrix();
	// glScalef(terrain_scale[0], terrain_scale[0], terrain_scale[1]);
	// for (int x = cg.get_size()[0]; x < cg.get_size()[2]; x++) {
	// 	glBegin(GL_TRIANGLE_STRIP);
	// 	for (int y = cg.get_size()[1]; y+1 < cg.get_size()[3]; y++) {
	// 		glColor3f(cg.get_point(x,y), 0.0f, 1-cg.get_point(x,y));
	// 		glVertex3f((float)x, (float)y, cg.get_point(x,y));
	// 		glColor3f(cg.get_point(x+1,y), 0.0f, 1-cg.get_point(x+1,y));
	// 		glVertex3f((float)(x+1), (float)y, cg.get_point(x+1,y));
	// 	}
	// 	glEnd();
	// }
	// glPopMatrix();

}

void WorldTerrain::draw_skypbox() {
	glUseProgram(game.get_state().get_skybox_prog());
	GLint currprog;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currprog);
	// std::cout << currprog << " " << game.get_state().get_skybox_prog() << std::endl;
	glGetProgramiv(game.get_state().get_skybox_prog(), GL_ATTACHED_SHADERS, &currprog);
	// std::cout << "Attached progs: " << currprog << std::endl;

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