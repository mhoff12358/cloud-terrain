
#ifndef __SHADER_H_INCLUDED__
#define __SHADER_H_INCLUDED__

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <utility>

#include <GL/glew.h>

using std::string;
using std::ifstream;
using std::vector;
using std::pair;

class ShaderProgram {
private:
	// GLuint vert_shad, frag_shad, tess_shad, geo_shad;	
	int load_shader_file(const string&, string&);

	GLuint program;
	vector<pair<GLuint, GLenum>> shaders;

public:
	int add_shader(const GLenum, const string&);

	void create_program();
	void link_program();
	GLint get_program();

	void add_attribute(string);

	vector<pair<unsigned int, string>> shader_attributes;

};

#endif