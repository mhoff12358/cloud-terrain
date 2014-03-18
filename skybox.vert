#version 150

uniform mat4 view_matrix;
uniform mat4 proj_matrix;

in vec3 in_position;
in vec3 in_color;

out vec3 frag_color;

void main()
{
	gl_Position = view_matrix*vec4(in_position.x, in_position.y, in_position.z, 1.0);
	frag_color = in_color;
}