#include "world_terrain.h"
#include "main.h"

void sphereVertex(float, float, float);

WorldTerrain::WorldTerrain(Game& g) : game(g) {
}

void WorldTerrain::initialize() {
	// world_grid.resize_cloud(grid_size);
	// world_grid.write_cloud("terrain.txt");
	// write_grid("terrain.txt");

	generate_stars(50);

	create_ground_vbo();
	create_skybox_vbo();
	create_stars_vbo();
}

void WorldTerrain::add_cloud_vertex(int x, int y, float * vertex_loc, float * color_loc, float * normal_loc) {
	vertex_loc[0] = (float)x*terrain_scale[0];
	vertex_loc[1] = (float)y*terrain_scale[0];
	// vertex_loc[2] = world_grid.get_height(x, y)*terrain_scale[1];
	vertex_loc[2] = world_grid.get_height(x, y)*terrain_scale[1];
	color_loc[0] = 0;
	color_loc[1] = 1.0/5.0;
	color_loc[2] = 1.0/25.0;
	array<float, 3> normal_vector = world_grid.get_normal(x, y, terrain_scale);
	normal_loc[0] = normal_vector[0];
	normal_loc[1] = normal_vector[1];
	normal_loc[2] = normal_vector[2]; //Visually, the ground is scaled vertically by terrain_scale[1]/terrain_scale[0]
}

void WorldTerrain::create_ground_vbo() {
	glGenBuffers(1, &ground_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, ground_vbo);

	array<int, 4> cloud_grid_size = grid_size;
	unsigned int loc_square_size = 3*3*2; //3 floats per vertex, 3 vertices per triangle, 2 triangles per square
	unsigned int col_square_size = 3*3*2; //3 floats per vertex, 3 vertices per triangle, 2 triangles per square
	unsigned int nor_square_size = 3*3*2; //3 floats per vertex, 3 vertices per triangle, 2 triangles per square
	unsigned int number_squares = (cloud_grid_size[2]-cloud_grid_size[0])*(cloud_grid_size[3]-cloud_grid_size[1]);
	loc_vbo_size = loc_square_size * number_squares;
	col_vbo_size = col_square_size * number_squares;
	nor_vbo_size = nor_square_size * number_squares;
	ground_vbo_size = loc_vbo_size + col_vbo_size + nor_vbo_size;
	float * cloud_data_buffer = new float[ground_vbo_size];
	float * vert_iter = cloud_data_buffer;
	float * color_iter = cloud_data_buffer+(loc_vbo_size);
	float * norm_iter = cloud_data_buffer+(loc_vbo_size+col_vbo_size);
	for (int y = cloud_grid_size[1]; y < cloud_grid_size[3]-1; y++) {
		for (int x = cloud_grid_size[0]; x < cloud_grid_size[2]-1; x++) {
			add_cloud_vertex(x  , y  , vert_iter+3*0, color_iter+3*0, norm_iter+3*0);
			add_cloud_vertex(x  , y+1, vert_iter+3*1, color_iter+3*1, norm_iter+3*1);
			add_cloud_vertex(x+1, y  , vert_iter+3*2, color_iter+3*2, norm_iter+3*2);
			add_cloud_vertex(x+1, y  , vert_iter+3*3, color_iter+3*3, norm_iter+3*3);
			add_cloud_vertex(x  , y+1, vert_iter+3*4, color_iter+3*4, norm_iter+3*4);
			add_cloud_vertex(x+1, y+1, vert_iter+3*5, color_iter+3*5, norm_iter+3*5);
			vert_iter += loc_square_size;
			color_iter += col_square_size;
			norm_iter += nor_square_size;
		}
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*ground_vbo_size, cloud_data_buffer, GL_STATIC_DRAW);
	std::cout << "Number of bytes in the buffer: " << sizeof(float)*ground_vbo_size << std::endl;

	std::cout << "GL ERROR CREATE GROUND VBO: " << glGetError() << std::endl;
	delete[] cloud_data_buffer;

	std::cout << "Number of vertices being passed to draw arrays: " << ground_vbo_size << std::endl;
}

void WorldTerrain::create_skybox_vbo() {
	glGenBuffers(1, &skybox_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, skybox_vbo);

	float vertex_vals[skybox_num_tri*3] =
	{
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
	};
	float skybox_data_buffer[skybox_num_tri*6]; //6 sides, 4 vertices, 3 floats for position and 3 floats for color
	memcpy(skybox_data_buffer, vertex_vals, skybox_num_tri*3*sizeof(float));

	//Set all vertex colors to CORNFLOWER BLUE YEAAAAAAH
	for (float * sdb_ptr = skybox_data_buffer + skybox_num_tri*3; sdb_ptr != skybox_data_buffer + skybox_num_tri*6; sdb_ptr += 3) {
		sdb_ptr[0] = 100.0/255;
		sdb_ptr[1] = 149.0/255;
		sdb_ptr[2] = 237.0/255;
	}

	//Makes each side of the skybox different for testing purposes
	// float * sdb = skybox_data_buffer + skybox_num_tri*3;
	// for (int i = 0; i < 6; i++) {
	// 	sdb[i*3] = 1.0; sdb[i*3+1] = 0.0; sdb[i*3+2] = 0.0;
	// 	sdb[i*3+3*6] = 0.0; sdb[i*3+1+3*6] = 1.0; sdb[i*3+2+3*6] = 0.0;
	// 	sdb[i*3+3*12] = 0.0; sdb[i*3+1+3*12] = 0.0; sdb[i*3+2+3*12] = 1.0;
	// 	sdb[i*3+3*18] = 1.0; sdb[i*3+1+3*18] = 1.0; sdb[i*3+2+3*18] = 0.0;
	// 	sdb[i*3+3*24] = 1.0; sdb[i*3+1+3*24] = 0.0; sdb[i*3+2+3*24] = 1.0;
	// 	sdb[i*3+3*30] = 0.0; sdb[i*3+1+3*30] = 1.0; sdb[i*3+2+3*30] = 1.0;
	// }

	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*skybox_num_tri*6, skybox_data_buffer, GL_STATIC_DRAW);
}

// void sphereVertex(float theta, float phi, float radius) {
// 	float x = cos(theta)*sin(phi)*radius;
// 	float z = sin(theta)*sin(phi)*radius;
// 	float y = cos(phi)*radius;
// 	glVertex3f(x, y, z);
// }

void sphereVertex(float * store, float theta, float phi, float radius) {
	store[0] = cos(theta)*sin(phi)*radius;
	store[2] = sin(theta)*sin(phi)*radius;
	store[1] = cos(phi)*radius;
};

void WorldTerrain::create_stars_vbo() {
	glGenBuffers(1, &stars_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, stars_vbo);

	unsigned int num_star_vertices = stars.size(); //each star is a
	//"square" and so needs 2 triangles each with 3 vertices
	float * stars_data_buffer = new float[num_star_vertices*3];

	float * sdb_iter = stars_data_buffer;
	for (vector<array<float, 2>>::const_iterator s = stars.cbegin(); s != stars.cend(); s++) {
		sphereVertex(sdb_iter, (*s)[0], (*s)[1], 1.0);
		sdb_iter += 3;
	// 	sphereVertex(sdb_iter, (*s)[0]-(star_size/sin((*s)[1]-star_size)), (*s)[1]-star_size, 1.0);
	// 	sphereVertex(sdb_iter+3, (*s)[0]+(star_size/sin((*s)[1]-star_size)), (*s)[1]-star_size, 1.0);
	// 	sphereVertex(sdb_iter+6, (*s)[0]-(star_size/sin((*s)[1]+star_size)), (*s)[1]+star_size, 1.0);
	// 	sphereVertex(sdb_iter+9, (*s)[0]-(star_size/sin((*s)[1]+star_size)), (*s)[1]+star_size, 1.0);
	// 	sphereVertex(sdb_iter+12, (*s)[0]+(star_size/sin((*s)[1]-star_size)), (*s)[1]-star_size, 1.0);
	// 	sphereVertex(sdb_iter+15, (*s)[0]+(star_size/sin((*s)[1]+star_size)), (*s)[1]+star_size, 1.0);
	// 	sdb_iter += 18;
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_star_vertices, stars_data_buffer, GL_STATIC_DRAW);

	delete[] stars_data_buffer;
}

void WorldTerrain::draw_terrain() {
	GLuint error;
	glUseProgram(game.get_state().get_ground_prog());

	glPushMatrix();
	// glScalef(terrain_scale[0], terrain_scale[0], terrain_scale[1]);
	GLfloat mvmat[16], promat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mvmat);
	glGetFloatv(GL_PROJECTION_MATRIX, promat);
	glUniformMatrix4fv(glGetUniformLocation(game.get_state().get_ground_prog(), "view_matrix"), 1, false, mvmat);
	glUniformMatrix4fv(glGetUniformLocation(game.get_state().get_ground_prog(), "proj_matrix"), 1, false, promat);
	glUniform3f(glGetUniformLocation(game.get_state().get_ground_prog(), "sun_dir"), sun_dir[0], sun_dir[1], sun_dir[2]);

	glBindBuffer(GL_ARRAY_BUFFER, ground_vbo);
	glEnableVertexAttribArray(game.get_state().ground_shad.shader_attributes[0].first);
	glVertexAttribPointer(game.get_state().ground_shad.shader_attributes[0].first, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(game.get_state().ground_shad.shader_attributes[1].first);
	glVertexAttribPointer(game.get_state().ground_shad.shader_attributes[1].first, 3, GL_FLOAT, GL_FALSE, 0, (char*)NULL+sizeof(float)*loc_vbo_size);
	glEnableVertexAttribArray(game.get_state().ground_shad.shader_attributes[2].first);
	glVertexAttribPointer(game.get_state().ground_shad.shader_attributes[2].first, 3, GL_FLOAT, GL_FALSE, 0, (char*)NULL+sizeof(float)*(loc_vbo_size+col_vbo_size));
	glDrawArrays(GL_TRIANGLES, 0, loc_vbo_size);
	error = glGetError();
	if (error != 0) {
		std::cout << "GL ERROR DRAWING TERRAIN: " << error << std::endl;
	}
	glPopMatrix();
}

void WorldTerrain::draw_skypbox() {
	GLuint error;
	//Set skybox attributes
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);

	//Load the matrices
	GLfloat mvmat[16], promat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mvmat);
	glGetFloatv(GL_PROJECTION_MATRIX, promat);
	GLint currprog;

	//Sky!
	glUseProgram(game.get_state().get_skybox_prog());
	glGetIntegerv(GL_CURRENT_PROGRAM, &currprog);
	glGetProgramiv(game.get_state().get_skybox_prog(), GL_ATTACHED_SHADERS, &currprog);

	glUniformMatrix4fv(glGetUniformLocation(game.get_state().get_skybox_prog(), "view_matrix"), 1, false, mvmat);
	glUniformMatrix4fv(glGetUniformLocation(game.get_state().get_skybox_prog(), "proj_matrix"), 1, false, promat);

	glBindBuffer(GL_ARRAY_BUFFER, skybox_vbo);
	glEnableVertexAttribArray(game.get_state().skybox_shad.shader_attributes[0].first);
	glVertexAttribPointer(game.get_state().skybox_shad.shader_attributes[0].first, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(game.get_state().skybox_shad.shader_attributes[1].first);
	glVertexAttribPointer(game.get_state().skybox_shad.shader_attributes[1].first, 3, GL_FLOAT, GL_FALSE, 0, (char*)NULL+sizeof(float)*skybox_num_tri*3);

	glDrawArrays(GL_TRIANGLES, 0, skybox_num_tri);

	error = glGetError();
	if (error != 0) {
		std::cout << "GL ERROR DRAWING SKYBOX: " << error << std::endl;
	}

	// Stars!
	glUseProgram(game.get_state().get_stars_prog());
	glGetIntegerv(GL_CURRENT_PROGRAM, &currprog);
	glGetProgramiv(game.get_state().get_stars_prog(), GL_ATTACHED_SHADERS, &currprog);

	glUniformMatrix4fv(glGetUniformLocation(game.get_state().get_skybox_prog(), "view_matrix"), 1, false, mvmat);
	glUniformMatrix4fv(glGetUniformLocation(game.get_state().get_skybox_prog(), "proj_matrix"), 1, false, promat);

	glBindBuffer(GL_ARRAY_BUFFER, stars_vbo);
	glEnableVertexAttribArray(game.get_state().stars_shad.shader_attributes[0].first);
	glVertexAttribPointer(game.get_state().stars_shad.shader_attributes[0].first, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glPointSize(10);
	glDrawArrays(GL_POINTS, 0, stars.size());

	error = glGetError();
	if (error != 0) {
		std::cout << "GL ERROR DRAWING STARS: " << error << std::endl;
	}
	// float w = 1.0, h = 1.0;
	// glColor3f(1.0, 1.0, 1.0);
	// glBindTexture(GL_TEXTURE_2D, game.get_state().grumptex);
	// for (vector<array<float, 2>>::const_iterator s = stars.cbegin(); s != stars.cend(); s++) {
	// 	glEnable(GL_TEXTURE_2D);
	// 	glEnable(GL_BLEND);
	// 	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// 	glBegin(GL_QUADS);
	// 	glColor4f(1, 1, 1, 1);
	// 	glTexCoord2f(0, 0);  sphereVertex((*s)[0]-(star_size/sin((*s)[1]-star_size)), (*s)[1]-star_size, 1.0);
	// 	glTexCoord2f(w, 0);  sphereVertex((*s)[0]+(star_size/sin((*s)[1]-star_size)), (*s)[1]-star_size, 1.0);
	// 	glTexCoord2f(w, h);  sphereVertex((*s)[0]+(star_size/sin((*s)[1]+star_size)), (*s)[1]+star_size, 1.0);
	// 	glTexCoord2f(0, h);  sphereVertex((*s)[0]-(star_size/sin((*s)[1]+star_size)), (*s)[1]+star_size, 1.0);
	// 	glEnd();
	// 	glDisable(GL_TEXTURE_2D);
	// }

	//Unset the attributes
	glPopAttrib();
}

void WorldTerrain::generate_stars(unsigned int num_stars) {
	for (unsigned int star_no = 0; star_no < num_stars; star_no++) {
		float theta = (float)(rand())/(float)(RAND_MAX/(2*M_PI));
		float phi = ((float)M_PI/2.0)+asin(((float)(rand())/(float)(RAND_MAX/2))-1);

		stars.emplace_back(array<float, 2>({{theta, phi}}));
	}
}

float WorldTerrain::get_height(const float x, const float y) {
	return 20.0;
	// return world_grid.get_height(x/terrain_scale[0], y/terrain_scale[0])*terrain_scale[1];
}

const float * WorldTerrain::get_scale() {
	return terrain_scale.data();
}

void WorldTerrain::write_grid(const string filename) {
	std::ofstream ofile;
	ofile.open(filename);
	for (int x = grid_size[0]; x < grid_size[2]; x++) {
		for (int y = grid_size[1]; y < grid_size[3]; y++) {
			ofile << world_grid.get_height(x, y) << '|';
		}
		ofile << std::endl;
	}
	ofile.close();
}