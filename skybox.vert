#version 150

uniform mat4 view_matrix, proj_matrix;

in vec3 in_position;
in vec3 in_color;

out vec3 frag_color;

void main()
{
	gl_Position = vec4(in_position.x, in_position.y, in_position.z, 1.0);
	frag_color = vec3(0, 1, 0);
}