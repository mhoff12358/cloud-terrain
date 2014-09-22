#version 150

uniform mat4 view_matrix;
uniform mat4 proj_matrix;
uniform vec3 sun_dir;
uniform float ambient;

in vec3 in_position;
in vec3 in_color;

out vec3 frag_color;

void main()
{
	gl_Position = proj_matrix*view_matrix*vec4(in_position, 1.0);
	frag_color = in_color;
}