
#include <fstream>
#include <string>
#include <iostream>

using std::string;
using std::ifstream;

class ShaderProgram {
private:
	// GLuint vert_shad, frag_shad, tess_shad, geo_shad;	
	char * load_shader_file(const string&);

public:
	void go();

};
