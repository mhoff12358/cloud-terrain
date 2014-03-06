#version 150

uniform mat4 view_matrix;
uniform mat4 proj_matrix;

in vec3 in_position;
in vec3 in_color;
in vec3 in_normal;

out vec3 frag_color;
out vec3 light_normal;

void main()
{
	gl_Position = proj_matrix*view_matrix*vec4(in_position.x, in_position.y, in_position.z, 1.0);
	//frag_color = vec3(in_position.z, 0, 1-in_position.z);
	frag_color = in_color;
	light_normal = in_normal;
}