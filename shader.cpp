#include "shader.h"

int ShaderProgram::load_shader_file(const string& file_name, string& file_contents) {
	ifstream src_file;
	src_file.open(file_name);

	if (src_file.is_open()) {
		//The file successfully opened, load the file's contents
		src_file.seekg(0, std::ios::end);
		file_contents.resize(src_file.tellg());
		src_file.seekg(0, std::ios::beg);
		src_file.read(&file_contents[0],file_contents.size());
		src_file.close();
	} else {
		//The file could not be opened report an error.
		return 1;
	}
	return 0;
}

void ShaderProgram::create_program() {
	program = glCreateProgram();
}

int ShaderProgram::add_shader(const GLenum shader_type, const string& file_name) {
	GLuint shader = glCreateShader(shader_type);

	//Load in the source code from the file
	string shader_source;
	if (!load_shader_file(file_name, shader_source)) return 1;

	//Set the source and compile the shader
	const char * src = shader_source.data();
	glShaderSource(shader, 1, (const GLchar**)&src, NULL);
	glCompileShader(shader);
	GLint compile_success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_success);
	if (compile_success == GL_FALSE) {
		//Check for error specifics
		GLint error_len = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &error_len);

		string error_msg;
		error_msg.reserve(error_len);
		glGetShaderInfoLog(shader, error_len, &error_len, (GLchar*)error_msg.data());

		//Log the error (currently just print)
		std::cout << error_msg << std::endl;

		glDeleteShader(shader);
		return 2;
	}

	//Attach the shader to the program
	glAttachShader(program, shader);

	//Add the shader to the list of included shaders
	shaders.push_back(pair<GLuint, GLenum>(shader, shader_type));
	std::cout << "SHADER SUCCESSFULLY ADDED" << std::endl;
	return 0;
}

void ShaderProgram::link_program() {
	glLinkProgram(program);
}

GLint ShaderProgram::get_program() {
	// return program;
	return 0;
}
