#version 150

uniform vec3 sun_dir;
uniform float ambient;

in vec3 frag_color;
in vec3 light_normal;
out vec4 final_color;

void main()
{
   	final_color = vec4(frag_color * (length(dot(sun_dir, light_normal))/2+ambient), 1.0);
}