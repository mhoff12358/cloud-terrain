#version 150

uniform mat4 view_matrix;
uniform mat4 proj_matrix;
uniform float water_level;

in vec3 in_position;

out vec3 frag_color;

void main()
{
	gl_Position = proj_matrix*view_matrix*vec4(in_position.x, in_position.y, water_level, 1.0);
	frag_color = vec4(0.0, 0.0, 1.0, 1.0);
}