#include "shader.h"

string& ShaderProgram::load_shader_file(const string& file_name) {
	ifstream src_file;
	src_file.open(file_name.c_str());
	string file_contents;

	if (src_file.is_open()) {
		//The file successfully opened, load the file's contents
		src_file.seekg(0, std::ios::end);
		file_contents.resize(src_file.tellg());
		src_file.seekg(0, std::ios::beg);
		src_file.read(&file_contents[0],file_contents.size());
		src_file.close();
	} else {
		//The file could not be opened report an error.
		return NULL;
	}
	std::cout << file_contents << std::endl;
	return NULL;
}

void ShaderProgram::go() {
	load_shader_file("shader.cpp");
}

int main() {
	ShaderProgram sp;
	sp.go();
	return 0;
}