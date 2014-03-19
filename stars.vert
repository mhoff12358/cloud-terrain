#version 150

uniform mat4 view_matrix;
uniform mat4 proj_matrix;

in vec3 in_position;

void main()
{
	gl_Position = proj_matrix*view_matrix*vec4(in_position.x, in_position.y, in_position.z, 1.0);
}